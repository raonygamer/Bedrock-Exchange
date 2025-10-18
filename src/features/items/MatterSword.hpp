#pragma once
#include "mc/src/common/world/item/WeaponItem.hpp"
#include "features/items/ChargeableItemMixin.hpp"
#include "features/items/Tiers.hpp"

class MatterSword :
	public WeaponItem,
	public ChargeableItemMixin
{
public:
	const int32_t mBaseDamage;
	MatterSword(const std::string& identifier, short numId, const Item::Tier& tier, int32_t baseDmg, short maxCharge, short steps, short startingCharge = 0);
	virtual bool isDamageable() const override;
	virtual void setCharge(ItemStackBase& stack, short charge) override;
};