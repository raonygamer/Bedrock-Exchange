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

class UpdateItemChargePacket : public Amethyst::CustomPacket {
public:
    bool mCharge = false;

    UpdateItemChargePacket() = default;
	UpdateItemChargePacket(bool charge) : 
        mCharge(charge) {}

    virtual std::string getName() const { 
        return "UpdateItemChargePacket"; 
    };

    virtual void write(BinaryStream& out) {
		out.write(mCharge);
    };

    virtual Bedrock::Result<void, std::error_code> read(ReadOnlyBinaryStream& in) {
		return in.read(&mCharge, 1);
    };
};

class UpdateItemChargePacketHandler : public Amethyst::CustomPacketHandler {
public:
    virtual void handle(const NetworkIdentifier& networkId, NetEventCallback& netEvent, const Amethyst::CustomPacket& _packet) const override {
        ServerNetworkHandler& serverNetwork = (ServerNetworkHandler&)netEvent;
        ServerPlayer* serverPlayer = serverNetwork._getServerPlayer(networkId, SubClientId::PrimaryClient);

        if (serverPlayer == nullptr) {
            Log::Info("UpdateItemChargePacket: ServerPlayer not found?");
            return;
        }

        PlayerInventory& inventory = serverPlayer->getSupplies();
        const ItemStack& mainhandStack = inventory.getSelectedItem();

        if (!mainhandStack || mainhandStack.isNull() || !mainhandStack.getItem()->hasTag("ee2:chargeable_item"))
            return;

        const ChargeableItem& item = static_cast<const ChargeableItem&>(*mainhandStack.getItem());
        ItemStack mainHandStackCopy = mainhandStack;

		const UpdateItemChargePacket& packet = static_cast<const UpdateItemChargePacket&>(_packet);
        
        if (packet.mCharge)
			item.charge(mainHandStackCopy);
        else
            item.uncharge(mainHandStackCopy);

        inventory.mInventory->createTransactionContext([](Container& container, int slot, ItemStack const& from, ItemStack const& to) {
        }, [&inventory , &mainHandStackCopy, &item] {
            inventory.setSelectedItem(mainHandStackCopy);
        });
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

            PlayerInventory& inventory = player.getSupplies();
            const ItemStack& mainhandStack = inventory.getSelectedItem();

            if (!mainhandStack || mainhandStack.isNull() || !mainhandStack.getItem()->hasTag("ee2:chargeable_item"))
                return Amethyst::InputPassthrough::Passthrough;

            const ChargeableItem& item = static_cast<const ChargeableItem&>(*mainhandStack.getItem());
            ItemStack mainHandStackCopy = mainhandStack;

            item.charge(mainHandStackCopy);

            inventory.mInventory->createTransactionContext([&packetSender](Container& container, int slot, ItemStack const& from, ItemStack const& to) {
                Amethyst::GetNetworkManager().SendToServer(packetSender, std::make_unique<UpdateItemChargePacket>(true));
            }, [&inventory, &mainHandStackCopy, &item] {
                inventory.setSelectedItem(mainHandStackCopy);
            });
            return Amethyst::InputPassthrough::Consume;
        };

        auto unchargeButtonHandler = [&](FocusImpact focus, ClientInstance& client) {
            LocalPlayer& player = *client.getLocalPlayer();
            Level& level = static_cast<Level&>(*player.getLevel());
            PacketSender& packetSender = *level.mPacketSender;

            PlayerInventory& inventory = player.getSupplies();
            const ItemStack& mainhandStack = inventory.getSelectedItem();

            if (!mainhandStack || mainhandStack.isNull() || !mainhandStack.getItem()->hasTag("ee2:chargeable_item"))
                return Amethyst::InputPassthrough::Passthrough;

            const ChargeableItem& item = static_cast<const ChargeableItem&>(*mainhandStack.getItem());
            ItemStack mainHandStackCopy = mainhandStack;

            item.uncharge(mainHandStackCopy);

            inventory.mInventory->createTransactionContext([&packetSender](Container& container, int slot, ItemStack const& from, ItemStack const& to) {
                Amethyst::GetNetworkManager().SendToServer(packetSender, std::make_unique<UpdateItemChargePacket>(false));
            }, [&inventory, &mainHandStackCopy, &item] {
                inventory.setSelectedItem(mainHandStackCopy);
            });
            return Amethyst::InputPassthrough::Consume;
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
    ctx.mNetworkManager->RegisterPacketType<UpdateItemChargePacket>(std::make_unique<UpdateItemChargePacketHandler>());

    Log::Info("{:X}", (uint32_t)StringToNameId("#selected_item_durability_visible"));
    Log::Info("{:X}", (uint32_t)StringToNameId("#item_durability_visible"));
    Log::Info("{:X}", (uint32_t)StringToNameId("#item_durability_total_amount"));
    Log::Info("{:X}", (uint32_t)StringToNameId("#item_durability_current_amount"));
}
