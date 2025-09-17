#include "AlchemicalBagManagerModel.hpp"
#include "minecraft/src/common/world/item/ItemStack.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/world/actor/player/Inventory.hpp"
#include "minecraft/src/common/world/containers/models/InventoryContainerModel.hpp"
#include "minecraft/src/common/world/inventory/network/ContainerScreenContext.hpp"

AlchemicalBagManagerModel::AlchemicalBagManagerModel(ContainerID id, Player& player) :
	ContainerManagerModel(id, player)
{
	setContainerType(ContainerType(35));
}

std::vector<ItemStack> AlchemicalBagManagerModel::getItemCopies()
{
	Log::Info("AlchemicalBagManagerModel::getItemCopies called");
	return {};
}

void AlchemicalBagManagerModel::setSlot(int slot, const ItemStack& stack, bool)
{
	Log::Info("AlchemicalBagManagerModel::setSlot called for slot {}", slot);
}

const ItemStack& AlchemicalBagManagerModel::getSlot(int slot)
{
	Log::Info("AlchemicalBagManagerModel::getSlot called for slot {}", slot);
	return ItemStack::EMPTY_ITEM;
}

void AlchemicalBagManagerModel::setData(int, int)
{
	Log::Info("AlchemicalBagManagerModel::setData called");
}

void AlchemicalBagManagerModel::broadcastChanges()
{
	
}

ContainerScreenContext AlchemicalBagManagerModel::_postInit()
{
	auto& supplies = mPlayer.getSupplies();
	auto hotbarSize = 9;
	auto containerSize = supplies.mInventory->getContainerSize();
	_addContainer(std::make_shared<ContainerModel>(ContainerEnumName::LevelEntityContainer, 1, ContainerCategory::Default, false));
	_addContainer(std::make_shared<InventoryContainerModel>(ContainerEnumName::HotbarContainer, hotbarSize, mPlayer));
	_addContainer(std::make_shared<InventoryContainerModel>(ContainerEnumName::InventoryContainer, containerSize - hotbarSize, mPlayer));
	return mScreenContext;
}