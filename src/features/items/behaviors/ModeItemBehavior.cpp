#include "ModeItemBehavior.hpp"
#include <format>
#include "amethyst/runtime/ModContext.hpp"
#include "mc/src/common/locale/I18n.hpp"
#include "mc/src-client/common/client/gui/gui/GuiData.hpp"

ModeItemBehavior::ModeItemBehavior(Item* item, const std::vector<std::string>& modes, size_t defaultMode = 0) :
	mItem(item),
	mModes(modes),
	mDefaultMode(defaultMode)
{
	item->mTags.push_back({ "ee2:switch_mode_item" });
}

void ModeItemBehavior::setMode(ItemStackBase& stack, size_t mode) {
	if (mode >= mModes.size())
		AssertFail("Mode index out of bounds");

	if (!stack.mUserData)
		stack.setUserData(std::make_unique<CompoundTag>());
	if (!stack.mUserData->contains("Mode"))
		stack.mUserData->put("Mode", IntTag(int(mode)));
	else
		stack.mUserData->getIntTag("Mode")->data = int(mode);

	if (!Amethyst::IsOnMainClientThread())
		return;
	auto& client = *Amethyst::GetClientCtx().mClientInstance;
	auto modeNameKey = std::format("hover.ee2:{}.text", getModeName(mode));
	client.mGuiData->displayLocalizedMessage(std::format(
		"Switched to §b{}§r Mode",
		getI18n().get(modeNameKey, nullptr)
	));
}

size_t ModeItemBehavior::getMode(const ItemStackBase& stack) const {
	if (!stack.mUserData)
		return mDefaultMode;
	auto* tag = stack.mUserData->get("Mode");
	if (tag && tag->getId() == Tag::Type::Int)
		return size_t(static_cast<IntTag*>(tag)->data);
	return mDefaultMode;
}

void ModeItemBehavior::nextMode(ItemStackBase& stack) {
	size_t current = getMode(stack);
	current = (current + 1) % mModes.size();
	setMode(stack, current);
}

void ModeItemBehavior::previousMode(ItemStackBase& stack) {
	size_t current = getMode(stack);
	if (current == 0)
		current = mModes.size() - 1;
	else
		current = (current - 1) % mModes.size();
	setMode(stack, current);
}

std::string ModeItemBehavior::getModeName(size_t mode) const {
	if (mode < mModes.size())
		return mModes[mode];
	AssertFail("Mode index out of bounds");
}

