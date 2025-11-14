#pragma once
#include "mc/src/common/world/item/Item.hpp"

class AlchemicalBagItem : public Item {
public:
	std::string mBagColor;
	int mBagIndex;

	AlchemicalBagItem(const std::string& name, short id, const std::string& color);
	ItemStack& use(ItemStack& stack, Player& player) const override;
	static bool isAlchemicalBagItem(const ItemStackBase& stack);
};