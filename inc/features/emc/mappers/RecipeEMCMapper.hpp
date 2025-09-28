#pragma once
#include "features/emc/mappers/IEMCMapper.hpp"

#include "mc/src/common/world/item/crafting/Recipes.hpp"

namespace ee2::emc {
	class RecipeEMCMapper : 
		public IEMCMapper 
	{
	protected:
		Recipes& mRecipes;
		std::unordered_map<HashedString, uint64_t> mEMCValues;
	public:
		RecipeEMCMapper(Recipes& recipes);
		void init() override;
		std::optional<EMCResult> getEMC(const Item& item) override;
		std::string getMapperName() override;
		bool hasEMC(const Item& item) override;
	};
} // namespace ee2::emc