#include "features/items/BasicBehaviorItem.hpp"

BasicBehaviorItem::BasicBehaviorItem(const std::string& identifier, short numId) :
	Item(identifier, numId),
	ItemBehaviorStorage(this) 
{
}