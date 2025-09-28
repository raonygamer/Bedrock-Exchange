#pragma once
#include <cstdint>
#include <optional>
#include <string>

#include "mc/src/common/world/item/Item.hpp"

namespace ee2::emc {
	struct EMCResult {
		uint64_t emc;
		HashedString queryString;
	};

	class IEMCMapper {
	public:
		virtual ~IEMCMapper() = default;
		virtual void init() = 0;
		virtual std::optional<EMCResult> getEMC(const Item& item) = 0;
		virtual std::string getMapperName() = 0;
		virtual bool hasEMC(const Item& item) = 0;

		static uint64_t calculateItemEMC(uint64_t baseEMC, const ItemStackBase& stack) {
			if (stack.isNull() || baseEMC == 0)
				return 0;
			if (stack.getItem()->isDamageable() && stack.mUserData) {
				float durabilityPerc = 1.0f - (float)stack.getItem()->getDamageValue(stack.mUserData) / stack.getItem()->getMaxDamage();
				if (durabilityPerc < 0)
					durabilityPerc = 0;
				baseEMC *= durabilityPerc;
			}
			return baseEMC;
		}

		static uint64_t calculateStackEMC(uint64_t emc, const ItemStackBase& stack) {
			if (stack.isNull())
				return 0;
			return emc * stack.mCount;
		}
	};
}