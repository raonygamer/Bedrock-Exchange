#pragma once
#include "mc/src/common/world/item/Item.hpp"

class AlchemicalBagItem : public Item {
public:
	
	std::string mBagColor;

	AlchemicalBagItem(const std::string& name, short id, const std::string& color);
	ItemStack& use(ItemStack& stack, Player& player) const override;
};