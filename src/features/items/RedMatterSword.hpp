#pragma once
#include "mc/src/common/world/item/WeaponItem.hpp"
#include "features/items/MatterSword.hpp"
#include "features/items/Tiers.hpp"

class RedMatterSword :
	public MatterSword
{
public:
	RedMatterSword(const std::string& identifier, short numId);
	virtual void appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const override;
};