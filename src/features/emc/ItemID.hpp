#pragma once
#include "mc/src-deps/core/string/StringHash.hpp"
#include "mc/src/common/world/item/Item.hpp"
#include "mc/src/common/world/item/ItemDescriptor.hpp"

#include <cstdint>

namespace ee2::emc {
struct ItemID {
	HashedString name = {};
	const std::vector<ItemTag>* tags = nullptr;
	uint16_t aux = 0x7fff;

	bool equals(const ItemID& other) const;
	bool operator==(const ItemID& other) const;
	bool operator!=(const ItemID& other) const;
	static ItemID fromItemName(const HashedString& name, uint16_t aux = 0x7fff);
	static ItemID fromItem(const Item& item, uint16_t aux = 0x7fff);
	static ItemID fromItem(const Item* item, uint16_t aux = 0x7fff);
	static ItemID fromEntry(const ItemDescriptor::ItemEntry& itemEntry);
	static ItemID fromStack(const ItemStackBase& stack);
};

struct EMCValue {
	static const EMCValue EMPTY;
	uint64_t emc = 0;
	bool fromTag = false;
	ItemTag tag = {};

	EMCValue() = default;
	EMCValue(uint64_t emc) : 
		emc(emc)
	{
	}

	EMCValue(uint64_t emc, const ItemTag& tag) :
		emc(emc),
		fromTag(true),
		tag(tag)
	{
	}

	operator bool() const {
		return emc != 0;
	}

	operator uint64_t() const {
		return emc;
	}
};
} // namespace ee2::emc