#include "dllmain.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"
#include "minecraft/src/common/world/item/registry/ItemRegistry.hpp"
#include "minecraft/src/common/world/item/Item.hpp"
#include "features/items/Items.hpp"

SafetyHookInline _test1;
void test1(void* self) {
    return;
}

SafetyHookInline _test2;
void test2(void* self) {
    return;
}

ModFunction void Initialize(AmethystContext& ctx) 
{
    Amethyst::InitializeAmethystMod(ctx);
    _test1 = safetyhook::create_inline(0x29F10E0 + GetMinecraftBaseAddress(), &test1);
    _test2 = safetyhook::create_inline(0x29F1240 + GetMinecraftBaseAddress(), &test2);
    ctx.mEventBus->AddListener<RegisterItemsEvent>([&](RegisterItemsEvent& event) { Items::RegisterAllItems(event, ctx); });
    auto& hooks = ctx.mHookManager;
}
