#pragma once
#include "mc/src/common/world/containers/managers/models/LevelContainerManagerModel.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"

class AlchemicalBagManagerModel :
	public LevelContainerManagerModel
{
public:
	AlchemicalBagManagerModel(ContainerID id, Player& player);
	virtual bool isValid();
    virtual ContainerScreenContext _postInit();

	static bool isPlayerUsingAlchemicalBag(Player& player) {
		if (player.getContainerManagerModel().expired())
			return false;
		return false;
	}
};