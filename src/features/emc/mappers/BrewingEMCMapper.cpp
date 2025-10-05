#include "features/emc/mappers/BrewingEMCMapper.hpp"
#include "features/emc/EMCRepository.hpp"
#include "features/items/Items.hpp"

namespace ee2::emc {
	void BrewingEMCMapper::init() {
		const auto& mixes = PotionBrewing::mPotionMixes;
		const auto& itemReg = *Items::ItemRegistry;

		bool progress = true;
		// We loop until we make no progress
		while (progress) {
			progress = false;

			// Loop through all brewing recipes
			for (const auto& mix : mixes) {
				// If either the input or output descriptor is null, skip this recipe
				if (!mix.mFrom.mImpl || !mix.mTo.mImpl)
					continue;

				const auto& fromDesc = *mix.mFrom.mImpl;
				const auto& toDesc = *mix.mTo.mImpl;

				// Loop through all result items of the recipe
				const auto resultItems = toDesc.getAllItems();
				for (const auto& resultItem : resultItems) {
					if (resultItem.isNull())
						continue;

					// Get the ItemID of the result item
					ItemID resultId = ItemID::fromEntry(resultItem);

					// Get the lowest EMC value of all possible input items
					uint64_t inputEMC = getLowestEMC(fromDesc);
					if (inputEMC == 0)
						continue;

					// Get the ingredient item from the item registry
					auto ingrItemIt = itemReg.mIdToItemMap.find(mix.mIngredient.mItemId);
					if (ingrItemIt == itemReg.mIdToItemMap.end())
						continue;

					// Get the ItemID of the ingredient item
					auto ingrId = ItemID::fromItem(*ingrItemIt->second, mix.mIngredient.mData);

					// Get the EMC value of the ingredient item
					auto ingrEMC = EMCRepository::getItemEMC(ingrId, true).emc;
					if (ingrEMC == 0)
						continue;

					// Divide by 3 since brewing stands have 3 slots
					ingrEMC /= 3;

					// Total EMC is input item EMC + ingredient EMC
					uint64_t finalEMC = inputEMC + ingrEMC;
					uint64_t existingEMC = getEMC(resultId).emc;
					if (existingEMC == 0)
						existingEMC = UINT64_MAX;

					// If we found a valid EMC value and it's lower than the existing one, update it
					if (finalEMC > 0 && finalEMC < existingEMC) {
						// Set the new EMC value
						setEMC(resultId, finalEMC);

						// Mark that we made progress and should loop again
						progress = true;
					}

					// Also recalculate container mixes for potions
					if (recalculateContainerMixes(resultItem.mAuxValue)) {
						progress = true;
					}
				}
			}
		}

		Log::Info("[{}] mapped {} EMC values for brewing recipes", getMapperName(), mEMCValues.size());
	}

	EMCValue BrewingEMCMapper::getEMC(const ItemID& identifier) {
		// Yeah, I know this is O(n) but CPUs don't care about that these days
		// And I can't do a exact match with unordered_map
		for (const auto& [id, emc] : mEMCValues) {
			if (id.name == identifier.name && (id.aux == 0x7fff || identifier.aux == 0x7fff || id.aux == identifier.aux)) {
				return EMCValue(emc);
			}
		}
		return EMCValue::EMPTY;
	}

	bool BrewingEMCMapper::canHandleTags() {
		return false;
	}

	EMCValue BrewingEMCMapper::getEMCByTag(const ItemTag& tag) {
		return EMCValue::EMPTY;
	}

	std::string BrewingEMCMapper::getMapperName() {
		return "BrewingEMCMapper";
	}

	void BrewingEMCMapper::setEMC(const ItemID& identifier, uint64_t emc) {
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

	uint64_t BrewingEMCMapper::getLowestEMC(const ItemDescriptor::BaseDescriptor& descriptor) {
		uint64_t itemLowestEMC = UINT64_MAX;

		// Loop through all items in the descriptor and return the lowest EMC value
		descriptor.forEachItem([&](const Item& item, uint16_t aux) {
			// Get the ItemID of the item
			auto itemId = ItemID::fromItem(item, aux);

			// Get the EMC value of the item
			auto emcResult = EMCRepository::getItemEMC(itemId, true);

			// If the result emc is not 0 and lower than the current lowest, update it
			if (emcResult.emc != 0 && emcResult.emc < itemLowestEMC) {
				itemLowestEMC = emcResult.emc;
				if (itemLowestEMC == 1)
					return false; // Can't get lower than 1, stop searching
			}
			return true;
		});

		// If we found no valid EMC value, return 0
		if (itemLowestEMC == UINT64_MAX)
			return 0;

		// Return the lowest EMC value found
		return itemLowestEMC;
	}

	bool BrewingEMCMapper::recalculateContainerMixes(uint16_t aux) {
		const auto& containerMixes = PotionBrewing::mContainerMixes;
		const auto& itemReg = *Items::ItemRegistry;

		// Loop through all container mixes and recalculate their EMC values for the given aux value
		for (const auto& mix : containerMixes) {
			// Get the ItemID of the result item
			const auto resultId = ItemID::fromItem(mix.mTo, aux);

			// Get the ItemID of the input item
			const auto inputItemId = ItemID::fromItem(mix.mFrom, aux);

			// Get the emc value of the input item
			auto inputEMC = EMCRepository::getItemEMC(inputItemId, true).emc;

			// If the input item has no EMC value, skip this mix
			if (inputEMC == 0)
				continue;

			// Get the ingredient item from the item registry
			auto ingrItemIt = itemReg.mIdToItemMap.find(mix.mIngredient.mItemId);
			if (ingrItemIt == itemReg.mIdToItemMap.end())
				continue;

			// Get the ItemID of the ingredient item
			auto ingrId = ItemID::fromItem(*ingrItemIt->second, mix.mIngredient.mData);

			// Get the EMC value of the ingredient item
			auto ingrEMC = EMCRepository::getItemEMC(ingrId, true).emc;
			if (ingrEMC == 0)
				continue;

			// Divide by 3 since brewing stands have 3 slots
			ingrEMC /= 3;

			// Total EMC is input item EMC + ingredient EMC
			uint64_t finalEMC = inputEMC + ingrEMC;
			uint64_t existingEMC = getEMC(resultId).emc;
			if (existingEMC == 0)
				existingEMC = UINT64_MAX;

			// If we found a valid EMC value and it's lower than the existing one, update it
			if (finalEMC > 0 && finalEMC < existingEMC) {
				// Set the new EMC value
				setEMC(resultId, finalEMC);

				// Return that we made a change
				return true;
			}
		}

		// Return that we made no changes
		return false;
	}
}