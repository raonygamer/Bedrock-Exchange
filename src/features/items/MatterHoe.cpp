#include "features/items/MatterHoe.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"
#include "features/behaviors/items/types/ChargeableItem.hpp"

MatterHoe::MatterHoe(const std::string& name, short id, const Item::Tier& tier) :
	HoeItem(name, id, tier),
	ItemBehaviorStorage(this)
{
}

bool MatterHoe::isDamageable() const {
	return false;
}

void MatterHoe::appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const {
	HoeItem::appendFormattedHovertext(stack, level, outText, showCategory);
	if (auto* modeBehavior = ModeItem::tryGet(stack)) {
		size_t mode = modeBehavior->getMode(stack);
		outText += ("\n" + modeBehavior->getModeDescription(mode));
	}
}

float MatterHoe::getDestroySpeed(const ItemStackBase& stack, const Block& block) const {
	float speed = HoeItem::getDestroySpeed(stack, block);
	if (auto* chargeableBehavior = ChargeableItem::tryGet(stack)) {
		short charge = chargeableBehavior->getCharge(stack);
		short maxCharge = chargeableBehavior->mMaxCharge;
		float chargeMultiplier = (static_cast<float>(charge) / static_cast<float>(maxCharge));
		return speed * std::clamp(chargeMultiplier, 0.2f, 1.0f);
	}
	return speed;
}