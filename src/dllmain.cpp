#include "dllmain.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"
#include "amethyst/runtime/events/GameEvents.hpp"
#include "amethyst/runtime/events/InputEvents.hpp"

#include "features/hooks/Hooks.hpp"
#include "features/items/Items.hpp"
#include "features/items/ChargeableItem.hpp"
#include "features/blocks/Blocks.hpp"
#include "features/containers/AlchemicalBagContainer.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"
#include "features/emc/EMCRepository.hpp"
#include "features/ModGlobals.hpp"

#include "mc/src/common/world/item/registry/ItemRegistry.hpp"
#include "mc/src/common/world/item/Item.hpp"
#include "mc/src-client/common/client/gui/screens/controllers/MinecraftScreenController.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src/common/world/actor/player/Inventory.hpp"
#include "mc/src/common/world/containers/models/ContainerModel.hpp"
#include "mc/src/common/world/inventory/FillingContainer.hpp"
#include "mc/src/common/world/item/registry/ItemRegistryManager.hpp"
#include "mc/src/common/world/level/block/types/ChestBlock.hpp"
#include <mc/src-client/common/client/player/LocalPlayer.hpp>
#include <mc/src/common/network/ServerNetworkHandler.hpp>
#include <mc/src/common/server/ServerPlayer.hpp>
#include "mc/src/common/world/entity/components/ActorEquipmentComponent.hpp"
#include "mc/src/common/world/SimpleContainer.hpp"
#include <mc/src/common/network/packet/Packet.hpp>
#include <mc/src/common/network/PacketHandlerDispatcherInstance.hpp>
#include <mc/src/common/network/PacketSender.hpp>

#include "mod_menu/Test.hpp"

ActorContainerType AlchemicalBagContainerType;

class ChargeItemPacket : public Amethyst::CustomPacket {
public:
    virtual std::string getName() const { return "ChargeItemPacket"; };
    virtual void write(BinaryStream& out) {};
    virtual Bedrock::Result<void, std::error_code> read(ReadOnlyBinaryStream& in) {
        return Bedrock::Result<void, std::error_code>();
    };
};

class ChargeItemPacketHandler : public Amethyst::CustomPacketHandler {
public:
    virtual void handle(const NetworkIdentifier& networkId, NetEventCallback& netEvent, const Amethyst::CustomPacket& _packet) const override {
        Log::Info("ChargeItemPacketHandler::handle");

        ServerNetworkHandler& serverNetwork = (ServerNetworkHandler&)netEvent;
        ServerPlayer* serverPlayer = serverNetwork._getServerPlayer(networkId, SubClientId::PrimaryClient);

        if (serverPlayer == nullptr) {
            Log::Info("ChargeItemPacketHandler: ServerPlayer not found?");
            return;
        }

        const PlayerInventory& inventory = serverPlayer->getSupplies();
        const ItemStack& mainhandStack = inventory.getSelectedItem();

        if (!mainhandStack || mainhandStack.isNull())
            return;
        auto& handItem = const_cast<ItemStack&>(mainhandStack);
        if (handItem.getItem()->mFullName == "ee2:philosophers_stone") {
            const ChargeableItem& item = static_cast<const ChargeableItem&>(*handItem.getItem());
            inventory.mInventory->createTransactionContext([](Container& container, int slot, ItemStack const& from, ItemStack const& to) {

            }, [&handItem, &item] {
                item.charge(handItem);
            });
        }
    }
};

