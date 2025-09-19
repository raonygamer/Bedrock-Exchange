#pragma once
#include "minecraft/src/common/world/containers/models/ContainerModel.hpp"
#include "minecraft/src/common/world/containers/models/InventoryContainerModel.hpp"
#include "minecraft/src/common/world/containers/models/LevelContainerModel.hpp"
#include "minecraft/src/common/world/inventory/FillingContainer.hpp"
#include "minecraft/src/common/world/item/ItemStack.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/world/containers/SlotData.hpp"

#include "features/components/AlchemicalBagContainerComponent.hpp"

extern ActorContainerType AlchemicalBagContainerType;
class LevelContainerManagerModel;

class AlchemicalBagContainerModel :
	public LevelContainerModel
{
public:
	AlchemicalBagContainerModel(ContainerEnumName name, int size, Player& player) :
		LevelContainerModel(name, size, player, BlockActorType::Undefined, { 0, 0, 0 }, ContainerCategory::Default)
	{

	}

	virtual Container* _getContainer() override
	{
		auto* component = mPlayer.tryGetComponent<AlchemicalBagContainerComponent>();
		Assert(component, "AlchemicalBagContainerComponent not found on player");
		return component->mContainer.get();
	}

	virtual int _getContainerOffset() override
	{
		return 0;
	}

	virtual ContainerWeakRef getContainerWeakRef() override
	{
		auto* container = _getContainer();
		return ContainerWeakRef{
			mPlayer.getUniqueID(),
			AlchemicalBagContainerType,
			BlockPos{ 0, 0, 0 },
			container->mContainerRuntimeId
		};
	}
};