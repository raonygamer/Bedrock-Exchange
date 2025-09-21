#include "features/hooks/Hooks.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"
#include "features/containers/managers/models/AlchemicalBagManagerModel.hpp"
#include "features/ModGlobals.hpp"

#include "amethyst/runtime/AmethystContext.hpp"
#include "amethyst/runtime/ModContext.hpp"

#include "minecraft/src/common/world/containers/models/ContainerModel.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/world/containers/ContainerValidation.hpp"
#include "minecraft/src/common/world/inventory/network/SparseContainer.hpp"
#include "minecraft/src/common/world/inventory/simulation/validation/ContainerScreenValidation.hpp"
#include "minecraft/src/common/world/inventory/network/ItemStackNetManagerServer.hpp"
#include "minecraft/src/common/world/inventory/network/ItemStackRequestActionHandler.hpp"
#include "minecraft/src/common/world/inventory/simulation/ContainerValidatorFactory.hpp"

extern ActorContainerType AlchemicalBagContainerType;
SafetyHookInline _Player_$constructor;
SafetyHookInline _ContainerWeakRef_tryGetActorContainer;
SafetyHookInline _Player_readAdditionalSaveData;
SafetyHookInline _Player_addAdditionalSaveData;

void Player_readAdditionalSaveData(Player* self, const CompoundTag& tag, DataLoadHelper& helper) {
    _Player_readAdditionalSaveData.thiscall<
        void,
		Player*,
		const CompoundTag&,
		DataLoadHelper&
    >(self, tag, helper);
	Log::Info("Player readAdditionalSaveData called");
}

void Player_addAdditionalSaveData(Player* self, CompoundTag& tag) {
    _Player_addAdditionalSaveData.thiscall<
        void,
        Player*,
        CompoundTag&
    >(self, tag);
    Log::Info("Player addAdditionalSaveData called");
}


void Player_$constructor(Player* self, void* a2, void* a3, void* a4, void* a5, void* a6, void* a7, void* a8, void* a9, void* a10, void* a11, void* a12, void* a13, void* a14) {
	_Player_$constructor.thiscall(self, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);

    if (!self->isClientSide() && !_Player_readAdditionalSaveData.original<void*>() && !_Player_addAdditionalSaveData.original<void*>()) {
        auto& ctx = Amethyst::GetContext();
        ctx.mHookManager->CreateHookAbsolute(
			_Player_readAdditionalSaveData,
            reinterpret_cast<uintptr_t*>(GetVtable(self))[147],
			&Player_readAdditionalSaveData
		);

        ctx.mHookManager->CreateHookAbsolute(
            _Player_addAdditionalSaveData,
            reinterpret_cast<uintptr_t*>(GetVtable(self))[148],
            &Player_addAdditionalSaveData
        );
    }

    if (!self->tryGetComponent<AlchemicalBagContainerComponent>()) {
		auto& component = self->addComponent<AlchemicalBagContainerComponent>();
        auto& colors = ModGlobals::AlchemicalBagColors;
        for (int i = 0; i < colors.size(); i++) {
            auto container = std::make_unique<AlchemicalBagContainer>(self, colors[i], i);
            container->init();
			component.mContainers[i] = std::move(container);
		}
	}
}

Container* ContainerWeakRef_tryGetActorContainer(Actor& actor, ActorContainerType type) {
    if (type == AlchemicalBagContainerType) {
        auto* component = actor.tryGetComponent<AlchemicalBagContainerComponent>();
        if (component) {
            return component->mContainers[0].get();
        }
    }
    return _ContainerWeakRef_tryGetActorContainer.call<
        Container*,
        Actor&,
        ActorContainerType
    >(actor, type);
}

SafetyHookInline _ContainerValidatorFactory_getBackingContainer;
Container* ContainerValidatorFactory_getBackingContainer(ContainerEnumName name, const ContainerScreenContext& ctx) {
    if (ctx.mScreenContainerType == ContainerType::CONTAINER && name == ContainerEnumName::LevelEntityContainer)
    {
        if (ctx.mOwner.index() == 1) {
            auto& actorId = std::get<1>(ctx.mOwner);
            if (actorId == ctx.mPlayer->getUniqueID()) {
                auto* component = ctx.mPlayer->tryGetComponent<AlchemicalBagContainerComponent>();
                if (component) {
                    return component->mContainers[0].get();
                }
            }
		}
	}
    return _ContainerValidatorFactory_getBackingContainer.call<
        Container*, 
        ContainerEnumName, 
        const ContainerScreenContext&
    >(name, ctx);
}

void CreateAllHooks(AmethystContext& ctx) {
    auto& hooks = *ctx.mHookManager;
    HOOK(Player, $constructor);
    HOOK(ContainerWeakRef, tryGetActorContainer);
	HOOK(ContainerValidatorFactory, getBackingContainer);
}