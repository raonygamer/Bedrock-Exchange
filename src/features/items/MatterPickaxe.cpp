#include "features/items/MatterPickaxe.hpp"
#include "features/utility/BlockUtils.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"

#include <format>

#include "amethyst/runtime/ModContext.hpp"

#include "mc/src/common/locale/I18n.hpp"
#include "mc/src/common/world/level/BlockSource.hpp"
#include "mc/src-client/common/client/gui/gui/GuiData.hpp"

MatterPickaxe::MatterPickaxe(const std::string& name, short id, const Item::Tier& tier) :
	PickaxeItem(name, id, tier),
	ItemBehaviorStorage(this)
{
}

bool MatterPickaxe::isDamageable() const {
	return false;
}

void MatterPickaxe::appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const {
	PickaxeItem::appendFormattedHovertext(stack, level, outText, showCategory);
	if (auto* modeBehavior = ModeItem::tryGet(stack)) {
		size_t mode = modeBehavior->getMode(stack);
		outText += ("\n" + modeBehavior->getModeDescription(mode));
	}
}

std::vector<std::pair<BlockPos, const Block*>> MatterPickaxe::getBlocksForMode(const ItemStackBase& stack, BlockSource& region, const BlockPos& center, const Directions& directions) {
	auto* modeBehavior = ModeItem::tryGet(stack);
	if (!modeBehavior)
		return {};
	size_t mode = modeBehavior->getMode(stack);

	auto blocks = BlockUtils::getBlocksForMode(mode, region, center, directions);
	for (auto it = blocks.begin(); it != blocks.end();) {
		if (!stack.canDestroyOptimally(*it->second)) {
			it = blocks.erase(it);
		}
		else {
			++it;
		}
	}
	return blocks;
}