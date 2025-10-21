#pragma once
#include "mc/src/common/world/item/Item.hpp"
#include "mc/src/common/world/item/ItemStack.hpp"

class ChargeableItemBehavior
{
public:
	Item* mItem = nullptr;
	const short mMaxCharge;
	const short mChargeSteps;
	const short mChargePerStep;
	const short mStartingCharge = 0;

	ChargeableItemBehavior(Item* item, short maxCharge, short steps, short startingCharge = 0);

	virtual void setCharge(ItemStackBase& stack, short charge);
	virtual short getCharge(const ItemStackBase& stack) const;
	virtual void charge(ItemStackBase& stack);
	virtual void uncharge(ItemStackBase& stack);
};