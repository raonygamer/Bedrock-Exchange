#include "features/inputs/ChargeableItemInputs.hpp"

#include "mc/src/common/network/packet/Packet.hpp"
#include "mc/src/common/network/PacketHandlerDispatcherInstance.hpp"
#include "mc/src/common/network/PacketSender.hpp"
#include "mc/src/common/network/ServerNetworkHandler.hpp"
#include "mc/src/common/server/ServerPlayer.hpp"
#include "mc/src/common/world/actor/player/Inventory.hpp"
#include "mc/src/common/world/containers/models/ContainerModel.hpp"
#include "mc/src/common/world/inventory/FillingContainer.hpp"
#include "mc/src-client/common/client/player/LocalPlayer.hpp"

#include "features/items/behaviors/ChargeableItemBehavior.hpp"
#include "features/networking/UpdateItemChargePacket.hpp"

#include "amethyst/runtime/networking/NetworkManager.hpp"
#include "amethyst/runtime/ModContext.hpp"

void ChargeableItemInputs::Initialize(RegisterInputsEvent& event, AmethystContext& ctx) {
	event.inputManager.RegisterNewInput("ee2.charge_item", { 'V' }, true).addButtonDownHandler(&ChargeItem);
	event.inputManager.RegisterNewInput("ee2.uncharge_item", { 'U' }, true).addButtonDownHandler(&UnchargeItem);
}

std::tuple<const ItemStack*, ChargeableItemBehavior*, PlayerInventory*> ChargeableItemInputs::GetMainHandChargeableItem(ClientInstance& client) {
	LocalPlayer& player = *client.getLocalPlayer();
	PlayerInventory& inventory = player.getSupplies();
	const ItemStack& mainhandStack = inventory.getSelectedItem();

	if (!mainhandStack || mainhandStack.isNull() || !mainhandStack.getItem()->hasTag("ee2:chargeable_item"))
		return { nullptr, nullptr, nullptr };

	ChargeableItemBehavior* behavior = dynamic_cast<ChargeableItemBehavior*>(mainhandStack.getItem());
	if (!behavior)
		AssertFail("Item has ee2:chargeable_item tag but is not a ChargeableItemBehavior");
	return { &mainhandStack, behavior, &inventory };
}

Amethyst::InputPassthrough ChargeableItemInputs::ChargeItem(FocusImpact focus, ClientInstance& client) {
	const auto [stack, behavior, inventory] = GetMainHandChargeableItem(client);
	if (!stack || !behavior || !inventory)
		return Amethyst::InputPassthrough::Passthrough;

	Level& level = static_cast<Level&>(*client.getLocalPlayer()->getLevel());
	PacketSender& packetSender = *level.mPacketSender;

	ItemStack mainHandStackCopy = *stack;
	behavior->charge(mainHandStackCopy);

	inventory->mInventory->createTransactionContext([&packetSender](Container& container, int slot, ItemStack const& from, ItemStack const& to) {
		Amethyst::GetNetworkManager().SendToServer(packetSender, std::make_unique<UpdateItemChargePacket>(true));
	}, [&inventory, &mainHandStackCopy, &behavior] {
		inventory->setSelectedItem(mainHandStackCopy);
	});
	return Amethyst::InputPassthrough::Consume;
}

Amethyst::InputPassthrough ChargeableItemInputs::UnchargeItem(FocusImpact focus, ClientInstance& client) {
	const auto [stack, behavior, inventory] = GetMainHandChargeableItem(client);
	if (!stack || !behavior || !inventory)
		return Amethyst::InputPassthrough::Passthrough;

	Level& level = static_cast<Level&>(*client.getLocalPlayer()->getLevel());
	PacketSender& packetSender = *level.mPacketSender;

	ItemStack mainHandStackCopy = *stack;
	behavior->uncharge(mainHandStackCopy);

	inventory->mInventory->createTransactionContext([&packetSender](Container& container, int slot, ItemStack const& from, ItemStack const& to) {
		Amethyst::GetNetworkManager().SendToServer(packetSender, std::make_unique<UpdateItemChargePacket>(false));
	}, [&inventory, &mainHandStackCopy, &behavior] {
		inventory->setSelectedItem(mainHandStackCopy);
	});
	return Amethyst::InputPassthrough::Consume;
}