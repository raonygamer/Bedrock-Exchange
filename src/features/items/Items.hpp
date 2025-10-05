#pragma once
#include "amethyst/runtime/AmethystContext.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"
#include "mc/src/common/world/item/registry/ItemRegistry.hpp"
#include "mc/src/common/world/item/Item.hpp"

class AlchemicalBagItem;
class ChargeableItem;
class Items {
public:
	static ItemRegistry* ItemRegistry;
	static std::unordered_map<std::string, WeakPtr<AlchemicalBagItem>> AlchemicalBags;
	static WeakPtr<ChargeableItem> PhilosophersStone;

	static void RegisterAllItems(RegisterItemsEvent& event, AmethystContext& ctx);
};