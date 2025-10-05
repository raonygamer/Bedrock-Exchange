#pragma once
#include <unordered_map>

#include "features/emc/mappers/IEMCMapper.hpp"

namespace ee2::emc {
	class DefaultEMCMapper : 
		public IEMCMapper 
	{
	protected:
		std::unordered_map<HashedString, uint64_t> mTagEMCValues;
		std::vector<std::pair<ItemID, uint64_t>> mEMCValues;
	public:
		void init() override;
		EMCValue getEMC(const ItemID& identifier) override;
		bool canHandleTags() override;
		EMCValue getEMCByTag(const ItemTag& tag) override;
		std::string getMapperName() override;
		void setEMC(const ItemID& identifier, uint64_t emc) override;
	};
} // namespace ee2::emc