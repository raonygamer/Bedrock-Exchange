#include "features/inputs/ModeItemInputs.hpp"
#include "amethyst/runtime/ModContext.hpp"
#include "amethyst/runtime/networking/NetworkManager.hpp"

#include "mc/src/common/network/packet/Packet.hpp"
#include "mc/src/common/network/PacketHandlerDispatcherInstance.hpp"
#include "mc/src/common/network/PacketSender.hpp"
#include "mc/src/common/network/ServerNetworkHandler.hpp"
#include "mc/src/common/server/ServerPlayer.hpp"
#include "mc/src/common/world/actor/player/Inventory.hpp"
#include "mc/src/common/world/containers/models/ContainerModel.hpp"
#include "mc/src/common/world/inventory/FillingContainer.hpp"
#include "mc/src-client/common/client/player/LocalPlayer.hpp"
#include "mc/src/common/world/level/Level.hpp"
#include "mc/src/common/world/actor/ActorDamageSource.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src/common/world/entity/components/ActorUniqueIDComponent.hpp"

#include "features/behaviors/items/types/ModeItem.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"
#include "features/networking/SwitchItemModePacket.hpp"

void ModeItemInputs::Initialize(RegisterInputsEvent& event, AmethystContext& ctx) {
	event.inputManager.RegisterNewInput("ee2.switch_item_mode", { 'G' }, true).addButtonDownHandler(&SwitchToNextMode);
}

Amethyst::InputPassthrough ModeItemInputs::SwitchToNextMode(FocusImpact focus, ClientInstance& client) {
	LocalPlayer& player = *client.getLocalPlayer();
	PlayerInventory& inventory = player.getSupplies();
	const ItemStack& stack = inventory.getSelectedItem();

	auto* storage = ItemBehaviorStorage::tryGetStorage(stack);
	if (!storage)
		return Amethyst::InputPassthrough::Passthrough;

	auto* behavior = storage->getFirstBehavior<ModeItem>();
	if (!behavior)
		return Amethyst::InputPassthrough::Passthrough;

	Level& level = *client.getLocalPlayer()->getLevel()->asLevel();
	PacketSender& packetSender = *level.mPacketSender;

	ItemStack stackCopy = stack;
	behavior->nextMode(stackCopy);

	inventory.mInventory->createTransactionContext([&packetSender, &behavior](Container& container, int slot, ItemStack const& from, ItemStack const& to) {
		Amethyst::GetNetworkManager().SendToServer(packetSender, std::make_unique<SwitchItemModePacket>(behavior->getMode(to)));
	}, [&inventory, &stackCopy, &behavior] {
		inventory.setSelectedItem(stackCopy);
	});
	return Amethyst::InputPassthrough::Consume;
}