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
	virtual void handlePlaceInItemContainer(const SlotData& from, const SlotData& to) {
		Log::Info("handlePlaceInItemContainer {}:{} -> {}:{}", from.mCollectionName, from.mCollectionIndex, to.mCollectionName, to.mCollectionIndex);
	}
};