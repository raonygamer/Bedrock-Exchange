#pragma once
#include <cstdint>
#include <optional>
#include <unordered_map>
#include <string>

#include "mc/src/common/world/item/Item.hpp"

#include "features/emc/mappers/IEMCMapper.hpp"

class Recipes;

namespace ee2::emc {
class EMCRepository {
	static std::vector<std::unique_ptr<IEMCMapper>> mAllMappers;
	static std::vector<std::pair<ItemID, EMCValue>> mCachedEMCValues;
	static std::unordered_map<HashedString, EMCValue> mTagCachedEMCValues;
public:
	static void init(Recipes& recipes);
	static EMCValue getItemEMC(const ItemID& identifier, bool noCache = false, bool lookupTags = true);
	static EMCValue getItemEMCByTag(const ItemTag& tag, bool noCache = false);
	static void clearCache();

	template<typename T, typename... Args>
	requires std::is_base_of_v<IEMCMapper, T> && std::is_constructible_v<T, Args...>
	static T& registerMapper(Args&&... args) {
		mAllMappers.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		return *static_cast<T*>(mAllMappers.back().get());
	}
};
}