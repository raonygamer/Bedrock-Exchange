#pragma once
#include "features/behaviors/items/ItemBehavior.hpp"

class Item;
class ItemStackBase;
class ItemBehaviorStorage;
class ModeItem :
	public ItemBehavior
{
public:
	class SwitchModeEvent :
		public Behavior::Event
	{
	public:
		ItemStackBase& mStack;
		size_t mLastMode;
		size_t mNewMode;
		SwitchModeEvent(BehaviorStorage& storage, Behavior& sender, ItemStackBase& stack, size_t lastMode, size_t newMode);
		std::type_index getTypeIndex() const override;
	};

	std::vector<std::string> mModes;
	const size_t mDefaultMode = 0;

	ModeItem(BehaviorStorage* owner, const std::vector<std::string>& modes, size_t defaultMode = 0, bool notifyModeChange = true);
	virtual std::string getBehaviorName() const override;
	virtual bool isSinglePerStorage() const override;
	virtual std::type_index getTypeIndex() const override;

	void setMode(ItemStackBase& stack, size_t mode);
	size_t getMode(const ItemStackBase& stack) const;
	void nextMode(ItemStackBase& stack);
	void previousMode(ItemStackBase& stack);
	const std::string& getModeName(size_t mode) const;
	std::string getModeDescription(size_t mode) const;

	void sendModeChangeMessage(size_t mode) const;

	static ModeItem* tryGet(const ItemStackBase& stack);
	static const ModeItem* tryGet(const Item* item);
	static ModeItem* tryGet(Item* item);
	static ModeItem* tryGet(ItemBehaviorStorage* storage);
};