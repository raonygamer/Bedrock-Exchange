#include "features/items/MatterSword.hpp"
#include "mc/src/common/locale/I18n.hpp"
#include "mc/src/common/world/level/Level.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"

MatterSword::MatterSword(const std::string& identifier, short numId, const Item::Tier& tier, int32_t baseDmg, short maxCharge, short steps, short startingCharge) :
	WeaponItem(identifier, numId, tier),
	ChargeableItemBehavior(this, maxCharge, steps, startingCharge)
{
	mDamage = baseDmg;
}

bool MatterSword::isDamageable() const {
	return false;
}

void MatterSword::setCharge(ItemStackBase& stack, short charge) {
	ChargeableItemBehavior::setCharge(stack, charge);
	short currentCharge = getCharge(stack);
	if (!stack.mUserData)
		stack.setUserData(std::make_unique<CompoundTag>());
	if (!stack.mUserData->contains("AdditionalAttackDamage"))
		stack.mUserData->put("AdditionalAttackDamage", IntTag(currentCharge));
	else
		stack.mUserData->getIntTag("AdditionalAttackDamage")->data = currentCharge;
}

#pragma optimize("", off)
void MatterSword::appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const {
	WeaponItem::appendFormattedHovertext(stack, level, outText, showCategory);
	short currentCharge = getCharge(stack);
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
#pragma optimize("", off)