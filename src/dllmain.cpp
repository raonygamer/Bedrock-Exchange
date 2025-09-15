#include "dllmain.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"
#include "minecraft/src/common/world/item/registry/ItemRegistry.hpp"
#include "minecraft/src/common/world/item/Item.hpp"
#include "features/items/Items.hpp"

ModFunction void Initialize(AmethystContext& ctx) 
{
    Amethyst::InitializeAmethystMod(ctx);
    ctx.mEventBus->AddListener<RegisterItemsEvent>([&](RegisterItemsEvent& event) { Items::RegisterAllItems(event, ctx); });
}
