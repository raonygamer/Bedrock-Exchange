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

#include "features/items/behaviors/ModeItemBehavior.hpp"
#include "features/networking/SwitchItemModePacket.hpp"

void ModeItemInputs::Initialize(RegisterInputsEvent& event, AmethystContext& ctx) {
	event.inputManager.RegisterNewInput("ee2.switch_item_mode", { 'G' }, true).addButtonDownHandler(&SwitchToNextMode);
}

std::tuple<const ItemStack*, ModeItemBehavior*, PlayerInventory*> ModeItemInputs::GetMainHandModeItem(ClientInstance& client) {
	LocalPlayer& player = *client.getLocalPlayer();
	PlayerInventory& inventory = player.getSupplies();
	const ItemStack& mainhandStack = inventory.getSelectedItem();

	if (!mainhandStack || mainhandStack.isNull() || !mainhandStack.getItem()->hasTag("ee2:switch_mode_item"))
		return { nullptr, nullptr, nullptr };

	ModeItemBehavior* behavior = dynamic_cast<ModeItemBehavior*>(mainhandStack.getItem());
	if (!behavior)
		AssertFail("Item has ee2:switch_mode_item tag but is not a ModeItemBehavior");
	return { &mainhandStack, behavior, &inventory };
}

Amethyst::InputPassthrough ModeItemInputs::SwitchToNextMode(FocusImpact focus, ClientInstance& client) {
	const auto [stack, behavior, inventory] = GetMainHandModeItem(client);
	if (!stack || !behavior || !inventory)
		return Amethyst::InputPassthrough::Passthrough;

	Level& level = *client.getLocalPlayer()->getLevel()->asLevel();
	PacketSender& packetSender = *level.mPacketSender;

	ItemStack mainHandStackCopy = *stack;
	behavior->nextMode(mainHandStackCopy);

	inventory->mInventory->createTransactionContext([&packetSender, &behavior](Container& container, int slot, ItemStack const& from, ItemStack const& to) {
		Amethyst::GetNetworkManager().SendToServer(packetSender, std::make_unique<SwitchItemModePacket>(behavior->getMode(to)));
	}, [&inventory, &mainHandStackCopy, &behavior] {
		inventory->setSelectedItem(mainHandStackCopy);
	});
	return Amethyst::InputPassthrough::Consume;
}