#pragma once
#include "mc/src/common/world/containers/managers/controllers/ContainerManagerController.hpp"
#include "mc/src/common/world/containers/managers/models/ContainerManagerModel.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src/common/world/actor/player/Inventory.hpp"
#include "mc/src/common/world/containers/controllers/ContainerController.hpp"
#include "mc/src/common/world/containers/models/ContainerModel.hpp"

class AlchemicalBagManagerModel;

class AlchemicalBagManagerController : 
	public ContainerManagerController 
{
public:
	AlchemicalBagManagerController(std::weak_ptr<AlchemicalBagManagerModel> model);
	virtual void handleTakeAll(const SlotData& to, const SlotData& from);
	virtual void handlePlaceOne(const SlotData& to, const SlotData& from);
	virtual void handlePlaceAmount(const SlotData& to, int, const SlotData& from);
	virtual void handleTakeAmount(const SlotData&, int, const SlotData&);
	virtual void handleTakeHalf(const SlotData& to, const SlotData& from);
	virtual void handleSwap(const SlotData& to, const SlotData& from);
	virtual void handleSplitSingle(const SlotData& to, const SlotData& from);
	virtual void handleSplitMultiple(const SelectedSlotInfo& selectedInfo, const ItemInstance& itemInstance, const SlotData& from);
	virtual bool isAlchemicalBagItem(const SlotData& slotData);
};