#pragma once
#include "minecraft/src/common/world/containers/managers/controllers/ContainerManagerController.hpp"
#include "minecraft/src/common/world/containers/managers/models/ContainerManagerModel.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/world/actor/player/Inventory.hpp"
#include "minecraft/src/common/world/containers/controllers/ContainerController.hpp"
#include "minecraft/src/common/world/containers/models/ContainerModel.hpp"

class AlchemicalBagManagerModel;

class AlchemicalBagManagerController : 
	public ContainerManagerController 
{
public:
	AlchemicalBagManagerController(std::weak_ptr<AlchemicalBagManagerModel> model);
};