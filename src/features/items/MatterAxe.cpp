#include "features/items/MatterAxe.hpp"
#include "features/utility/BlockUtils.hpp"
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
	/*size_t currentMode = getMode(stack);
	std::string modeNameKey = std::format("hover.ee2:{}.text", getModeName(currentMode));
	outText += std::format("\n§f{}: §b{}§r", "hover.ee2:mode.text"_i18n, getI18n().get(modeNameKey, nullptr));*/
}

std::vector<std::pair<BlockPos, const Block*>> MatterAxe::getBlocksForMode(const ItemStackBase& stack, BlockSource& region, const BlockPos& center, const Directions& directions) {
	//size_t mode = getMode(stack);
	//std::vector<std::pair<BlockPos, const Block*>> result;

	//const Block& targetBlock = region.getBlock(center);
	//if (!targetBlock.mLegacyBlock)
	//	return { { center, &region.getBlock(center) } };

	//// Standard
	//if (mode == 0) {
	//	result.push_back({ center, &region.getBlock(center) });
	//}
	//// Tree Chopper
	//else if (mode == 1) {
	//	if (targetBlock.mLegacyBlock->mTags.end() != std::find(targetBlock.mLegacyBlock->mTags.begin(), targetBlock.mLegacyBlock->mTags.end(), "wood")) {
	//		result = BlockUtils::floodFillBlocks(region, center, targetBlock.mLegacyBlock, 32, 100);
	//	}
	//	else {
	//		result.push_back({ center, &targetBlock });
	//	}
	//}
	//return result;
	return {};
}