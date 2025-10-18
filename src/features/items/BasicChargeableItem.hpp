#pragma once
#include "mc/src/common/world/item/Item.hpp"
#include "features/items/ChargeableItemMixin.hpp"

class BasicChargeableItem : 
	public Item,
	public ChargeableItemMixin
{
public:
	BasicChargeableItem(const std::string& identifier, short numId, short maxCharge, short steps, short startingCharge = 0);
};