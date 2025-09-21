#include "AlchemicalBagManagerController.hpp"
#include "features/containers/managers/models/AlchemicalBagManagerModel.hpp"

AlchemicalBagManagerController::AlchemicalBagManagerController(std::weak_ptr<AlchemicalBagManagerModel> model)
	: ContainerManagerController(model)
{
}

void AlchemicalBagManagerController::handleTakeAll(const SlotData& to, const SlotData& from)
{
	if (to.mCollectionName == "cursor_items" && isAlchemicalBagItem(from))
		return; // prevent taking out the bag onto the cursor
	ContainerManagerController::handleTakeAll(to, from);
}

void AlchemicalBagManagerController::handlePlaceOne(const SlotData& to, const SlotData& from)
{
	if (to.mCollectionName == "cursor_items" && isAlchemicalBagItem(from))
		return; // prevent taking out the bag onto the cursor
	ContainerManagerController::handlePlaceOne(to, from);
}

void AlchemicalBagManagerController::handlePlaceAmount(const SlotData& to, int amount, const SlotData& from)
{
	if (to.mCollectionName == "cursor_items" && isAlchemicalBagItem(from))
		return; // prevent taking out the bag onto the cursor
	ContainerManagerController::handlePlaceAmount(to, amount, from);
}

void AlchemicalBagManagerController::handleTakeAmount(const SlotData& to, int amount, const SlotData& from)
{
	if (to.mCollectionName == "cursor_items" && isAlchemicalBagItem(from))
		return; // prevent taking out the bag onto the cursor
	ContainerManagerController::handleTakeAmount(to, amount, from);
}

void AlchemicalBagManagerController::handleTakeHalf(const SlotData& to, const SlotData& from)
{
	if (to.mCollectionName == "cursor_items" && isAlchemicalBagItem(from))
		return; // prevent taking out the bag onto the cursor
	ContainerManagerController::handleTakeHalf(to, from);
}

void AlchemicalBagManagerController::handleSwap(const SlotData& to, const SlotData& from)
{
	if (to.mCollectionName == "cursor_items" && isAlchemicalBagItem(from))
		return; // prevent taking out the bag onto the cursor
	ContainerManagerController::handleSwap(to, from);
}

void AlchemicalBagManagerController::handleSplitSingle(const SlotData& to, const SlotData& from)
{
	if (to.mCollectionName == "cursor_items" && isAlchemicalBagItem(from))
		return; // prevent taking out the bag onto the cursor
	ContainerManagerController::handleSplitSingle(to, from);
}

void AlchemicalBagManagerController::handleSplitMultiple(const SelectedSlotInfo& selectedInfo, const ItemInstance& itemInstance, const SlotData& to)
{
	if (to.mCollectionName == "cursor_items" && isAlchemicalBagItem(selectedInfo.mSlot))
		return; // prevent taking out the bag onto the cursor
	ContainerManagerController::handleSplitMultiple(selectedInfo, itemInstance, to);
}

bool AlchemicalBagManagerController::isAlchemicalBagItem(const SlotData& slotData)
{
	auto* containerModel = mContainerManagerModel.lock()->getContainerByName(slotData.mCollectionName);
	if (!containerModel)
		return false;
	auto* fromItem = containerModel->getItemStack(slotData.mCollectionIndex).getItem();
	if (!fromItem)
		return false;
	return fromItem->mFullName.getString().starts_with("ee2:") && fromItem->mFullName.getString().ends_with("_alchemical_bag");
}