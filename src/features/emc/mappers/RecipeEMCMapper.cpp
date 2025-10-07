#include "features/emc/mappers/RecipeEMCMapper.hpp"
#include "features/emc/EMCRepository.hpp"
#include "features/emc/EMCUtils.hpp"
#include "amethyst/runtime/ModContext.hpp"

namespace ee2::emc{
	RecipeEMCMapper::RecipeEMCMapper(Recipes& recipes) :
		mRecipes(recipes) {
	}

    void RecipeEMCMapper::init() {
		std::unordered_map<HashedString, std::shared_ptr<Recipe>> recipes;
		// Extract all crafting and smithing recipes
		for (const auto& [tag, recipesMap] : mRecipes.mRecipes) {
			for (const auto& [id, recipe] : recipesMap) {
				// We only care about crafting and smithing recipes
				if (tag != "crafting_table" && tag != "smithing_table")
					continue;
				recipes[id] = recipe;
			}
		}

		bool progress = true;
		// We loop until we make no progress
		while (progress) {
			progress = false;

			// Loop through all recipes
			for (const auto& [id, recipe] : recipes) {
				// Loop through all result items of the recipe
				std::vector<const ItemInstance*> allResultItems = {};
				for (const auto& resultItem : recipe->getResultItem()) {
					if (resultItem.isNull() || resultItem.mCount == 0)
						continue;
					allResultItems.push_back(&resultItem);
				}

				if (recipe->getResultItem().empty())
					continue;

				// Get the first result item (we only care about the main one for EMC mapping)
				const auto& resultItem = recipe->getResultItem()[0];

				if (resultItem.isNull() || resultItem.mCount == 0)
					continue;

				// Get the ItemID of the result item
				auto resultId = ItemID::fromStack(resultItem);

				uint64_t accumulatedEMC = 0;
				bool isComplete = true;

				// Loop through all ingredients of the recipe
				recipe->forEachIngredient([&](size_t x, size_t y, const RecipeIngredient& ingredient) {
					// If the ingredient is empty, skip it
					if (ingredient.mImpl == nullptr || ingredient.mStackSize == 0)
						return true;

					// Resolve the EMC value of the ingredient
					std::optional<uint64_t> ingredientEMC = resolveIngredientEMC(ingredient, allResultItems);

					// If we found a valid EMC value, add it to the total
					if (ingredientEMC.has_value()) {
						accumulatedEMC += ingredientEMC.value();
						return true;
					}

					// If we didn't find a valid EMC value, we can't calculate this recipe
					isComplete = false;
					return false;
				});

				// Divide the total EMC by the result item count
				const uint64_t finalEMC = std::max(1ull, accumulatedEMC / resultItem.mCount);

				// Get the existing EMC value for the result item
				uint64_t existingEMC = getEMC(resultId).emc;

				// If there is no existing EMC value, set it to max
				bool hasExisting = existingEMC != 0;
				if (existingEMC == 0)
					existingEMC = UINT64_MAX;

				// If we could calculate the EMC value and it's lower than the existing one, set it
				if (isComplete && finalEMC < existingEMC) {
					// Set the new EMC value
					setEMC(resultId, finalEMC);

					// Mark that we made progress and should loop again
					progress = true;
				}
			}
		}

		Log::Info("[{}] mapped {} EMC values for crafting and smithing recipes", getMapperName(), mEMCValues.size());
    }

	EMCValue RecipeEMCMapper::getEMC(const ItemID& identifier) {
		// Yeah, I know this is O(n) but CPUs don't care about that these days
		// And I can't do a exact match with unordered_map
		for (const auto& [id, emc] : mEMCValues) {
			if (id.name == identifier.name && (id.aux == 0x7fff || identifier.aux == 0x7fff || id.aux == identifier.aux)) {
				return EMCValue(emc);
			}
		}
		return EMCValue::EMPTY;
	}

	bool RecipeEMCMapper::canHandleTags() {
		return false;
	}

	EMCValue RecipeEMCMapper::getEMCByTag(const ItemTag& tag) {
		return EMCValue::EMPTY;
	}

	std::string RecipeEMCMapper::getMapperName() {
		return "RecipeEMCMapper";
	}

	void RecipeEMCMapper::setEMC(const ItemID& identifier, uint64_t emc) {
		// Try to find an existing entry
		auto it = std::find_if(
			mEMCValues.begin(),
			mEMCValues.end(),
			[&](const auto& pair) { return pair.first.equals(identifier); }
		);

		if (it != mEMCValues.end()) {
			// Update existing entry if it exists
			it->second = emc;
		}
		else {
			// If there was no existing entry, add a new one
			mEMCValues.push_back({ identifier, emc });
		}
	}

	std::optional<uint64_t> RecipeEMCMapper::resolveIngredientEMC(const RecipeIngredient& ingredient, std::vector<const ItemInstance*> resultItems) {
		auto& level = *Amethyst::GetLevel();
		if (!ingredient.mImpl)
			return std::nullopt;
		const auto& desc = *ingredient.mImpl;
		const auto ingredientCount = ingredient.mStackSize;

		uint64_t lastEMC = UINT64_MAX;

		// Get all item variants of that ingredient slot
		// And select the lowest EMC value
		bool lastIgnored = false;
		const auto items = desc.getAllItems();
		for (const auto& item : items) {
			lastIgnored = false;
			auto itemId = ItemID::fromEntry(item);

			// If the item is ignored skip it
			for (const auto& ignored : mIgnoredIngredientEMCs) {
				if (ignored.equals(itemId)) {
					lastIgnored = true;
					continue;
				}
			}

			// Try getting the EMC value for that item from the EMC repository
			// No caching so we always get the latest value since we are in init phase
			auto emcResult = EMCRepository::getItemEMC(
				itemId,
				true
			);

			// If we found a valid EMC value and it's lower than the last one, set it
			if (emcResult.emc != 0 && emcResult.emc < lastEMC) {
				lastEMC = emcResult.emc;

				// If the item is also a result item of the recipe, we subtract it's own EMC value
				for (const auto& resultItem : resultItems) {
					auto resultItemId = ItemID::fromStack(*resultItem);
					if (resultItemId.equals(itemId)) {
						uint64_t resultEMC = EMCRepository::getItemEMC(resultItemId, true);
						if (resultEMC == 0)
							continue;
						resultEMC = EMCUtils::calculateItemEMC(resultEMC, *resultItem, level);
						resultEMC = EMCUtils::calculateStackEMC(resultEMC, *resultItem);
						lastEMC -= resultEMC;
					}
				}

				if (lastEMC == 1)
					break; // Can't get lower than 1, stop searching
			}
		}

		lastEMC = std::max(0ull, lastEMC);

		// If the last item was ignored, we return 0 EMC
		if (lastIgnored)
			return 0;

		// If we didn't find a valid EMC value, return 0
		if (lastEMC == UINT64_MAX)
			return std::nullopt;

		// If we found a valid EMC value return it multiplied by the ingredient count
		return lastEMC * ingredientCount;
	}
}