#include "features/items/MatterSword.hpp"
#include "mc/src/common/locale/I18n.hpp"
#include "mc/src/common/world/level/Level.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"

MatterSword::MatterSword(const std::string& identifier, short numId, const Item::Tier& tier, int32_t baseDmg) :
	WeaponItem(identifier, numId, tier),
	ItemBehaviorStorage(this)
{
	mDamage = baseDmg;
}

bool MatterSword::isDamageable() const {
	return false;
}

void MatterSword::appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const {
	WeaponItem::appendFormattedHovertext(stack, level, outText, showCategory);
	
	ChargeableItem* chargeable = getFirstBehavior<ChargeableItem>();
	if (chargeable) {
		short currentCharge = chargeable->getCharge(stack);
		int totalDamage = getAttackDamage() + currentCharge;
		auto* player = level.getPrimaryLocalPlayer();
		if (stack.isEnchanted()) {
			auto enchants = stack.constructItemEnchantsFromUserData();
			for (const auto& enchantInstance : enchants.mItemEnchants[1]) {
				auto& enchant = *Enchant::mEnchants[static_cast<size_t>(enchantInstance.mEnchantType)];
				totalDamage += enchant.getDamageBonus(enchantInstance.mLevel, *player);
			}
		}

		if (currentCharge > 0) {
			outText += std::format("\n§9+{} {}§r", currentCharge, "hover.ee2:charge_damage.text"_i18n);
			outText += std::format("\n§9+{} {}§r", totalDamage, "hover.ee2:total_damage.text"_i18n);
		}
	}
}

float MatterSword::getDestroySpeed(const ItemStackBase& stack, const Block& block) const {
	float speed = WeaponItem::getDestroySpeed(stack, block);
	if (auto* chargeableBehavior = ChargeableItem::tryGet(stack)) {
		short charge = chargeableBehavior->getCharge(stack);
		short maxCharge = chargeableBehavior->mMaxCharge;
		float chargeMultiplier = (static_cast<float>(charge) / static_cast<float>(maxCharge));
		return speed * std::clamp(chargeMultiplier, 0.2f, 1.0f);
	}
	return speed;
}

void MatterSword::onSetCharge(ChargeableItem::SetChargeEvent* e) {
	auto& stack = e->mStack;
	short currentCharge = e->mNewCharge;
	if (!stack.mUserData)
		stack.setUserData(std::make_unique<CompoundTag>());
	if (!stack.mUserData->contains("AdditionalAttackDamage"))
		stack.mUserData->put("AdditionalAttackDamage", IntTag(currentCharge));
	else
		stack.mUserData->getIntTag("AdditionalAttackDamage")->data = currentCharge;
}