#include "BasicChargeableItem.hpp"

BasicChargeableItem::BasicChargeableItem(const std::string& identifier, short numId, short maxCharge, short steps, short startingCharge) :
	Item(identifier, numId),
	ChargeableItemMixin(this, maxCharge, steps, startingCharge) {
}