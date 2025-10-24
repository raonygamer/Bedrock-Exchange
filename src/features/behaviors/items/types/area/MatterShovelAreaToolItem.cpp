#include "features/behaviors/items/types/area/MatterShovelAreaToolItem.hpp"
#include "features/behaviors/BehaviorStorage.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"
#include "features/behaviors/items/types/ChargeableItem.hpp"
#include "features/items/MatterShovel.hpp"
#include "features/utility/BlockUtils.hpp"

#include "mc/src/common/world/actor/Actor.hpp"
#include "mc/src/common/world/level/BlockSource.hpp"
#include "mc/src-client/common/client/game/ClientInstance.hpp"
#include "mc/src-client/common/client/renderer/game/LevelRenderer.hpp"
#include "mc/src-client/common/client/renderer/game/LevelRendererPlayer.hpp"
#include "mc/src-client/common/client/renderer/BaseActorRenderContext.hpp"

MatterShovelAreaToolItem::MatterShovelAreaToolItem(BehaviorStorage* owner) :
	AreaToolItem(owner)
{
}

std::string MatterShovelAreaToolItem::getBehaviorName() const {
	return "MatterShovelAreaToolItem";
}

std::type_index MatterShovelAreaToolItem::getTypeIndex() const {
	return typeid(MatterShovelAreaToolItem);
}

std::vector<BlockAreaResult> MatterShovelAreaToolItem::getBlocksInArea(const ItemStackBase& stack, BlockSource& region, Actor& actor, const BlockPos& center) const {
	auto* modeBehavior = mOwner->getFirstBehavior<ModeItem>();
	if (!modeBehavior) {
		return {};
	}

	auto* chargeBehavior = mOwner->getFirstBehavior<ChargeableItem>();
	if (chargeBehavior && chargeBehavior->getCharge(stack) < chargeBehavior->mMaxCharge) {
		return {};
	}

	auto mode = modeBehavior->getModeAsEnum<MatterShovel::Mode>(stack);
	auto direction = Directions::fromLookVec3(actor.getHeadLookVector(1.0f));
	std::vector<BlockAreaResult> blocks;
	switch (mode) {
	case MatterShovel::Mode::Tallshot:
	case MatterShovel::Mode::Wideshot:
	case MatterShovel::Mode::Longshot: {
		Vec3 offset;
		Vec3 targetDir = mode ==
			MatterShovel::Mode::Tallshot ?
			direction.mUp :
			mode == MatterShovel::Mode::Wideshot ?
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
	case MatterShovel::Mode::Plate: {
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
	}

	return blocks;
}

bool MatterShovelAreaToolItem::highlightBlock(const ItemStackBase& stack, BaseActorRenderContext& context, BlockSource& region, Actor& actor, const BlockPos& target) const {
	auto& levelRendererPlayer = *context.mClientInstance->mLevelRenderer->mLevelRendererPlayer;
	levelRendererPlayer.renderHitSelect(context, region, target, true);
	return true;
}