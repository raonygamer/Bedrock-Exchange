#pragma once
#include "mc/src/common/world/item/WeaponItem.hpp"
#include "features/items/MatterSword.hpp"
#include "features/items/Tiers.hpp"
#include "features/items/behaviors/ModeItemBehavior.hpp"

class RedMatterSword :
	public MatterSword,
	public ModeItemBehavior
{
public:
	RedMatterSword(const std::string& identifier, short numId);
	virtual void appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const override;
};