#pragma once
#include "mc/src/common/world/item/HatchetItem.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"
#include "features/items/behaviors/ModeItemBehavior.hpp"

class Directions;
class MatterAxe :
	public HatchetItem,
	public ItemBehaviorStorage
{
public:
	MatterAxe(const std::string& name, short id, const Item::Tier& tier);

	virtual bool isDamageable() const override;
	virtual void appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const override;
	virtual std::vector<std::pair<BlockPos, const Block*>> getBlocksForMode(const ItemStackBase& stack, BlockSource& region, const BlockPos& center, const Directions& directions);
};