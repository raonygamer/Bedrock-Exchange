#pragma once
#include <unordered_map>

#include "features/emc/mappers/IEMCMapper.hpp"

namespace ee2::emc {
	class DefaultEMCMapper : 
		public IEMCMapper 
	{
	protected:
		std::unordered_map<HashedString, uint64_t> mEMCValues;
	public:
		void init() override;
		std::optional<EMCResult> getEMC(const Item& item) override;
		std::string getMapperName() override;
		bool hasEMC(const Item& item) override;
	};
}