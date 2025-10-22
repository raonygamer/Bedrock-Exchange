#include "features/items/MatterPickaxe.hpp"
#include <format>
#include "amethyst/runtime/ModContext.hpp"
#include "mc/src/common/locale/I18n.hpp"
#include "mc/src/common/world/level/BlockSource.hpp"
#include "mc/src-client/common/client/gui/gui/GuiData.hpp"

MatterPickaxe::MatterPickaxe(
	const std::string& name, 
	short id, 
	const Item::Tier& tier, 
	short maxCharge, 
	short steps, 
	short startingCharge, 
	const std::vector<std::string>& modes, 
	size_t defaultMode) :
	PickaxeItem(name, id, tier),
	ChargeableItemBehavior(this, maxCharge, steps, startingCharge),
	ModeItemBehavior(this, modes, defaultMode)
{
}

bool MatterPickaxe::isDamageable() const {
	return false;
}

void MatterPickaxe::appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const {
	PickaxeItem::appendFormattedHovertext(stack, level, outText, showCategory);
	size_t currentMode = getMode(stack);
	std::string modeNameKey = std::format("hover.ee2:{}.text", getModeName(currentMode));
	outText += std::format("\n§f{}: §b{}§r", "hover.ee2:mode.text"_i18n, getI18n().get(modeNameKey, nullptr));
}

std::vector<std::pair<BlockPos, const Block*>> MatterPickaxe::getBlocksForMode(const ItemStackBase& stack, BlockSource& region, const BlockPos& center, const Directions& directions) {
	size_t mode = getMode(stack);
	if (mode == 0) {
		return { { center, &region.getBlock(center) } };
	}

	std::vector<std::pair<BlockPos, const Block*>> result;
	const int lineSize = 1;
	for (int i = -lineSize; i <= lineSize; i++) {
		BlockPos newPos = center;
		Vec3 offset;

		switch (mode) {
		case 1: offset = directions.mUp * i; break;      // Tallshot
		case 2: offset = directions.mRight * i; break;   // Wideshot
		case 3: offset = directions.mForward * i; break; // Longshot
		}

		newPos.x += (int)std::round(offset.x);
		newPos.y += (int)std::round(offset.y);
		newPos.z += (int)std::round(offset.z);
		result.push_back({ newPos, &region.getBlock(newPos) });
	}
	return result;
}