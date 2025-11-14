#include "features/behaviors/items/types/area/MatterHoeAreaToolItem.hpp"
#include "features/behaviors/BehaviorStorage.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"
#include "features/behaviors/items/types/ChargeableItem.hpp"
#include "features/items/MatterHoe.hpp"
#include "features/utility/BlockUtils.hpp"

#include "mc/src/common/world/actor/Actor.hpp"
#include "mc/src/common/world/level/BlockSource.hpp"
#include "mc/src-client/common/client/game/ClientInstance.hpp"
#include "mc/src-client/common/client/renderer/game/LevelRenderer.hpp"
#include "mc/src-client/common/client/renderer/game/LevelRendererPlayer.hpp"
#include "mc/src-client/common/client/renderer/BaseActorRenderContext.hpp"

MatterHoeAreaToolItem::MatterHoeAreaToolItem(BehaviorStorage* owner) :
	AreaToolItem(owner)
{
}

std::string MatterHoeAreaToolItem::getBehaviorName() const {
	return "MatterHoeAreaToolItem";
}

std::type_index MatterHoeAreaToolItem::getTypeIndex() const {
	return typeid(MatterHoeAreaToolItem);
}

std::vector<BlockAreaResult> MatterHoeAreaToolItem::getBlocksInArea(const ItemStackBase& stack, BlockSource& region, Actor& actor, const BlockPos& center) const {
	auto* modeBehavior = mOwner->getFirstBehavior<ModeItem>();
	if (!modeBehavior) {
		return {};
	}

	auto* chargeBehavior = mOwner->getFirstBehavior<ChargeableItem>();
	if (chargeBehavior && chargeBehavior->getCharge(stack) < chargeBehavior->mMaxCharge) {
		return {};
	}

	if (!stack.mItem)
		return {};

	auto mode = modeBehavior->getModeAsEnum<MatterHoe::Mode>(stack);
	auto& centerBlock = region.getBlock(center);
	std::vector<BlockAreaResult> blocks;
	switch (mode) {
	case MatterHoe::Mode::MultiHarvest: {
		const int radius = 8;
		const int maxBlocks = radius * radius * radius + 1;
		if (!stack.canDestroyOptimally(centerBlock) || stack.mItem->getDestroySpeed(stack, centerBlock) <= 1.0f)
			break;
		auto results = BlockUtils::floodFillBlocks(region, center, region.getBlock(center).mLegacyBlock, radius, maxBlocks);
		for (const auto& [pos, block] : results) {
			blocks.emplace_back(pos, *block, *this);
		}
		break;
	}
	case MatterHoe::Mode::Plane5x5: {
		const int planeSize = 2;
		for (int f = -planeSize; f <= planeSize; f++) {
			for (int r = -planeSize; r <= planeSize; r++) {
				BlockPos newPos = center;
				Vec3 offset = Vec3(0, 0, 1) * f + Vec3(1, 0, 0) * r;
				newPos.x += (int)std::floor(offset.x);
				newPos.y += (int)std::floor(offset.y);
				newPos.z += (int)std::floor(offset.z);
				const Block& block = region.getBlock(newPos);
				if (stack.canDestroyOptimally(block)) {
					blocks.emplace_back(newPos, block, *this);
				}
			}
		}
		break;
	}
	}

	return blocks;
}

bool MatterHoeAreaToolItem::highlightBlock(const ItemStackBase& stack, BaseActorRenderContext& context, BlockSource& region, Actor& actor, const BlockPos& target) const {
#ifdef CLIENT
	auto& levelRendererPlayer = *context.mClientInstance.mLevelRenderer->mLevelRendererPlayer;
	levelRendererPlayer.renderHitSelect(context, region, target, true);
#endif
	return true;
}