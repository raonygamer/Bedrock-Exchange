#pragma once
#include "amethyst/runtime/AmethystContext.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"
#include "mc/src/common/world/item/registry/ItemRegistry.hpp"
#include "mc/src/common/world/item/Item.hpp"
#include "features/items/BasicChargeableItem.hpp"

class AlchemicalBagItem;
class BasicChargeableItem;
class Items {
public:
	static ItemRegistry* ItemRegistry;
	static std::unordered_map<std::string, WeakPtr<AlchemicalBagItem>> AlchemicalBags;
	static WeakPtr<BasicChargeableItem> PhilosophersStone;
	static WeakPtr<Item> AlchemicalCoal;
	static WeakPtr<Item> MobiusFuel;
	static WeakPtr<Item> AeternalisFuel;
	static WeakPtr<Item> LowCovalenceDust;
	static WeakPtr<Item> MediumCovalenceDust;
	static WeakPtr<Item> HighCovalenceDust;
	static WeakPtr<Item> DarkMatter;
	static WeakPtr<Item> RedMatter;
	static WeakPtr<Item> DarkMatterSword;
	static WeakPtr<Item> DarkMatterPickaxe;
	static WeakPtr<Item> RedMatterSword;

	static void RegisterAllItems(RegisterItemsEvent& event, AmethystContext& ctx);
};