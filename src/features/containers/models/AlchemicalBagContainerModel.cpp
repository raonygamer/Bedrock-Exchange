#include "features/containers/models/AlchemicalBagContainerModel.hpp"

#include "mc/src/common/world/containers/managers/models/LevelContainerManagerModel.hpp"
#include "features/ModGlobals.hpp"

AlchemicalBagContainerModel::AlchemicalBagContainerModel(ContainerEnumName name, int size, Player& player, int bagIndex) :
	LevelContainerModel(name, size, player, BlockActorType::Undefined, { 0, 0, 0 }, ContainerCategory::Default),
	mBagIndex(bagIndex)
{
}

Container* AlchemicalBagContainerModel::_getContainer()
{
	auto* component = mPlayer.tryGetComponent<AlchemicalBagContainerComponent>();
	if (!component)
		AssertFail("AlchemicalBagContainerComponent not found on player");
	return component->mContainers[mBagIndex].get();
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
		ActorContainerType(CustomActorContainerType::FirstAlchemicalBag + mBagIndex),
		BlockPos{ 0, 0, 0 },
		container->mContainerRuntimeId
	};
}