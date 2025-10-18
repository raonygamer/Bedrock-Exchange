#include "features/items/MatterSword.hpp"

MatterSword::MatterSword(const std::string& identifier, short numId, const Item::Tier& tier, int32_t baseDmg, short maxCharge, short steps, short startingCharge) :
	WeaponItem(identifier, numId, tier),
	ChargeableItemMixin(this, maxCharge, steps, startingCharge),
	mBaseDamage(baseDmg)
{
	mDamage = mBaseDamage;
}

bool MatterSword::isDamageable() const {
	return false;
}

void MatterSword::setCharge(ItemStackBase& stack, short charge) {
	ChargeableItemMixin::setCharge(stack, charge);
	// TODO: figure out how to do that damage scale per stack instead of per item
	mDamage = mBaseDamage + getCharge(stack);
}