#pragma once
#include "mc/src/common/world/inventory/FillingContainer.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src/common/world/actor/player/Inventory.hpp"

class AlchemicalBagContainer :
	public FillingContainer
{
public:
	std::string mColor;
	int mColorIndex;

	AlchemicalBagContainer(Player* player, const std::string& color, int colorIndex) :
		FillingContainer(player, 104, ContainerType::CONTAINER),
		mColor(color),
		mColorIndex(colorIndex)
	{
	}

	virtual void readAdditionalSaveData(const CompoundTag& tag) {
		
	}

	virtual void addAdditionalSaveData(CompoundTag& tag) {
		
	}

	virtual void setItem(int slot, const ItemStack& item) override {
		Log::Info("Setting item in alchemical bag slot {}: {}, client: {}", slot, item.toString(), mPlayer->isClientSide());
		FillingContainer::setItem(slot, item);
	}
};