#pragma once
#include "features/emc/mappers/IEMCMapper.hpp"

#include "mc/src/common/world/item/crafting/Recipes.hpp"

namespace ee2::emc {
	class RecipeEMCMapper : 
		public IEMCMapper 
	{
	protected:
		Recipes& mRecipes;
		std::vector<std::pair<ItemID, uint64_t>> mEMCValues;
		std::vector<ItemID> mIgnoredIngredientEMCs;
	public:
		RecipeEMCMapper(Recipes& recipes);
		void init() override;
		EMCValue getEMC(const ItemID& identifier) override;
		bool canHandleTags() override;
		EMCValue getEMCByTag(const ItemTag& tag) override;
		std::string getMapperName() override;
		void setEMC(const ItemID& identifier, uint64_t emc) override;

		std::optional<uint64_t> resolveIngredientEMC(const RecipeIngredient& ingredient, std::vector<const ItemInstance*> resultItems = {});
	};
} // namespace ee2::emc