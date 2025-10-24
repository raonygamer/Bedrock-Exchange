#include "features/behaviors/items/types/area/MatterAxeAreaToolItem.hpp"
#include "features/behaviors/BehaviorStorage.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"
#include "features/behaviors/items/types/ChargeableItem.hpp"
#include "features/items/MatterAxe.hpp"
#include "features/utility/BlockUtils.hpp"

#include "mc/src/common/world/actor/Actor.hpp"
#include "mc/src/common/world/level/BlockSource.hpp"
#include "mc/src-client/common/client/game/ClientInstance.hpp"
#include "mc/src-client/common/client/renderer/game/LevelRenderer.hpp"
#include "mc/src-client/common/client/renderer/game/LevelRendererPlayer.hpp"
#include "mc/src-client/common/client/renderer/BaseActorRenderContext.hpp"

MatterAxeAreaToolItem::MatterAxeAreaToolItem(BehaviorStorage* owner) :
	AreaToolItem(owner)
{
}

std::string MatterAxeAreaToolItem::getBehaviorName() const {
	return "MatterAxeAreaToolItem";
}

std::type_index MatterAxeAreaToolItem::getTypeIndex() const {
	return typeid(MatterAxeAreaToolItem);
}

std::vector<BlockAreaResult> MatterAxeAreaToolItem::getBlocksInArea(const ItemStackBase& stack, BlockSource& region, Actor& actor, const BlockPos& center) const {
	auto* modeBehavior = mOwner->getFirstBehavior<ModeItem>();
	if (!modeBehavior) {
		return {};
	}

	auto* chargeBehavior = mOwner->getFirstBehavior<ChargeableItem>();
	if (chargeBehavior && chargeBehavior->getCharge(stack) < chargeBehavior->mMaxCharge) {
		return {};
	}

	auto mode = modeBehavior->getModeAsEnum<MatterAxe::Mode>(stack);
	std::vector<BlockAreaResult> blocks;
	switch (mode) {
	case MatterAxe::Mode::Multichop: {
		const Block& targetBlock = region.getBlock(center);
		if (!targetBlock.mLegacyBlock)
			return {};

		if (targetBlock.mLegacyBlock->mTags.end() != std::find(targetBlock.mLegacyBlock->mTags.begin(), targetBlock.mLegacyBlock->mTags.end(), "wood")) {
			auto results = BlockUtils::floodFillBlocks(region, center, targetBlock.mLegacyBlock, 32, 100);
			for (const auto& [pos, block] : results) {
				blocks.emplace_back(pos, *block, *this);
			}
		}
		break;
	}
	}

	return blocks;
}

bool MatterAxeAreaToolItem::highlightBlock(const ItemStackBase& stack, BaseActorRenderContext& context, BlockSource& region, Actor& actor, const BlockPos& target) const {
	auto& levelRendererPlayer = *context.mClientInstance->mLevelRenderer->mLevelRendererPlayer;
	levelRendererPlayer.renderHitSelect(context, region, target, true);
	return true;
}