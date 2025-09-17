#pragma once
#include "minecraft/src/common/world/containers/managers/controllers/ContainerManagerController.hpp"
#include "minecraft/src/common/world/containers/managers/models/ContainerManagerModel.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/world/actor/player/Inventory.hpp"

class AlchemicalBagManagerModel;

class AlchemicalBagManagerController : 
	public ContainerManagerController 
{
public:
	AlchemicalBagManagerController(std::weak_ptr<AlchemicalBagManagerModel> model);
	virtual void handleTakeAll(const SlotData& a, const SlotData& b) {

	}
};