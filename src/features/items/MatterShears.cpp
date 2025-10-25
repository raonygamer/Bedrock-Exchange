#include "features/items/MatterShears.hpp"
#include "features/behaviors/items/types/ChargeableItem.hpp"

MatterShears::MatterShears(const std::string& name, short id, const Item::Tier& tier) :
	ShearsItem(name, id),
	ItemBehaviorStorage(this),
	mTier(tier)
{
}

bool MatterShears::isDamageable() const {
	return false;
}

float MatterShears::getDestroySpeed(const ItemStackBase& stack, const Block& block) const {
	float speed = ShearsItem::getDestroySpeed(stack, block);
	if (auto* chargeableBehavior = ChargeableItem::tryGet(stack)) {
		short charge = chargeableBehavior->getCharge(stack);
		short maxCharge = chargeableBehavior->mMaxCharge;
		float chargeMultiplier = (static_cast<float>(charge) / static_cast<float>(maxCharge));
		return speed * std::clamp(chargeMultiplier, 0.2f, 1.0f);
	}
	return speed;
}