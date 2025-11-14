#include "features/behaviors/items/types/ModeItem.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"

#include <format>

#include "amethyst/runtime/ModContext.hpp"

#include "mc/src/common/locale/I18n.hpp"
#include "mc/src-client/common/client/gui/gui/GuiData.hpp"
#include "mc/src/common/world/level/Level.hpp"
#include "mc/src/common/world/actor/ActorDamageSource.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src-client/common/client/player/LocalPlayer.hpp"
#include "mc/src/common/world/entity/components/ActorUniqueIDComponent.hpp"

ModeItem::ModeItem(BehaviorStorage* owner, const std::vector<std::string>& modes, size_t defaultMode, bool notifyModeChange) :
	ItemBehavior(owner),
	mModes(modes),
	mDefaultMode(defaultMode)
{
	if (notifyModeChange) {
		addEventListener<SwitchModeEvent>([this](SwitchModeEvent* e) {
			sendModeChangeMessage(e->mNewMode);
			return false;
		});
	}
}

std::string ModeItem::getBehaviorName() const {
	return "ModeItem";
}

bool ModeItem::isSinglePerStorage() const {
	return true;
}

std::type_index ModeItem::getTypeIndex() const {
	return typeid(ModeItem);
}

void ModeItem::setMode(ItemStackBase& stack, size_t mode) {
	size_t last = getMode(stack);
	if (mode >= mModes.size())
		AssertFail("Mode index out of bounds");

	if (!stack.mUserData)
		stack.setUserData(std::make_unique<CompoundTag>());
	if (!stack.mUserData->contains("Mode"))
		stack.mUserData->put("Mode", IntTag(int(mode)));
	else
		stack.mUserData->getIntTag("Mode")->data = int(mode);
	SwitchModeEvent event(*mOwner, *this, stack, last, mode);
	triggerEvent(&event);
}

size_t ModeItem::getMode(const ItemStackBase& stack) const {
	if (!stack.mUserData)
		return mDefaultMode;
	auto* tag = stack.mUserData->get("Mode");
	if (tag && tag->getId() == Tag::Type::Int)
		return size_t(static_cast<IntTag*>(tag)->data);
	return mDefaultMode;
}

void ModeItem::nextMode(ItemStackBase& stack) {
	size_t current = getMode(stack);
	current = (current + 1) % mModes.size();
	setMode(stack, current);
}

void ModeItem::previousMode(ItemStackBase& stack) {
	size_t current = getMode(stack);
	if (current == 0)
		current = mModes.size() - 1;
	else
		current = (current - 1) % mModes.size();
	setMode(stack, current);
}

const std::string& ModeItem::getModeName(size_t mode) const {
	if (mode < mModes.size())
		return mModes[mode];
	AssertFail("Mode index out of bounds");
}

std::string ModeItem::getModeDescription(size_t mode) const {
	auto modeNameKey = std::format("hover.ee2:{}.text", getModeName(mode));
	return std::format(
		"§f{}: §b{}§r", "hover.ee2:mode.text"_i18n, 
		getI18n().get(modeNameKey, nullptr)
	);
}

void ModeItem::sendModeChangeMessage(size_t mode) const {
#ifdef CLIENT
	if (!Amethyst::IsOnMainClientThread())
		return;
	auto& client = *Amethyst::GetClientCtx().mClientInstance;
	auto modeNameKey = std::format("hover.ee2:{}.text", getModeName(mode));
	client.mGuiData->displayLocalizedMessage(std::format(
		"Switched to §b{}§r Mode",
		getI18n().get(modeNameKey, nullptr)
	));
#endif
}

ModeItem::SwitchModeEvent::SwitchModeEvent(BehaviorStorage& storage, Behavior& sender, ItemStackBase& stack, size_t lastMode, size_t newMode) :
	Behavior::Event(storage, sender),
	mStack(stack),
	mLastMode(lastMode),
	mNewMode(newMode)
{
}

std::type_index ModeItem::SwitchModeEvent::getTypeIndex() const {
	return typeid(ModeItem::SwitchModeEvent);
}

ModeItem* ModeItem::tryGet(const ItemStackBase& stack) {
	auto* storage = ItemBehaviorStorage::tryGetStorage(stack);
	if (!storage)
		return nullptr;
	return storage->getFirstBehavior<ModeItem>();
}

ModeItem* ModeItem::tryGet(Item* item) {
	if (!item)
		AssertFail("Item pointer was null");
	auto* storage = ItemBehaviorStorage::tryGetStorage(*item);
	if (!storage)
		return nullptr;
	return storage->getFirstBehavior<ModeItem>();
}

const ModeItem* ModeItem::tryGet(const Item* item) {
	if (!item)
		AssertFail("Item pointer was null");
	auto* storage = ItemBehaviorStorage::tryGetStorage(*item);
	if (!storage)
		return nullptr;
	return storage->getFirstBehavior<ModeItem>();
}

ModeItem* ModeItem::tryGet(ItemBehaviorStorage* storage) {
	return storage->getFirstBehavior<ModeItem>();
}