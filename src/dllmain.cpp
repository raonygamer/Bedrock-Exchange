#include "dllmain.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"
#include "minecraft/src/common/world/item/registry/ItemRegistry.hpp"
#include "minecraft/src/common/world/item/Item.hpp"
#include "features/items/Items.hpp"
#include "minecraft/src-client/common/client/gui/screens/controllers/MinecraftScreenController.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/world/actor/player/Inventory.hpp"
#include "minecraft/src/common/world/containers/models/ContainerModel.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"
#include "features/containers/AlchemicalBagContainer.hpp"
#include "minecraft/src/common/world/inventory/FillingContainer.hpp"

class Level;
class PacketSender;
enum class GameType;
class NetworkIdentifier;
namespace mce {
    class UUID;
}
class Certificate;
class EntityContext;

ActorContainerType AlchemicalBagContainerType;

SafetyHookInline _Player_Player;
void Player_Player(Player* self, void* a2, void* a3, void* a4, void* a5, void* a6, void* a7, void* a8, void* a9, void* a10, void* a11, void* a12, void* a13, void* a14) {
    _Player_Player.thiscall(self, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
    if (!self->tryGetComponent<AlchemicalBagContainerComponent>()) {
		auto& component = self->addComponent<AlchemicalBagContainerComponent>();
		component.mContainer = std::make_unique<AlchemicalBagContainer>(self);
        component.mContainer->init();
	}
}

SafetyHookInline _ContainerWeakRef_tryGetActorContainer;
Container* ContainerWeakRef_tryGetActorContainer(Actor& actor, ActorContainerType type) {
    if (type == AlchemicalBagContainerType) {
        auto* component = actor.tryGetComponent<AlchemicalBagContainerComponent>();
        if (component) {
            return component->mContainer.get();
        }
    }
    return _ContainerWeakRef_tryGetActorContainer.call<
        Container*,
        Actor&,
        ActorContainerType
    >(actor, type);
}

ModFunction void Initialize(AmethystContext& ctx) 
{
    Amethyst::InitializeAmethystMod(ctx);
    ctx.mEventBus->AddListener<RegisterItemsEvent>([&](RegisterItemsEvent& event) { Items::RegisterAllItems(event, ctx); });
    ctx.mEnumAllocator->RegisterEnum<ActorContainerType>(ActorContainerType::PlayerEnderChest);
    AlchemicalBagContainerType = ctx.mEnumAllocator->GetNextValue<ActorContainerType>(); // ActorContainerType::AlchemicalBag
    auto& hooks = *ctx.mHookManager;
    _Player_Player = safetyhook::create_inline(0x2737340 + GetMinecraftBaseAddress(), &Player_Player);
	HOOK(ContainerWeakRef, tryGetActorContainer);
}
