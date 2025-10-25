#include "features/items/MatterShovel.hpp"
#include "features/utility/BlockUtils.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"
#include "features/behaviors/items/types/ChargeableItem.hpp"

#include <format>

#include "amethyst/runtime/ModContext.hpp"

#include "mc/src/common/locale/I18n.hpp"
#include "mc/src/common/world/level/BlockSource.hpp"
#include "mc/src-client/common/client/gui/gui/GuiData.hpp"
#include "mc/src/common/world/actor/Actor.hpp"

MatterShovel::MatterShovel(const std::string& name, short id, const Item::Tier& tier) :
	ShovelItem(name, id, tier),
	ItemBehaviorStorage(this)
{
}

bool MatterShovel::isDamageable() const {
	return false;
}

void MatterShovel::appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const {
	ShovelItem::appendFormattedHovertext(stack, level, outText, showCategory);
	if (auto* modeBehavior = ModeItem::tryGet(stack)) {
		size_t mode = modeBehavior->getMode(stack);
		outText += ("\n" + modeBehavior->getModeDescription(mode));
	}
}

float MatterShovel::getDestroySpeed(const ItemStackBase& stack, const Block& block) const {
	float speed = ShovelItem::getDestroySpeed(stack, block);
	if (auto* chargeableBehavior = ChargeableItem::tryGet(stack)) {
		short charge = chargeableBehavior->getCharge(stack);
		short maxCharge = chargeableBehavior->mMaxCharge;
		float chargeMultiplier = (static_cast<float>(charge) / static_cast<float>(maxCharge));
		return speed * std::clamp(chargeMultiplier, 0.2f, 1.0f);
	}
	return speed;
}