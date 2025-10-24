#include "features/items/MatterShears.hpp"

MatterShears::MatterShears(const std::string& name, short id, const Item::Tier& tier) :
	ShearsItem(name, id),
	ItemBehaviorStorage(this),
	mTier(tier)
{
}

bool MatterShears::isDamageable() const {
	return false;
}