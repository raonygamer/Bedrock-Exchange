#include "features/emc/EMCUtils.hpp"
#include "features/emc/EMCRepository.hpp"

#include "mc/src/common/world/level/Level.hpp"
#include "mc/src/common/world/item/registry/TrimMaterialRegistry.hpp"
#include "mc/src/common/world/item/registry/TrimPatternRegistry.hpp"

namespace ee2::emc {
uint64_t EMCUtils::calculateItemEMC(uint64_t baseEMC, const ItemStackBase& stack, Level& level) {
	if (stack.isNull() || baseEMC == 0)
		return 0;

	if (stack.mUserData) {
		bool dmgReducesEMC = true;
		if (stack.mUserData->contains("DamageReducesEMC") && stack.mUserData->getByte("DamageReducesEMC") == 0)
			dmgReducesEMC = false;

		if (stack.getItem()->isDamageable() && dmgReducesEMC) {
			float durabilityPerc = 1.0f - (float)stack.getItem()->getDamageValue(stack.mUserData) / stack.getItem()->getMaxDamage();
			if (durabilityPerc < 0)
				durabilityPerc = 0;
			baseEMC *= durabilityPerc;
		}

		if (stack.mUserData->contains("Items")) {
			const ListTag* items = stack.mUserData->getList("Items");
			if (items) {
				for (int i = 0; i < items->size(); i++) {
					const CompoundTag* itemCompound = items->getCompound(i);
					byte slot = itemCompound->getByte("Slot");
					ItemStackBase stack;
					stack._loadItem(itemCompound);
					uint64_t itemEMC = EMCRepository::getItemEMC(ItemID::fromStack(stack)).emc;
					if (itemEMC > 0) {
						itemEMC = calculateItemEMC(itemEMC, stack, level);
						itemEMC = calculateStackEMC(itemEMC, stack);
						baseEMC += itemEMC;
					}
				}
			}
		}

		if (stack.mUserData->contains("Trim")) {
			const CompoundTag* trimTag = stack.mUserData->getCompound("Trim");
			if (trimTag && trimTag->contains("Material") && trimTag->contains("Pattern")) {
				HashedString materialId = *trimTag->getString("Material");
				HashedString patternId = *trimTag->getString("Pattern");
				if (!materialId.empty() && !patternId.empty() && level.mTrimMaterialRegistry && level.mTrimPatternRegistry) {
					auto& materialRegistry = *level.mTrimMaterialRegistry;
					auto& patternRegistry = *level.mTrimPatternRegistry;
					auto materialIt = std::find_if(materialRegistry.mTrimMaterials.begin(), materialRegistry.mTrimMaterials.end(), [&](const TrimMaterial& mat) {
						return mat.mMaterialId == materialId;
					});
					auto patternIt = patternRegistry.mPatternIdToTemplateItemMap.find(patternId);
					if (materialIt != materialRegistry.mTrimMaterials.end() && patternIt != patternRegistry.mPatternIdToTemplateItemMap.end()) {
						uint64_t materialEMC = EMCRepository::getItemEMC(ItemID::fromItemName(materialIt->mItemName)).emc;
						uint64_t patternEMC = EMCRepository::getItemEMC(ItemID::fromItemName(patternIt->second)).emc;
						if (materialEMC > 0)
							baseEMC += materialEMC;
						if (patternEMC > 0)
							baseEMC += patternEMC;
					}
				}
			}
		}
	}

	return baseEMC;
}

uint64_t EMCUtils::calculateStackEMC(uint64_t emc, const ItemStackBase& stack) {
	if (stack.isNull())
		return 0;
	return emc * stack.mCount;
}
} // namespace ee2::emc