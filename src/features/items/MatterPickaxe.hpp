#pragma once
#include "mc/src/common/world/item/PickaxeItem.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"

class Directions;
class MatterPickaxe :
	public PickaxeItem,
	public ItemBehaviorStorage
{
public:
	enum class Mode {
		Standard,
		Tallshot,
		Wideshot,
		Longshot,
		Plate,
		Veinminer
	};

	MatterPickaxe(const std::string& name, short id, const Item::Tier& tier);

	virtual bool isDamageable() const override;
	virtual void appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const override;
	virtual float getDestroySpeed(const ItemStackBase& stack, const Block& block) const override;
};