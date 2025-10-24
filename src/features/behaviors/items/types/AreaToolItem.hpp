#pragma once
#include "features/behaviors/items/ItemBehavior.hpp"

#include <unordered_set>

#include "mc/src/common/world/level/block/Block.hpp"
#include "mc/src/common/world/level/BlockPos.hpp"

class AreaToolItem;
struct BlockAreaResult {
	BlockPos mPosition;
	const Block& mBlock;
	const AreaToolItem& mSourceBehavior;

	explicit BlockAreaResult(const BlockPos& pos, const Block& block, const AreaToolItem& sourceBehavior) :
		mPosition(pos), 
		mBlock(block),
		mSourceBehavior(sourceBehavior)
	{
	}

	bool operator==(const BlockAreaResult& other) const {
		return mPosition == other.mPosition;
	}

	bool operator!=(const BlockAreaResult& other) const {
		return !(*this == other);
	}
};

namespace std {
	template<>
	struct hash<BlockAreaResult> {
		size_t operator()(const BlockAreaResult& bar) const {
			size_t h1 = std::hash<BlockPos>()(bar.mPosition);
			return h1;
		}
	};
}

class Actor;
class ItemStackBase;
class BaseActorRenderContext;
class AreaToolItem :
	public ItemBehavior 
{
public:
	explicit AreaToolItem(BehaviorStorage* owner);
	
	virtual bool isSinglePerStorage() const override;
	virtual std::vector<BlockAreaResult> getBlocksInArea(const ItemStackBase& stack, BlockSource& region, Actor& actor, const BlockPos& center) const = 0;
	virtual bool highlightBlock(const ItemStackBase& stack, BaseActorRenderContext& context, BlockSource& region, Actor& actor, const BlockPos& target) const = 0;

	static std::unordered_set<BlockAreaResult> mergeBlocksInAreas(const std::vector<AreaToolItem*>& areaBehaviors, const ItemStackBase& stack, BlockSource& region, Actor& actor, const BlockPos& center);
};