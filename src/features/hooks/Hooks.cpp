#include "features/hooks/Hooks.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"

#include "amethyst/runtime/AmethystContext.hpp"

#include "minecraft/src/common/world/containers/models/ContainerModel.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"

extern ActorContainerType AlchemicalBagContainerType;
SafetyHookInline _Player_$constructor;
SafetyHookInline _ContainerWeakRef_tryGetActorContainer;

void Player_$constructor(Player* self, void* a2, void* a3, void* a4, void* a5, void* a6, void* a7, void* a8, void* a9, void* a10, void* a11, void* a12, void* a13, void* a14) {
	_Player_$constructor.thiscall(self, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);
	
    if (!self->tryGetComponent<AlchemicalBagContainerComponent>()) {
		auto& component = self->addComponent<AlchemicalBagContainerComponent>();
		component.mContainer = std::make_unique<AlchemicalBagContainer>(self);
		component.mContainer->init();
	}
}

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

void CreateAllHooks(AmethystContext& ctx) {
    auto& hooks = *ctx.mHookManager;
    HOOK(Player, $constructor);
    HOOK(ContainerWeakRef, tryGetActorContainer);
}