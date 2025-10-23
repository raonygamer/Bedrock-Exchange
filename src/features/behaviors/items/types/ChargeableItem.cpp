#include "features/behaviors/items/types/ChargeableItem.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"

#include "amethyst/runtime/ModContext.hpp"

#include "mc/src/common/world/level/Level.hpp"
#include "mc/src/common/world/actor/ActorDamageSource.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src-client/common/client/player/LocalPlayer.hpp"
#include "mc/src/common/world/entity/components/ActorUniqueIDComponent.hpp"

ChargeableItem::ChargeableItem(BehaviorStorage* owner, short maxCharge, short steps, short startingCharge, bool shouldPlaySound) :
	ItemBehavior(owner),
	mMaxCharge(maxCharge),
	mChargeSteps(steps),
	mChargePerStep(maxCharge / steps),
	mStartingCharge(startingCharge)
{
	if (shouldPlaySound) {
		addEventListener<SetChargeEvent>([this](SetChargeEvent* event) -> bool {
			if (event->delta() >= 1)
				playChargeSound(event->mNewCharge);
			else if (event->delta() <= -1)
				playUnchargeSound(event->mNewCharge);
			return false;
		});
	}
}

std::string ChargeableItem::getBehaviorName() const {
	return getTypeIndex().name();
}

bool ChargeableItem::isSinglePerStorage() const {
	return true;
}

std::type_index ChargeableItem::getTypeIndex() const {
	return typeid(ChargeableItem);
}

ChargeableItem::SetChargeEvent::SetChargeEvent(BehaviorStorage& storage, Behavior& sender, ItemStackBase& stack, short lastCharge, short newCharge) :
	Event(storage, sender),
	mStack(stack),
	mLastCharge(lastCharge),
	mNewCharge(newCharge)
{
}

std::type_index ChargeableItem::SetChargeEvent::getTypeIndex() const {
	return typeid(ChargeableItem::SetChargeEvent);
}

short ChargeableItem::SetChargeEvent::delta() const {
	return mNewCharge - mLastCharge;
}

void ChargeableItem::setCharge(ItemStackBase& stack, short charge) {
	auto lastCharge = getCharge(stack);
	if (!stack.mUserData)
		stack.setUserData(std::make_unique<CompoundTag>());

	if (!stack.mUserData->contains("Charge"))
		stack.mUserData->put("Charge", IntTag(charge));
	else
		stack.mUserData->getIntTag("Charge")->data = charge;
	auto event = SetChargeEvent(*mOwner, *this, stack, lastCharge, charge);
	triggerEvent(&event);
}

short ChargeableItem::getCharge(const ItemStackBase& stack) const {
	if (stack.mUserData && stack.mUserData->contains("Charge"))
		return stack.mUserData->getInt("Charge");
	return 0;
}

void ChargeableItem::charge(ItemStackBase& stack) {
	short currentCharge = getCharge(stack);
	short nextCharge = currentCharge + mChargePerStep;
	if (nextCharge > mMaxCharge)
		return;

	setCharge(stack, nextCharge);
}

void ChargeableItem::uncharge(ItemStackBase& stack) {
	short currentCharge = getCharge(stack);
	short nextCharge = currentCharge - mChargePerStep;
	if (nextCharge < 0)
		return;

	setCharge(stack, nextCharge);
}

void ChargeableItem::playChargeSound(short charge) {
	if (!Amethyst::IsOnMainClientThread())
		return;
	auto& ctx = Amethyst::GetClientCtx();
	ClientInstance& client = *ctx.mClientInstance;
	auto& player = *client.getLocalPlayer();
	const float startPitch = 0.5f;
	float pitch = startPitch + (((float)charge / mMaxCharge) * (1.0f - startPitch));
	if (player.isClientSide())
		player.getLevel()->playSound("ui.item.charge", *player.getPosition(), 1.0f, pitch);
}

void ChargeableItem::playUnchargeSound(short charge) {
	if (!Amethyst::IsOnMainClientThread())
		return;
	auto& ctx = Amethyst::GetClientCtx();
	ClientInstance& client = *ctx.mClientInstance;
	auto& player = *client.getLocalPlayer();
	const float startPitch = 0.7f;
	float pitch = startPitch + (((float)charge / mMaxCharge) * (1.0f - startPitch));
	if (player.isClientSide())
		player.getLevel()->playSound("ui.item.uncharge", *player.getPosition(), 1.0f, pitch);
}

ChargeableItem* ChargeableItem::tryGet(const ItemStackBase& stack) {
	auto* storage = ItemBehaviorStorage::tryGetStorage(stack);
	if (!storage)
		return nullptr;
	return storage->getFirstBehavior<ChargeableItem>();
}

ChargeableItem* ChargeableItem::tryGet(Item* item) {
	if (!item)
		AssertFail("Item pointer was null");
	auto* storage = ItemBehaviorStorage::tryGetStorage(*item);
	if (!storage)
		return nullptr;
	return storage->getFirstBehavior<ChargeableItem>();
}

const ChargeableItem* ChargeableItem::tryGet(const Item* item) {
	if (!item)
		AssertFail("Item pointer was null");
	auto* storage = ItemBehaviorStorage::tryGetStorage(*item);
	if (!storage)
		return nullptr;
	return storage->getFirstBehavior<ChargeableItem>();
}

ChargeableItem* ChargeableItem::tryGet(ItemBehaviorStorage* storage) {
	return storage->getFirstBehavior<ChargeableItem>();
}