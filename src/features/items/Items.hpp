#pragma once
#include "amethyst/runtime/AmethystContext.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"
#include "mc/src/common/world/item/registry/ItemRegistry.hpp"
#include "mc/src/common/world/item/Item.hpp"

class Items {
public:
	static void RegisterAllItems(RegisterItemsEvent& event, AmethystContext& ctx);
};