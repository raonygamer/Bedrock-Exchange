#pragma once
#include "minecraft/src/common/world/inventory/FillingContainer.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/world/actor/player/Inventory.hpp"

class AlchemicalBagContainer : 
	public FillingContainer 
{
public:
	AlchemicalBagContainer(Player* player) :
		FillingContainer(player, 104, ContainerType::CONTAINER) 
	{
	}

	virtual void setItem(int slot, const ItemStack& item) override 
	{
		if (mPlayer->isClientSide()) {
			Log::Info("Tried to set item on alchemical bag container on client side");
		}
		else {
			Log::Info("Setting item on alchemical bag container on server side");
		}
		FillingContainer::setItem(slot, item);
	}

	virtual void load(const ListTag& tag, const SemVersion& version, Level&) {

	}
};