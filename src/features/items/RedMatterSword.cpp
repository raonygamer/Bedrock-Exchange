#include "features/items/RedMatterSword.hpp"
#include "mc/src/common/locale/I18n.hpp"

RedMatterSword::RedMatterSword(const std::string& identifier, short numId) :
	MatterSword(identifier, numId, *ee2::Tiers::RED_MATTER, 17, 3, 3),
	ModeItemBehavior(this, { "mode.slay_hostile", "mode.slay_all" }, 0)
{
}

void RedMatterSword::appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const {
	MatterSword::appendFormattedHovertext(stack, level, outText, showCategory);
	size_t currentMode = getMode(stack);
	std::string modeNameKey = std::format("hover.ee2:{}.text", getModeName(currentMode));
	outText += std::format("\n§f{}: §b{}§r", "hover.ee2:mode.text"_i18n, getI18n().get(modeNameKey, nullptr));
}
