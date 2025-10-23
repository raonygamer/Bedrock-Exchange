#include "features/items/MatterShovel.hpp"
#include "features/utility/BlockUtils.hpp"
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

	/*size_t currentMode = getMode(stack);
	std::string modeNameKey = std::format("hover.ee2:{}.text", getModeName(currentMode));
	outText += std::format("\n§f{}: §b{}§r", "hover.ee2:mode.text"_i18n, getI18n().get(modeNameKey, nullptr));*/
}

std::vector<std::pair<BlockPos, const Block*>> MatterShovel::getBlocksForMode(const ItemStackBase& stack, BlockSource& region, const BlockPos& center, const Directions& directions) const {
	/*size_t mode = getMode(stack);
	auto blocks = BlockUtils::getBlocksForMode(mode, region, center, directions);
	for (auto it = blocks.begin(); it != blocks.end();) {
		if (!stack.canDestroyOptimally(*it->second)) {
			it = blocks.erase(it);
		} else {
			++it;
		}
	}
	return blocks;*/
	return {};
}
