#pragma once
#include "minecraft/src/common/world/item/Item.hpp"

class AlchemyBagItem : public Item {
public:
	static std::vector<std::string> sAlchemyBagColors;

	AlchemyBagItem(const std::string& name, short id, const std::string& color);
	ItemStack& use(ItemStack& stack, Player& player) const override;
};