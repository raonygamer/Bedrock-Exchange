#pragma once
#include "features/emc/ItemID.hpp"

#include <cstdint>
#include <optional>
#include <string>

#include "mc/src/common/world/item/Item.hpp"
#include "mc/src/common/world/item/ItemDescriptor.hpp"

namespace ee2::emc {
class IEMCMapper {
public:
	virtual ~IEMCMapper() = default;
	virtual void init() = 0;
	virtual EMCValue getEMC(const ItemID& identifier) = 0;
	virtual bool canHandleTags() = 0;
	virtual EMCValue getEMCByTag(const ItemTag& tag) = 0;
	virtual std::string getMapperName() = 0;
	virtual void setEMC(const ItemID& identifier, uint64_t emc) = 0;

	EMCValue getEMCByAnyTag(const std::vector<ItemTag>& tags) {
		for (const auto& tag : tags) {
			auto result = getEMCByTag(tag);
			if (result.emc != 0) {
				return result;
			}
		}
		return EMCValue::EMPTY;
	}
};
} // namespace ee2::emc