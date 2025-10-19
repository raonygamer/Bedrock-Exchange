#include "features/items/MatterSword.hpp"
#include "mc/src/common/locale/I18n.hpp"

MatterSword::MatterSword(const std::string& identifier, short numId, const Item::Tier& tier, int32_t baseDmg, short maxCharge, short steps, short startingCharge) :
	WeaponItem(identifier, numId, tier),
	ChargeableItemMixin(this, maxCharge, steps, startingCharge),
	mBaseDamage(baseDmg)
{
	mDamage = mBaseDamage;
}

bool MatterSword::isDamageable() const {
	return false;
}

void MatterSword::setCharge(ItemStackBase& stack, short charge) {
	ChargeableItemMixin::setCharge(stack, charge);
	short currentCharge = getCharge(stack);
	if (!stack.mUserData)
		stack.setUserData(std::make_unique<CompoundTag>());
	if (!stack.mUserData->contains("AdditionalAttackDamage"))
		stack.mUserData->put("AdditionalAttackDamage", IntTag(currentCharge));
	else
		stack.mUserData->getIntTag("AdditionalAttackDamage")->data = currentCharge;
}

void MatterSword::appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const {
	WeaponItem::appendFormattedHovertext(stack, level, outText, showCategory);
	short currentCharge = getCharge(stack);
	if (currentCharge > 0) {
		outText += std::format("\n§9+{} {}§r", currentCharge, "text.charge_damage.value"_i18n);
		outText += std::format("\n§9+{} {}§r", getAttackDamage() + currentCharge, "text.total_damage.value"_i18n);
	}
}
