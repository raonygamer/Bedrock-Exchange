#pragma once
#include "minecraft/src/common/world/containers/managers/models/LevelContainerManagerModel.hpp"

class AlchemicalBagManagerModel :
	public LevelContainerManagerModel
{
public:
	AlchemicalBagManagerModel(ContainerID id, Player& player);
	virtual bool isValid();
    virtual ContainerScreenContext _postInit();
};