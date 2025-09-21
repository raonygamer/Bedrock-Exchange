#include "AlchemicalBagContainerModel.hpp"
#include "minecraft/src/common/world/containers/managers/models/LevelContainerManagerModel.hpp"

AlchemicalBagContainerModel::AlchemicalBagContainerModel(ContainerEnumName name, int size, Player& player) :
	LevelContainerModel(name, size, player, BlockActorType::Undefined, { 0, 0, 0 }, ContainerCategory::PlayerInventory)
{
}

Container* AlchemicalBagContainerModel::_getContainer()
{
	auto* component = mPlayer.tryGetComponent<AlchemicalBagContainerComponent>();
	Assert(component, "AlchemicalBagContainerComponent not found on player");
	return component->mContainers[0].get();
}

int AlchemicalBagContainerModel::_getContainerOffset()
{
	return 0;
}

ContainerWeakRef AlchemicalBagContainerModel::getContainerWeakRef()
{
	auto* container = _getContainer();
	return ContainerWeakRef{
		mPlayer.getUniqueID(),
		AlchemicalBagContainerType,
		BlockPos{ 0, 0, 0 },
		container->mContainerRuntimeId
	};
}