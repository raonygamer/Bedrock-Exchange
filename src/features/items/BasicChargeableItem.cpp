#include "features/items/BasicChargeableItem.hpp"

BasicChargeableItem::BasicChargeableItem(const std::string& identifier, short numId, short maxCharge, short steps, short startingCharge) :
	Item(identifier, numId),
	ChargeableItemBehavior(this, maxCharge, steps, startingCharge) {
}