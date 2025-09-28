#include "features/emc/mappers/RecipeEMCMapper.hpp"
#include "features/emc/EMCRepository.hpp"

namespace ee2::emc{
	RecipeEMCMapper::RecipeEMCMapper(Recipes& recipes) :
		mRecipes(recipes)
	{
	}

	void RecipeEMCMapper::init()
	{
		/*for (const auto& [tag, recipeMap] : mRecipes.mRecipes) {
			for (const auto& [id, recipePtr] : recipeMap) {
				if (!recipePtr)
					continue;
				if (tag == "crafting_table") {
					const auto& results = recipePtr->getResultItem();
					int size = recipePtr->size();
					uint64_t sumEMC = 0;
					for (int x = 0; x < size; x++) {
						for (int y = 0; y < size; y++) {
							const auto& ingredient = recipePtr->getIngredient(x, y);
							if (ingredient.mImpl != nullptr) {
								auto item = ingredient.mImpl->getItem();
								if (!item.mItem) {
									continue;
								}
								auto emc = ee2::emc::EMCRepository::getBaseEMCForItem(*item.mItem);
								if (emc.has_value()) {
									sumEMC += emc.value() * ingredient.mStackSize;
								}
							}
						}
					}
					if (sumEMC == 0) {
						continue;
					}
					for (const auto& result : results) {
						if (!result.isNull() && result.getItem()) {
							uint64_t emcPerItem = sumEMC / result.mCount;
							Log::Info("RecipeEMCMapper: Setting EMC for item '{}' to {} (from recipe '{}')", result.getItem()->mFullName.getString(), emcPerItem, id);
							mEMCValues[result.getItem()->mFullName] = emcPerItem;
						}
					}
				}
			}
		}*/
		bool changed = true;

    while (changed) {
        changed = false;

        for (const auto& [tag, recipeMap] : mRecipes.mRecipes) {
            if (tag != "crafting_table")
                continue;

            for (const auto& [id, recipePtr] : recipeMap) {
                if (!recipePtr)
                    continue;

                // Already resolved all outputs? Skip
                bool allResolved = true;
                for (const auto& result : recipePtr->getResultItem()) {
                    if (!result.isNull() && result.getItem() &&
                        mEMCValues.find(result.getItem()->mFullName) == mEMCValues.end()) {
                        allResolved = false;
                        break;
                    }
                }
                if (allResolved)
                    continue;

                // Sum ingredients EMC
                uint64_t sumEMC = 0;
                bool allKnown = true;
                int size = recipePtr->size();

                for (int x = 0; x < size; x++) {
                    for (int y = 0; y < size; y++) {
                        const auto& ingredient = recipePtr->getIngredient(x, y);
                        if (!ingredient.mImpl)
                            continue;

                        auto item = ingredient.mImpl->getItem();
                        if (!item.mItem)
                            continue;

                        auto emc = EMCRepository::getBaseEMCForItem(*item.mItem);
                        if (!emc.has_value()) {
                            allKnown = false;
                            break;
                        }
                        sumEMC += emc.value() * ingredient.mStackSize;
                    }
                    if (!allKnown)
                        break;
                }

                if (!allKnown || sumEMC == 0)
                    continue;

                // Assign EMC to results
                for (const auto& result : recipePtr->getResultItem()) {
                    if (!result.isNull() && result.getItem()) {
                        uint64_t emcPerItem = sumEMC / result.mCount;
                        if (mEMCValues.find(result.getItem()->mFullName) == mEMCValues.end()) {
                            Log::Info("RecipeEMCMapper: Setting EMC for item '{}' to {} (from recipe '{}')",
                                result.getItem()->mFullName.getString(), emcPerItem, id);
                            mEMCValues[result.getItem()->mFullName] = emcPerItem;
                            changed = true;
                        }
                    }
                }
            }
        }
    }
	}

	std::optional<EMCResult> RecipeEMCMapper::getEMC(const Item& item) {
		auto it = mEMCValues.find(item.mFullName);
		if (it != mEMCValues.end()) {
			Log::Info("RecipeEMCMapper: EMC for item '{}' is {}", item.mFullName.getString(), it->second);
			return EMCResult { it->second, item.mFullName };
		}
		return std::nullopt;
	}

	std::string RecipeEMCMapper::getMapperName() {
		return "RecipeEMCMapper";
	}

	bool RecipeEMCMapper::hasEMC(const Item& item) {
		return getEMC(item).has_value();
	}
}