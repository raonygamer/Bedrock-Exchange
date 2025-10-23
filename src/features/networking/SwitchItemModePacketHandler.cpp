#include "features/networking/SwitchItemModePacketHandler.hpp"
#include "features/networking/SwitchItemModePacket.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"

#include "mc/src/common/network/packet/Packet.hpp"
#include "mc/src/common/network/PacketHandlerDispatcherInstance.hpp"
#include "mc/src/common/network/PacketSender.hpp"
#include "mc/src/common/network/ServerNetworkHandler.hpp"
#include "mc/src/common/server/ServerPlayer.hpp"
#include "mc/src/common/world/actor/player/Inventory.hpp"
#include "mc/src/common/world/containers/models/ContainerModel.hpp"
#include "mc/src/common/world/inventory/FillingContainer.hpp"

void SwitchItemModePacketHandler::handle(const NetworkIdentifier& networkId, NetEventCallback& netEvent, const Amethyst::CustomPacket& _packet) const {
	ServerNetworkHandler& serverNetwork = (ServerNetworkHandler&)netEvent;
	ServerPlayer* serverPlayer = serverNetwork._getServerPlayer(networkId, SubClientId::PrimaryClient);

	if (serverPlayer == nullptr) {
		Log::Info("SwitchItemModePacketHandler: ServerPlayer not found?");
		return;
	}

	PlayerInventory& inventory = serverPlayer->getSupplies();
	const ItemStack& stack = inventory.getSelectedItem();
	if (!stack || stack.isNull())
		return;

	auto* storage = ItemBehaviorStorage::tryGetStorage(stack);
	if (!storage)
		return;

	auto* behavior = storage->getFirstBehavior<ModeItem>();
	if (!behavior)
		return;

	ItemStack stackCopy = stack;
	const SwitchItemModePacket& packet = static_cast<const SwitchItemModePacket&>(_packet);
	
	if (packet.mMode >= behavior->mModes.size()) {
		Log::Info("SwitchItemModePacketHandler: Invalid mode {} for item '{}'", packet.mMode, storage->getOwner()->mFullName.getString());
		return;
	}

	Log::Info("[Networked] Switching item '{}' to mode '{}'", storage->getOwner()->mFullName.getString(), behavior->getModeName(packet.mMode));
	behavior->setMode(stackCopy, packet.mMode);

	inventory.mInventory->createTransactionContext([](Container& container, int slot, ItemStack const& from, ItemStack const& to) {
	}, [&inventory, &stackCopy, &behavior] {
		inventory.setSelectedItem(stackCopy);
	});
}