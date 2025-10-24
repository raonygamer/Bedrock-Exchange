#include "features/behaviors/items/types/area/MatterPickaxeAreaToolItem.hpp"
#include "features/behaviors/BehaviorStorage.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"
#include "features/behaviors/items/types/ChargeableItem.hpp"
#include "features/items/MatterPickaxe.hpp"
#include "features/utility/BlockUtils.hpp"

#include "mc/src/common/world/actor/Actor.hpp"
#include "mc/src/common/world/level/BlockSource.hpp"
#include "mc/src-client/common/client/game/ClientInstance.hpp"
#include "mc/src-client/common/client/renderer/game/LevelRenderer.hpp"
#include "mc/src-client/common/client/renderer/game/LevelRendererPlayer.hpp"
#include "mc/src-client/common/client/renderer/BaseActorRenderContext.hpp"

MatterPickaxeAreaToolItem::MatterPickaxeAreaToolItem(BehaviorStorage* owner) :
	AreaToolItem(owner)
{
}

std::string MatterPickaxeAreaToolItem::getBehaviorName() const {
	return "MatterPickaxeAreaToolItem";
}

std::type_index MatterPickaxeAreaToolItem::getTypeIndex() const {
	return typeid(MatterPickaxeAreaToolItem);
}

std::vector<BlockAreaResult> MatterPickaxeAreaToolItem::getBlocksInArea(const ItemStackBase& stack, BlockSource& region, Actor& actor, const BlockPos& center) const {
	auto* modeBehavior = mOwner->getFirstBehavior<ModeItem>();
	if (!modeBehavior) {
		return {};
	}

	auto* chargeBehavior = mOwner->getFirstBehavior<ChargeableItem>();
	if (chargeBehavior && chargeBehavior->getCharge(stack) < chargeBehavior->mMaxCharge) {
		return {};
	}

	auto mode = modeBehavior->getModeAsEnum<MatterPickaxe::Mode>(stack);
	auto direction = Directions::fromLookVec3(actor.getHeadLookVector(1.0f));
	std::vector<BlockAreaResult> blocks;
	switch (mode) {
	case MatterPickaxe::Mode::Tallshot:
	case MatterPickaxe::Mode::Wideshot:
	case MatterPickaxe::Mode::Longshot: {
		Vec3 offset;
		Vec3 targetDir = mode ==
			MatterPickaxe::Mode::Tallshot ?
			direction.mUp :
			mode == MatterPickaxe::Mode::Wideshot ?
			direction.mRight :
			direction.mForward;

		const int lineLength = 1;
		for (int i = -lineLength; i <= lineLength; i++) {
			BlockPos newPos = center;
			offset = targetDir * i;
			newPos.x += (int)std::floor(offset.x);
			newPos.y += (int)std::floor(offset.y);
			newPos.z += (int)std::floor(offset.z);
			const Block& block = region.getBlock(newPos);
			if (stack.canDestroyOptimally(block)) {
				blocks.emplace_back(newPos, block, *this);
			}
		}
		break;
	}
	case MatterPickaxe::Mode::Plate: {
		const int plateSize = 1;
		for (int u = -plateSize; u <= plateSize; u++) {
			for (int r = -plateSize; r <= plateSize; r++) {
				BlockPos newPos = center;
				Vec3 offset = direction.mUp * u + direction.mRight * r;
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
	case MatterPickaxe::Mode::Veinminer: {
		auto* defaultState = region.getBlock(center).mLegacyBlock->mDefaultState;
		if (!defaultState)
			defaultState = &region.getBlock(center);
		if (!defaultState->mLegacyBlock->mNameInfo.mFullName.getString().ends_with("_ore"))
			return {};
		const int radius = 8;
		const int maxBlocks = radius * radius * radius + 1;
		auto results = BlockUtils::floodFillBlocks(region, center, region.getBlock(center).mLegacyBlock, radius, maxBlocks);
		for (const auto& [pos, block] : results) {
			if (stack.canDestroyOptimally(*block)) {
				blocks.emplace_back(pos, *block, *this);
			}
		}
		break;
	}
	}

	return blocks;
}

bool MatterPickaxeAreaToolItem::highlightBlock(const ItemStackBase& stack, BaseActorRenderContext& context, BlockSource& region, Actor& actor, const BlockPos& target) const {
	auto& levelRendererPlayer = *context.mClientInstance->mLevelRenderer->mLevelRendererPlayer;
	levelRendererPlayer.renderHitSelect(context, region, target, true);
	return true;
}