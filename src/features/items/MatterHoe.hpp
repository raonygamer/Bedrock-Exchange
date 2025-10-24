#pragma once
#include "features/behaviors/items/ItemBehaviorStorage.hpp"

#include "mc/src/common/world/item/HoeItem.hpp"

class MatterHoe : 
	public HoeItem,
	public ItemBehaviorStorage
{
public:
	enum class Mode {
		Standard,
		MultiHarvest,
		Plane5x5
	};

	MatterHoe(const std::string& name, short id, const Item::Tier& tier);

	virtual bool isDamageable() const override;
	virtual void appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const override;
};