#include "features/emc/EMCRepository.hpp"

#include "features/emc/mappers/DefaultEMCMapper.hpp"
#include "features/emc/mappers/RecipeEMCMapper.hpp"

namespace ee2::emc {
	std::vector<std::unique_ptr<IEMCMapper>> EMCRepository::mAllMappers = {};
	std::unordered_map<HashedString, uint64_t> EMCRepository::mCachedEMCValues = {};

	void EMCRepository::initDefaultItems() {
		for (auto it = mAllMappers.begin(); it != mAllMappers.end(); ) {
			if ((*it)->getMapperName() == "DefaultEMCMapper") {
				it = mAllMappers.erase(it);
			}
			else {
				++it;
			}
		}
		registerMapper<DefaultEMCMapper>().init();
	}

	void EMCRepository::initRecipeItems(Recipes& recipes) {
		for (auto it = mAllMappers.begin(); it != mAllMappers.end(); ) {
			if ((*it)->getMapperName() == "RecipeEMCMapper") {
				it = mAllMappers.erase(it);
			}
			else {
				++it;
			}
		}
		registerMapper<RecipeEMCMapper>(recipes).init();
	}

	std::optional<uint64_t> EMCRepository::getBaseEMCForItem(const Item& item) {
		auto it = mCachedEMCValues.find(item.mFullName);
		if (it != mCachedEMCValues.end()) {
			return it->second;
		}
		for (const auto& mapper : mAllMappers) {
			auto emc = mapper->getEMC(item);
			if (emc.has_value()) {
				mCachedEMCValues[emc.value().queryString] = emc.value().emc;
				return emc.value().emc;
			}
		}
		return std::nullopt;
	}

	void EMCRepository::clearCache() {
		mCachedEMCValues.clear();
	}
}