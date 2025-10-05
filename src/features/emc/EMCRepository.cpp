#include "features/emc/EMCRepository.hpp"

#include "features/emc/mappers/DefaultEMCMapper.hpp"
#include "features/emc/mappers/RecipeEMCMapper.hpp"
#include "features/emc/mappers/BrewingEMCMapper.hpp"

namespace ee2::emc {
	std::vector<std::unique_ptr<IEMCMapper>> EMCRepository::mAllMappers = {};
	std::vector<std::pair<ItemID, EMCValue>> EMCRepository::mCachedEMCValues = {};
	std::unordered_map<HashedString, EMCValue> EMCRepository::mTagCachedEMCValues = {};

	void EMCRepository::init(Recipes& recipes) {
		std::erase_if(mAllMappers, [](const auto& mapper) {
			return 
				mapper->getMapperName() == "DefaultEMCMapper" ||
				mapper->getMapperName() == "RecipeEMCMapper" ||
				mapper->getMapperName() == "BrewingEMCMapper";
		});
		registerMapper<DefaultEMCMapper>().init();
		registerMapper<RecipeEMCMapper>(recipes).init();
		registerMapper<BrewingEMCMapper>().init();
		clearCache();
	}

	EMCValue EMCRepository::getItemEMC(const ItemID& identifier, bool noCache, bool lookupTags)
	{
		if (!noCache) {
			// Yeah, I know this is O(n) but CPUs don't care about that these days
			// And I can't do a exact match with unordered_map
			for (const auto& [id, result] : mCachedEMCValues) {
				if (id.name == identifier.name && (id.aux == 0x7fff || identifier.aux == 0x7fff || id.aux == identifier.aux)) {
					return result;
				}
			}

			if (lookupTags && identifier.tags) {
				for (const auto& tag : *identifier.tags) {
					EMCValue value = getItemEMCByTag(tag, noCache);
					if (value.emc)
						return value;
				}
			}
		}

		for (const auto& mapper : mAllMappers) {
			auto result = mapper->getEMC(identifier);
			if (result.emc) {
				if (!noCache)
					mCachedEMCValues.push_back({ identifier, result });
				return result;
			}

			if (lookupTags && mapper->canHandleTags() && identifier.tags) {
				for (const auto& tag : *identifier.tags) {
					result = getItemEMCByTag(tag, noCache);
					if (result.emc)
						return result;
				}
			}
		}
		return EMCValue::EMPTY;
	}

	EMCValue EMCRepository::getItemEMCByTag(const ItemTag& tag, bool noCache) {
		if (!noCache) {
			auto tagIt = mTagCachedEMCValues.find(tag);
			if (tagIt != mTagCachedEMCValues.end()) {
				return tagIt->second;
			}
		}
		for (const auto& mapper : mAllMappers) {
			if (!mapper->canHandleTags())
				continue;
			auto result = mapper->getEMCByTag(tag);
			if (result.emc) {
				if (!noCache)
					mTagCachedEMCValues.insert({ tag, result });
				return result;
			}
		}
		return EMCValue::EMPTY;
	}

	void EMCRepository::clearCache() {
		mCachedEMCValues.clear();
		mTagCachedEMCValues.clear();
	}
}