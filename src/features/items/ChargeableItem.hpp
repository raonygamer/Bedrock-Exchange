#pragma once
#include "mc/src/common/world/item/Item.hpp"
#include "mc/src/common/world/item/ItemStack.hpp"

class ChargeableItem :
	public Item 
{
public:
	const short mMaxCharge;
	const short mChargeSteps;
	const short mChargePerStep;
	const short mStartingCharge = 0;

	ChargeableItem(const std::string& name, short id, short maxCharge, short steps, short startingCharge = 0);
	virtual short getMaxDamage() const override;
	virtual int getDamageChance(int unk0) const override;

	void setCharge(ItemStackBase& stack, short charge) const;
	short getCharge(const ItemStackBase& stack) const;
	void charge(ItemStackBase& stack) const;
	void uncharge(ItemStackBase& stack) const;
};