#include "features/networking/SwitchItemModePacketHandler.hpp"

#include "mc/src/common/network/packet/Packet.hpp"
#include "mc/src/common/network/PacketHandlerDispatcherInstance.hpp"
#include "mc/src/common/network/PacketSender.hpp"
#include "mc/src/common/network/ServerNetworkHandler.hpp"
#include "mc/src/common/server/ServerPlayer.hpp"
#include "mc/src/common/world/actor/player/Inventory.hpp"
#include "mc/src/common/world/containers/models/ContainerModel.hpp"
#include "mc/src/common/world/inventory/FillingContainer.hpp"

#include "features/items/behaviors/ModeItemBehavior.hpp"
#include "features/networking/SwitchItemModePacket.hpp"

void SwitchItemModePacketHandler::handle(const NetworkIdentifier& networkId, NetEventCallback& netEvent, const Amethyst::CustomPacket& _packet) const {
	ServerNetworkHandler& serverNetwork = (ServerNetworkHandler&)netEvent;
	ServerPlayer* serverPlayer = serverNetwork._getServerPlayer(networkId, SubClientId::PrimaryClient);

	if (serverPlayer == nullptr) {
		Log::Info("UpdateItemChargePacketHandler: ServerPlayer not found?");
		return;
	}

	PlayerInventory& inventory = serverPlayer->getSupplies();
	const ItemStack& mainhandStack = inventory.getSelectedItem();

	if (!mainhandStack || mainhandStack.isNull() || !mainhandStack.getItem()->hasTag("ee2:switch_mode_item"))
		return;

	ModeItemBehavior* behavior = dynamic_cast<ModeItemBehavior*>(mainhandStack.getItem());
	if (!behavior)
		AssertFail("Item has ee2:switch_mode_item tag but is not a ModeItemBehavior");

	ItemStack mainHandStackCopy = mainhandStack;

	const SwitchItemModePacket& packet = static_cast<const SwitchItemModePacket&>(_packet);
	
	if (packet.mMode >= behavior->mModes.size()) {
		Log::Info("SwitchItemModePacketHandler: Invalid mode {} for item '{}'", packet.mMode, behavior->mItem->mFullName.getString());
		return;
	}

	Log::Info("[Networked] Switching item '{}' to mode '{}'", behavior->mItem->mFullName.getString(), behavior->getModeName(packet.mMode));
	behavior->setMode(mainHandStackCopy, packet.mMode);

	inventory.mInventory->createTransactionContext([](Container& container, int slot, ItemStack const& from, ItemStack const& to) {
	}, [&inventory, &mainHandStackCopy, &behavior] {
		inventory.setSelectedItem(mainHandStackCopy);
	});
}