#pragma once
#include "mc/src/common/world/item/WeaponItem.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"
#include "features/behaviors/items/types/ChargeableItem.hpp"
#include "features/items/Tiers.hpp"

class MatterSword :
	public WeaponItem,
	public ItemBehaviorStorage
{
public:
	MatterSword(const std::string& identifier, short numId, const Item::Tier& tier, int32_t baseDmg);
	virtual bool isDamageable() const override;
	virtual void appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const override;

	void onSetCharge(ChargeableItem::SetChargeEvent* e);
};