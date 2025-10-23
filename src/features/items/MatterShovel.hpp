#pragma once
#include "mc/src/common/world/item/ShovelItem.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"

class Directions;
class MatterShovel :
	public ShovelItem,
	public ItemBehaviorStorage
{
public:
	MatterShovel(const std::string& name, short id, const Item::Tier& tier);

	virtual bool isDamageable() const override;
	virtual void appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const override;
	virtual std::vector<std::pair<BlockPos, const Block*>> getBlocksForMode(const ItemStackBase& stack, BlockSource& region, const BlockPos& center, const Directions& directions) const;
};