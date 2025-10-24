#include "features/networking/UpdateItemChargePacketHandler.hpp"

#include "mc/src/common/network/packet/Packet.hpp"
#include "mc/src/common/network/PacketHandlerDispatcherInstance.hpp"
#include "mc/src/common/network/PacketSender.hpp"
#include "mc/src/common/network/ServerNetworkHandler.hpp"
#include "mc/src/common/server/ServerPlayer.hpp"
#include "mc/src/common/world/actor/player/Inventory.hpp"
#include "mc/src/common/world/containers/models/ContainerModel.hpp"
#include "mc/src/common/world/inventory/FillingContainer.hpp"

#include "features/behaviors/items/types/ChargeableItem.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"
#include "features/networking/UpdateItemChargePacket.hpp"

void UpdateItemChargePacketHandler::handle(const NetworkIdentifier& networkId, NetEventCallback& netEvent, const Amethyst::CustomPacket& _packet) const {
	ServerNetworkHandler& serverNetwork = (ServerNetworkHandler&)netEvent;
	ServerPlayer* serverPlayer = serverNetwork._getServerPlayer(networkId, SubClientId::PrimaryClient);

	if (serverPlayer == nullptr) {
		Log::Info("UpdateItemChargePacketHandler: ServerPlayer not found?");
		return;
	}

	PlayerInventory& inventory = serverPlayer->getSupplies();
	const ItemStack& stack = inventory.getSelectedItem();

	if (!stack || stack.isNull()) 
		return;

	auto* storage = ItemBehaviorStorage::tryGetStorage(*stack.getItem());
	if (!storage)
		return;

	auto* behavior = storage->getFirstBehavior<ChargeableItem>();
	if (!behavior)
		return;

	ItemStack stackCopy = stack;

	const UpdateItemChargePacket& packet = static_cast<const UpdateItemChargePacket&>(_packet);

	if (packet.mCharge) {
		short currentCharge = behavior->getCharge(stackCopy);
		short nextCharge = currentCharge + behavior->mChargePerStep;
		behavior->charge(stackCopy);
	}
	else {
		short currentCharge = behavior->getCharge(stackCopy);
		short nextCharge = currentCharge - behavior->mChargePerStep;
		behavior->uncharge(stackCopy);
	}

	inventory.mInventory->createTransactionContext([](Container& container, int slot, ItemStack const& from, ItemStack const& to) {
	}, [&inventory, &stackCopy, &behavior] {
		inventory.setSelectedItem(stackCopy);
	});
}