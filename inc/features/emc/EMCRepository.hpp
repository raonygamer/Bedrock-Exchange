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
	static std::unordered_map<HashedString, uint64_t> mCachedEMCValues;
public:
	static void initDefaultItems();
	static void initRecipeItems(Recipes& recipes);
	static std::optional<uint64_t> getBaseEMCForItem(const Item& item);
	static void clearCache();

	template<typename T, typename... Args>
	requires std::is_base_of_v<IEMCMapper, T> && std::is_constructible_v<T, Args...>
	static T& registerMapper(Args&&... args) {
		mAllMappers.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		return *static_cast<T*>(mAllMappers.back().get());
	}
};
}