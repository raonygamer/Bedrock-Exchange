#include "features/items/MatterHoe.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"

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