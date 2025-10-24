#pragma once
#include "features/behaviors/items/ItemBehaviorStorage.hpp"

#include "mc/src/common/world/item/ShearsItem.hpp"

class MatterShears :
	public ShearsItem,
	public ItemBehaviorStorage
{
public:
	const Item::Tier& mTier;

	MatterShears(const std::string& name, short id, const Item::Tier& tier);

	virtual bool isDamageable() const override;
};