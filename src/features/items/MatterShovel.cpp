#include "features/items/MatterShovel.hpp"
#include "features/utility/BlockUtils.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"

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

std::vector<std::pair<BlockPos, const Block*>> MatterShovel::getBlocksForMode(const ItemStackBase& stack, BlockSource& region, const BlockPos& center, const Directions& directions) const {
	auto* modeBehavior = ModeItem::tryGet(stack);
	if (!modeBehavior)
		return {};
	size_t mode = modeBehavior->getMode(stack);

	auto blocks = BlockUtils::getBlocksForMode(mode, region, center, directions);
	for (auto it = blocks.begin(); it != blocks.end();) {
		if (!stack.canDestroyOptimally(*it->second)) {
			it = blocks.erase(it);
		} else {
			++it;
		}
	}
	return blocks;
}
