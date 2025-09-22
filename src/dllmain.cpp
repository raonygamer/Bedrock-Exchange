#include "dllmain.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"

#include "features/hooks/Hooks.hpp"
#include "features/items/Items.hpp"
#include "features/blocks/Blocks.hpp"
#include "features/containers/AlchemicalBagContainer.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"

#include "minecraft/src/common/world/item/registry/ItemRegistry.hpp"
#include "minecraft/src/common/world/item/Item.hpp"
#include "minecraft/src-client/common/client/gui/screens/controllers/MinecraftScreenController.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/world/actor/player/Inventory.hpp"
#include "minecraft/src/common/world/containers/models/ContainerModel.hpp"
#include "minecraft/src/common/world/inventory/FillingContainer.hpp"

ActorContainerType AlchemicalBagContainerType;

ModFunction void Initialize(AmethystContext& ctx, const Mod& mod) 
{
    Amethyst::InitializeAmethystMod(ctx, mod);

	// Listen to RegisterItemsEvent to add new items
    ctx.mEventBus->AddListener<RegisterItemsEvent>([&](RegisterItemsEvent& event) {
        Items::RegisterAllItems(event, ctx);
    });

	// Listen to RegisterBlocksEvent to add new blocks
    ctx.mEventBus->AddListener<RegisterBlocksEvent>([&](RegisterBlocksEvent& event) {
		Blocks::RegisterAllBlocks(event, ctx);
	});

	// Listen to InitBlockGraphicsEvent to initialize block graphics
    ctx.mEventBus->AddListener<InitBlockGraphicsEvent>([&](InitBlockGraphicsEvent& event) {
        Blocks::InitAllBlockGraphics(event, ctx);
    });

	// Register new ActorContainerType
    ctx.mEnumAllocator->RegisterEnum<ActorContainerType>(ActorContainerType::PlayerEnderChest);
    AlchemicalBagContainerType = ctx.mEnumAllocator->GetNextValue<ActorContainerType>();

    // Register game hooks
	CreateAllHooks(ctx);
}
