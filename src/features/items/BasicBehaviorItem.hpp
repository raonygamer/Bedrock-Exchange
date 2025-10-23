#pragma once
#include "mc/src/common/world/item/Item.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"

class BasicBehaviorItem : 
	public Item,
	public ItemBehaviorStorage
{
public:
	BasicBehaviorItem(const std::string& identifier, short numId);
};