ModFunction void Initialize(AmethystContext& ctx, const Amethyst::Mod& mod) 
{
    Amethyst::InitializeAmethystMod(ctx, mod);
	ModGlobals::InitializeEnums();

	// Listen to RegisterItemsEvent to add new items
    ctx.mEventBus->AddListener<RegisterItemsEvent>([&](RegisterItemsEvent& event) {
        Items::RegisterAllItems(event, ctx);
    });

	// Listen to RegisterBlocksEvent to add new blocks
    ctx.mEventBus->AddListener<RegisterBlocksEvent>([&](RegisterBlocksEvent& event) {
		Blocks::RegisterAllBlocks(event, ctx);
	});

	// Listen to InitBlockGraphicsEvent to initialize block graphics
    ctx.mEventBus->AddListener<InitBlockGraphicsEvent>([&](InitBlockGraphicsEvent& event) {
        Blocks::InitAllBlockGraphics(event, ctx);
    });

	// Listen to InitBlockEntityRenderersEvent to initialize block entity renderers
    ctx.mEventBus->AddListener<InitBlockEntityRenderersEvent>([&](InitBlockEntityRenderersEvent& event) {
        Blocks::InitAllBlockEntityRenderers(event, ctx);
	});

    
    ctx.mEventBus->AddListener<RegisterInputsEvent>([&](RegisterInputsEvent& e) {
        auto chargeButtonHandler = [&](FocusImpact focus, ClientInstance& client) {
            LocalPlayer& player = *client.getLocalPlayer();
            Level& level = static_cast<Level&>(*player.getLevel());
            PacketSender& packetSender = *level.mPacketSender;
            Amethyst::GetNetworkManager().SendToServer(packetSender, std::make_unique<ChargeItemPacket>());

            const PlayerInventory& inventory = player.getSupplies();
            const ItemStack& mainhandStack = inventory.getSelectedItem();

            if (!mainhandStack || mainhandStack.isNull())
                return Amethyst::InputPassthrough::Passthrough;
            auto& handItem = const_cast<ItemStack&>(mainhandStack);
            if (handItem.getItem()->mFullName == "ee2:philosophers_stone") {
                const ChargeableItem& item = static_cast<const ChargeableItem&>(*handItem.getItem());
                inventory.mInventory->createTransactionContext([](Container& container, int slot, ItemStack const& from, ItemStack const& to) {

                }, [&handItem, &item] {
                    item.charge(handItem);
                });
                return Amethyst::InputPassthrough::Consume;
            }
            return Amethyst::InputPassthrough::Passthrough;
        };

        auto unchargeButtonHandler = [&](FocusImpact focus, ClientInstance& clientInstance) {
            auto* itemStackPtr = clientInstance.getLocalPlayer()->getAllHand()[0];
            if (!itemStackPtr || itemStackPtr->isNull())
                return Amethyst::InputPassthrough::Passthrough;
            auto& handItem = const_cast<ItemStack&>(*itemStackPtr);
            if (handItem.getItem()->mFullName == "ee2:philosophers_stone") {
                const ChargeableItem& item = static_cast<const ChargeableItem&>(*handItem.getItem());
                item.uncharge(handItem);
                return Amethyst::InputPassthrough::Consume;
            }
            return Amethyst::InputPassthrough::Passthrough;
        };

        e.inputManager.RegisterNewInput("ee2.charge_item", { 'V' }, true)
            .addButtonDownHandler(chargeButtonHandler);

        e.inputManager.RegisterNewInput("ee2.uncharge_item", { 'U' }, true)
            .addButtonDownHandler(unchargeButtonHandler);
    });

	// Register new ActorContainerType
    ctx.mEnumAllocator->RegisterEnum<ActorContainerType>(ActorContainerType::PlayerEnderChest);
    AlchemicalBagContainerType = ctx.mEnumAllocator->GetNextValue<ActorContainerType>();

    // Register game hooks
	CreateAllHooks(ctx);
    ctx.mNetworkManager->RegisterPacketType<ChargeItemPacket>(std::make_unique<ChargeItemPacketHandler>());

    Log::Info("{:X}", (uint32_t)StringToNameId("#selected_item_durability_visible"));
    Log::Info("{:X}", (uint32_t)StringToNameId("#item_durability_visible"));
    Log::Info("{:X}", (uint32_t)StringToNameId("#item_durability_total_amount"));
    Log::Info("{:X}", (uint32_t)StringToNameId("#item_durability_current_amount"));
}
