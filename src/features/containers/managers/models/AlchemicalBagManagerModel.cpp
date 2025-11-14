#include "AlchemicalBagManagerModel.hpp"
#include "features/ModGlobals.hpp"
#include "mc/src/common/world/item/ItemStack.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src/common/world/actor/player/Inventory.hpp"
#include "mc/src/common/world/containers/models/InventoryContainerModel.hpp"
#include "mc/src/common/world/containers/models/PlayerUIContainerModel.hpp"
#include "mc/src/common/world/inventory/network/ContainerScreenContext.hpp"
#include "features/containers/models/AlchemicalBagContainerModel.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"

AlchemicalBagManagerModel::AlchemicalBagManagerModel(ContainerID id, Player& player, int bagIndex) :
	LevelContainerManagerModel(id, player, player.getUniqueID()),
	mBagIndex(bagIndex)
{
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
	_addContainer(std::make_shared<AlchemicalBagContainerModel>(ContainerEnumName::ContainerItems, 104, mPlayer, mBagIndex));
	_addContainer(std::make_shared<PlayerUIContainerModel>(ContainerEnumName::CursorContainer, mPlayer));
	_addContainer(std::make_shared<InventoryContainerModel>(ContainerEnumName::HotbarContainer, hotbarSize, mPlayer));
	_addContainer(std::make_shared<InventoryContainerModel>(ContainerEnumName::InventoryContainer, containerSize - hotbarSize, mPlayer));
	_addContainer(std::make_shared<InventoryContainerModel>(ContainerEnumName::CombinedHotbarAndInventoryContainer, containerSize, mPlayer));
	for (auto& [name, container] : mContainers) {
		container->postInit();
	}
	return LevelContainerManagerModel::_postInit();
}