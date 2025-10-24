#include "features/behaviors/items/types/AreaToolItem.hpp"

AreaToolItem::AreaToolItem(BehaviorStorage* owner) :
	ItemBehavior(owner)
{
}

bool AreaToolItem::isSinglePerStorage() const {
    return false;
}

std::unordered_set<BlockAreaResult> AreaToolItem::mergeBlocksInAreas(const std::vector<AreaToolItem*>& areaBehaviors, const ItemStackBase& stack, BlockSource& region, Actor& actor, const BlockPos& center) {
	std::unordered_set<BlockAreaResult> mergedResults;
	for (const auto& behavior : areaBehaviors) {
		auto results = behavior->getBlocksInArea(stack, region, actor, center);
		mergedResults.insert(results.begin(), results.end());
	}
	return mergedResults;
}
