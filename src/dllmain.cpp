#include "dllmain.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"
#include "amethyst/runtime/events/GameEvents.hpp"
#include "amethyst/runtime/events/InputEvents.hpp"

#include "features/hooks/Hooks.hpp"
#include "features/items/Items.hpp"
#include "features/blocks/Blocks.hpp"
#include "features/containers/AlchemicalBagContainer.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"
#include "features/emc/EMCRepository.hpp"
#include "features/ModGlobals.hpp"
#include "features/items/Tiers.hpp"
#include "features/networking/UpdateItemChargePacket.hpp"
#include "features/networking/UpdateItemChargePacketHandler.hpp"
#include "features/networking/SwitchItemModePacket.hpp"
#include "features/networking/SwitchItemModePacketHandler.hpp"
#include "features/inputs/ChargeableItemInputs.hpp"
#include "features/inputs/ModeItemInputs.hpp"

#include "mc/src/common/world/item/registry/ItemRegistry.hpp"
#include "mc/src/common/world/item/Item.hpp"
#include "mc/src-client/common/client/gui/screens/controllers/MinecraftScreenController.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src/common/world/item/registry/ItemRegistryManager.hpp"
#include "mc/src/common/world/level/block/types/ChestBlock.hpp"
#include "mc/src/common/world/entity/components/ActorEquipmentComponent.hpp"
#include "mc/src/common/world/SimpleContainer.hpp"

ModFunction void Initialize(AmethystContext& ctx, const Amethyst::Mod& mod) 
{
    Amethyst::InitializeAmethystMod(ctx, mod);
	ModGlobals::InitializeEnums();
	ee2::Tiers::Initialize();
	ctx.mNetworkManager->RegisterPacketType<UpdateItemChargePacket>(std::make_unique<UpdateItemChargePacketHandler>());
	ctx.mNetworkManager->RegisterPacketType<SwitchItemModePacket>(std::make_unique<SwitchItemModePacketHandler>());

	// Listen to RegisterItemsEvent to add new items
    ctx.mEventBus->AddListener<RegisterItemsEvent>([&](RegisterItemsEvent& event) {
        Items::RegisterAllItems(event, ctx);
    });

	// Listen to RegisterBlocksEvent to add new blocks
    ctx.mEventBus->AddListener<RegisterBlocksEvent>([&](RegisterBlocksEvent& event) {
		Blocks::RegisterAllBlocks(event, ctx);
	});

#if CLIENT
	// Listen to InitBlockGraphicsEvent to initialize block graphics
    ctx.mEventBus->AddListener<InitBlockGraphicsEvent>([&](InitBlockGraphicsEvent& event) {
        Blocks::InitAllBlockGraphics(event, ctx);
    });

	// Listen to InitBlockEntityRenderersEvent to initialize block entity renderers
    ctx.mEventBus->AddListener<InitBlockEntityRenderersEvent>([&](InitBlockEntityRenderersEvent& event) {
        Blocks::InitAllBlockEntityRenderers(event, ctx);
	});
    
	// Listen to RegisterInputsEvent to add new inputs
    ctx.mEventBus->AddListener<RegisterInputsEvent>([&](RegisterInputsEvent& event) {
		ChargeableItemInputs::Initialize(event, ctx);
		ModeItemInputs::Initialize(event, ctx);
    });
#endif

    // Register game hooks
	CreateAllHooks(ctx);
}
