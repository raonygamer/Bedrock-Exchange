#include "AlchemicalBagManagerModel.hpp"
#include "minecraft/src/common/world/item/ItemStack.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/world/actor/player/Inventory.hpp"
#include "minecraft/src/common/world/containers/models/InventoryContainerModel.hpp"
#include "minecraft/src/common/world/containers/models/PlayerUIContainerModel.hpp"
#include "minecraft/src/common/world/inventory/network/ContainerScreenContext.hpp"
#include "features/containers/models/AlchemicalBagContainerModel.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"

AlchemicalBagManagerModel::AlchemicalBagManagerModel(ContainerID id, Player& player) :
	LevelContainerManagerModel(id, player, player.getUniqueID())
{
	mScreenContext.mPlayer = &player;
	mScreenContext.mOwner = player.getUniqueID();
	mScreenContext.mScreenContainerType = ContainerType::CONTAINER;
	setContainerType(ContainerType::CONTAINER);
}

bool AlchemicalBagManagerModel::isValid() {
	return true;
}

ContainerScreenContext AlchemicalBagManagerModel::_postInit()
{
	auto& supplies = mPlayer.getSupplies();
	auto hotbarSize = 9;
	auto containerSize = supplies.mInventory->getContainerSize();
	_addContainer(std::make_shared<AlchemicalBagContainerModel>(ContainerEnumName::LevelEntityContainer, 104, mPlayer));
	_addContainer(std::make_shared<PlayerUIContainerModel>(ContainerEnumName::CursorContainer, mPlayer));
	_addContainer(std::make_shared<InventoryContainerModel>(ContainerEnumName::HotbarContainer, hotbarSize, mPlayer));
	_addContainer(std::make_shared<InventoryContainerModel>(ContainerEnumName::InventoryContainer, containerSize - hotbarSize, mPlayer));
	_addContainer(std::make_shared<InventoryContainerModel>(ContainerEnumName::CombinedHotbarAndInventoryContainer, containerSize, mPlayer));
	for (auto& [name, container] : mContainers) {
		container->postInit();
	}
	return mScreenContext;
}