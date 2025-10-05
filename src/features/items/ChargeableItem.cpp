#include "features/items/ChargeableItem.hpp"

ChargeableItem::ChargeableItem(const std::string& name, short id, short maxCharge, short steps, short startingCharge) :
    Item(name, id),
	mMaxCharge(maxCharge),
	mChargeSteps(steps),
	mChargePerStep(maxCharge / steps),
	mStartingCharge(startingCharge)
{
	setMaxStackSize(1);
	mMaxDamage = maxCharge;
	mTags.push_back({ "ee2:chargeable_item" });
}

short ChargeableItem::getMaxDamage() const {
	return mMaxCharge;
}

int ChargeableItem::getDamageChance(int unk0) const {
	return 0;
}

void ChargeableItem::setCharge(ItemStackBase& stack, short charge) const {
	if (!stack.mUserData)
		stack.setUserData(std::make_unique<CompoundTag>());

	if (!stack.mUserData->contains("Charge"))
		stack.mUserData->put("Charge", IntTag(charge));
	else
		stack.mUserData->getIntTag("Charge")->data = charge;
}

short ChargeableItem::getCharge(const ItemStackBase& stack) const {
	if (stack.mUserData && stack.mUserData->contains("Charge"))
		return stack.mUserData->getInt("Charge");
	return 0;
}

void ChargeableItem::charge(ItemStackBase& stack) const {
	short currentCharge = getCharge(stack);
	short nextCharge = currentCharge + mChargePerStep;
	if (nextCharge > mMaxCharge)
		return;
	setCharge(stack, nextCharge);
}

void ChargeableItem::uncharge(ItemStackBase& stack) const {
	short currentCharge = getCharge(stack);
	short nextCharge = currentCharge - mChargePerStep;
	if (nextCharge < 0)
		return;
	setCharge(stack, nextCharge);
}
