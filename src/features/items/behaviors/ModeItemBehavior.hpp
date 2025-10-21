#pragma once
#include "mc/src/common/world/item/Item.hpp"

class ModeItemBehavior {
public:
	Item* mItem;
	std::vector<std::string> mModes;
	const size_t mDefaultMode = 0;

	ModeItemBehavior(Item* item, const std::vector<std::string>& modes, size_t defaultMode);
	virtual void setMode(ItemStackBase& stack, size_t mode);
	virtual size_t getMode(const ItemStackBase& stack) const;
	virtual void nextMode(ItemStackBase& stack);
	virtual void previousMode(ItemStackBase& stack);
	virtual std::string getModeName(size_t mode) const;
};