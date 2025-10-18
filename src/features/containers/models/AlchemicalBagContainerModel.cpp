#include "features/containers/models/AlchemicalBagContainerModel.hpp"

#include "mc/src/common/world/containers/managers/models/LevelContainerManagerModel.hpp"
#include "features/ModGlobals.hpp"

AlchemicalBagContainerModel::AlchemicalBagContainerModel(ContainerEnumName name, int size, Player& player) :
	LevelContainerModel(name, size, player, BlockActorType::Undefined, { 0, 0, 0 }, ContainerCategory::Default)
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
		CustomActorContainerType::AlchemicalBag,
		BlockPos{ 0, 0, 0 },
		container->mContainerRuntimeId
	};
}