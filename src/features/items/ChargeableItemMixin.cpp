#include "features/items/ChargeableItemMixin.hpp"
#include "amethyst/runtime/ModContext.hpp"

#include "mc/src/common/world/level/Level.hpp"
#include "mc/src/common/world/actor/ActorDamageSource.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src-client/common/client/player/LocalPlayer.hpp"
#include "mc/src/common/world/entity/components/ActorUniqueIDComponent.hpp"

ChargeableItemMixin::ChargeableItemMixin(Item* item, short maxCharge, short steps, short startingCharge) :
	mItem(item),
	mMaxCharge(maxCharge),
	mChargeSteps(steps),
	mChargePerStep(maxCharge / steps),
	mStartingCharge(startingCharge)
{
	item->setMaxStackSize(1);
	item->mMaxDamage = maxCharge;
	item->mTags.push_back({ "ee2:chargeable_item" });
}

void ChargeableItemMixin::setCharge(ItemStackBase& stack, short charge) {
	if (!stack.mUserData)
		stack.setUserData(std::make_unique<CompoundTag>());

	if (!stack.mUserData->contains("Charge"))
		stack.mUserData->put("Charge", IntTag(charge));
	else
		stack.mUserData->getIntTag("Charge")->data = charge;
}

short ChargeableItemMixin::getCharge(const ItemStackBase& stack) const {
	if (stack.mUserData && stack.mUserData->contains("Charge"))
		return stack.mUserData->getInt("Charge");
	return 0;
}

void ChargeableItemMixin::charge(ItemStackBase& stack) {
	short currentCharge = getCharge(stack);
	short nextCharge = currentCharge + mChargePerStep;
	Log::Info("Charging item '{}' from {} to {}", mItem->mFullName.getString(), currentCharge, nextCharge);
	if (nextCharge > mMaxCharge)
		return;

	setCharge(stack, nextCharge);
	auto& ctx = Amethyst::GetClientCtx();
	ClientInstance& client = *ctx.mClientInstance;
	auto& player = *client.getLocalPlayer();
	const float startPitch = 0.5f;
	float pitch = startPitch + (((float)nextCharge / mMaxCharge) * (1.0f - startPitch));
	if (player.isClientSide())
		player.getLevel()->playSound("ui.item.charge", *player.getPosition(), 1.0f, pitch);
}

void ChargeableItemMixin::uncharge(ItemStackBase& stack) {
	short currentCharge = getCharge(stack);
	short nextCharge = currentCharge - mChargePerStep;
	Log::Info("Uncharging item '{}' from {} to {}", mItem->mFullName.getString(), currentCharge, nextCharge);
	if (nextCharge < 0)
		return;
	
	setCharge(stack, nextCharge);
	auto& ctx = Amethyst::GetClientCtx();
	ClientInstance& client = *ctx.mClientInstance;
	auto& player = *client.getLocalPlayer();
	const float startPitch = 0.7f;
	float pitch = startPitch + (((float)currentCharge / mMaxCharge) * (1.0f - startPitch));
	if (player.isClientSide())
		player.getLevel()->playSound("ui.item.uncharge", *player.getPosition(), 1.0f, pitch);
}
