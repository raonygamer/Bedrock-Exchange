#pragma once
#include "features/behaviors/items/types/AreaToolItem.hpp"

class MatterPickaxeAreaToolItem :
	public AreaToolItem 
{
public:
	explicit MatterPickaxeAreaToolItem(BehaviorStorage* owner);

	virtual std::string getBehaviorName() const override;
	virtual std::type_index getTypeIndex() const override;

	virtual std::vector<BlockAreaResult> getBlocksInArea(const ItemStackBase& stack, BlockSource& region, Actor& actor, const BlockPos& center) const override;
	virtual bool highlightBlock(const ItemStackBase& stack, BaseActorRenderContext& context, BlockSource& region, Actor& actor, const BlockPos& target) const override;
};