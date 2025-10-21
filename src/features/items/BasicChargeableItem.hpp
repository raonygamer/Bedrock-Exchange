#pragma once
#include "mc/src/common/world/item/Item.hpp"
#include "features/items/behaviors/ChargeableItemBehavior.hpp"

class BasicChargeableItem : 
	public Item,
	public ChargeableItemBehavior
{
public:
	BasicChargeableItem(const std::string& identifier, short numId, short maxCharge, short steps, short startingCharge = 0);
};