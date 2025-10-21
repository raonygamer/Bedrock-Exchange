#include "features/networking/UpdateItemChargePacketHandler.hpp"

#include "mc/src/common/network/packet/Packet.hpp"
#include "mc/src/common/network/PacketHandlerDispatcherInstance.hpp"
#include "mc/src/common/network/PacketSender.hpp"
#include "mc/src/common/network/ServerNetworkHandler.hpp"
#include "mc/src/common/server/ServerPlayer.hpp"
#include "mc/src/common/world/actor/player/Inventory.hpp"
#include "mc/src/common/world/containers/models/ContainerModel.hpp"
#include "mc/src/common/world/inventory/FillingContainer.hpp"

#include "features/items/behaviors/ChargeableItemBehavior.hpp"
#include "features/networking/UpdateItemChargePacket.hpp"

void UpdateItemChargePacketHandler::handle(const NetworkIdentifier& networkId, NetEventCallback& netEvent, const Amethyst::CustomPacket& _packet) const {
	ServerNetworkHandler& serverNetwork = (ServerNetworkHandler&)netEvent;
	ServerPlayer* serverPlayer = serverNetwork._getServerPlayer(networkId, SubClientId::PrimaryClient);

	if (serverPlayer == nullptr) {
		Log::Info("UpdateItemChargePacketHandler: ServerPlayer not found?");
		return;
	}

	PlayerInventory& inventory = serverPlayer->getSupplies();
	const ItemStack& mainhandStack = inventory.getSelectedItem();

	if (!mainhandStack || mainhandStack.isNull() || !mainhandStack.getItem()->hasTag("ee2:chargeable_item"))
		return;

	ChargeableItemBehavior* behavior = dynamic_cast<ChargeableItemBehavior*>(mainhandStack.getItem());
	if (!behavior)
		AssertFail("Item has ee2:chargeable_item tag but is not a ChargeableItemBehavior");

	ItemStack mainHandStackCopy = mainhandStack;

	const UpdateItemChargePacket& packet = static_cast<const UpdateItemChargePacket&>(_packet);

	if (packet.mCharge) {
		short currentCharge = behavior->getCharge(mainHandStackCopy);
		short nextCharge = currentCharge + behavior->mChargePerStep;
		Log::Info("[Networked] Charging item '{}' from {} to {}", behavior->mItem->mFullName.getString(), currentCharge, nextCharge);
		behavior->charge(mainHandStackCopy);
	}
	else {
		short currentCharge = behavior->getCharge(mainHandStackCopy);
		short nextCharge = currentCharge - behavior->mChargePerStep;
		Log::Info("[Networked] Uncharging item '{}' from {} to {}", behavior->mItem->mFullName.getString(), currentCharge, nextCharge);
		behavior->uncharge(mainHandStackCopy);
	}

	inventory.mInventory->createTransactionContext([](Container& container, int slot, ItemStack const& from, ItemStack const& to) {
	}, [&inventory, &mainHandStackCopy, &behavior] {
		inventory.setSelectedItem(mainHandStackCopy);
	});
}