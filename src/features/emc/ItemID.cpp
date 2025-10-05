#include "features/emc/ItemID.hpp"

namespace ee2::emc {
bool ItemID::equals(const ItemID& other) const {
	if (name != other.name)
		return false;
	if (aux != 0x7fff && other.aux != 0x7fff && aux != other.aux)
		return false;
	return true;
}

bool ItemID::operator==(const ItemID& other) const {
	return equals(other);
}

bool ItemID::operator!=(const ItemID& other) const {
	return !equals(other);
}

ItemID ItemID::fromItemName(const HashedString& name, uint16_t aux)
{
	return ItemID{ name, {}, aux };
}

ItemID ItemID::fromItem(const Item& item, uint16_t aux) {
	return ItemID{ item.mFullName, &item.mTags, aux };
}

ItemID ItemID::fromItem(const Item* item, uint16_t aux) {
	if (item == nullptr)
		return {};
	return fromItem(*item, aux);
}

ItemID ItemID::fromEntry(const ItemDescriptor::ItemEntry& itemEntry) {
	if (itemEntry.isNull())
		return {};
	return fromItem(*itemEntry.mItem, itemEntry.mAuxValue);
}

ItemID ItemID::fromStack(const ItemStackBase& stack) {
	if (stack.isNull())
		return {};
	return fromItem(stack.getItem(), stack.mAuxValue);
}

EMCValue const EMCValue::EMPTY = EMCValue(0);
} // namespace ee2::emc