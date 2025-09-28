#include "features/emc/mappers/DefaultEMCMapper.hpp"

namespace ee2::emc {
	void DefaultEMCMapper::init() {
		mEMCValues = {
			{ "minecraft:diamond", 8192 },
			{ "minecraft:logs", 32 }
		};
	}

	std::optional<EMCResult> DefaultEMCMapper::getEMC(const Item& item) {
		auto it = mEMCValues.find(item.mFullName);
		if (it != mEMCValues.end()) {
			Log::Info("DefaultEMCMapper: EMC for item '{}' is {}", item.mFullName.getString(), it->second);
			return EMCResult { it->second, item.mFullName };
		}
		for (const auto& tag : item.mTags) {
			it = mEMCValues.find(tag);
			if (it != mEMCValues.end()) {
				Log::Info("DefaultEMCMapper: EMC for tag '{}' is {}", tag.getString(), it->second);
				return EMCResult{ it->second, tag };
			}
		}
		return std::nullopt;
	}

	std::string DefaultEMCMapper::getMapperName() {
		return "DefaultEMCMapper";
	}

	bool DefaultEMCMapper::hasEMC(const Item& item) {
		return getEMC(item).has_value();
	}
} // namespace ee2::emc