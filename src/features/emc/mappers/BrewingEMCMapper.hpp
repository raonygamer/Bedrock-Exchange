#pragma once
#include "features/emc/mappers/IEMCMapper.hpp"

#include "mc/src/common/world/item/alchemy/PotionBrewing.hpp"

namespace ee2::emc {
class BrewingEMCMapper :
	public IEMCMapper
{
protected:
	std::vector<std::pair<ItemID, uint64_t>> mEMCValues;
public:
	void init() override;
	EMCValue getEMC(const ItemID& identifier) override;
	bool canHandleTags() override;
	EMCValue getEMCByTag(const ItemTag& tag) override;
	std::string getMapperName() override;
	void setEMC(const ItemID& identifier, uint64_t emc) override;

	uint64_t getLowestEMC(const ItemDescriptor::BaseDescriptor& descriptor);
	bool recalculateContainerMixes(uint16_t aux);
};
} // namespace ee2::emc