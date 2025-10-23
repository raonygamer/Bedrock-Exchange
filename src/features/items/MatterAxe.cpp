#include "features/items/MatterAxe.hpp"
#include "features/utility/BlockUtils.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"

#include <format>

#include "amethyst/runtime/ModContext.hpp"

#include "mc/src/common/locale/I18n.hpp"
#include "mc/src/common/world/level/BlockSource.hpp"
#include "mc/src-client/common/client/gui/gui/GuiData.hpp"

MatterAxe::MatterAxe(const std::string& name, short id, const Item::Tier& tier) :
	HatchetItem(name, id, tier),
	ItemBehaviorStorage(this)
{
}

bool MatterAxe::isDamageable() const {
	return false;
}

void MatterAxe::appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const {
	HatchetItem::appendFormattedHovertext(stack, level, outText, showCategory);
	if (auto* modeBehavior = ModeItem::tryGet(stack)) {
		size_t mode = modeBehavior->getMode(stack);
		outText += ("\n" + modeBehavior->getModeDescription(mode));
	}
}

std::vector<std::pair<BlockPos, const Block*>> MatterAxe::getBlocksForMode(const ItemStackBase& stack, BlockSource& region, const BlockPos& center, const Directions& directions) {
	auto* modeBehavior = ModeItem::tryGet(stack);
	if (!modeBehavior)
		return {};
	size_t mode = modeBehavior->getMode(stack);

	const Block& targetBlock = region.getBlock(center);
	if (!targetBlock.mLegacyBlock)
		return {};

	// Tree Chopper
	if (mode == 1) {
		if (targetBlock.mLegacyBlock->mTags.end() != std::find(targetBlock.mLegacyBlock->mTags.begin(), targetBlock.mLegacyBlock->mTags.end(), "wood")) {
			return BlockUtils::floodFillBlocks(region, center, targetBlock.mLegacyBlock, 32, 100);
		}
	}
	return {};
}