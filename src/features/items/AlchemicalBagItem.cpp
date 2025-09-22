#include "features/items/AlchemicalBagItem.hpp"
#include "features/screens/AlchemicalBagScreenController.hpp"
#include "features/containers/managers/models/AlchemicalBagManagerModel.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"
#include "features/ModGlobals.hpp"

#include "amethyst/runtime/ModContext.hpp"

#include <string>
#include <cstdint>
#include <cstdio>

#include "minecraft/src/common/Minecraft.hpp"
#include "minecraft/src/common/network/packet/ContainerOpenPacket.hpp"
#include "minecraft/src/common/network/packet/InventoryContentPacket.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/server/ServerPlayer.hpp"
#include "minecraft/src-client/common/client/gui/screens/SceneCreationUtils.hpp"
#include "minecraft/src-client/common/client/gui/screens/UIScene.hpp"
#include "minecraft/src-client/common/client/gui/screens/ScreenController.hpp"
#include "minecraft/src/common/world/containers/ContainerFactory.hpp"
#include "minecraft/src/common/world/inventory/FillingContainer.hpp"

AlchemicalBagItem::AlchemicalBagItem(const std::string& name, short id, const std::string& color) :
	Item(name, id),
	mBagColor(color)
{
	auto& colors = ModGlobals::AlchemicalBagColors;
	int32_t index = -1;
	auto it = std::find(colors.begin(), colors.end(), color);
	if (it != colors.end()) {
		index = static_cast<int32_t>(std::distance(colors.begin(), it));
	}
	if (index != -1)
		setIconInfo("ee2:alchemical_bag", index);
	mMaxStackSize = 1;
	mCreativeCategory = CreativeItemCategory::Items;
	mCreativeGroup = "ee2:itemGroup.name.alchemical_bags";
}

ItemStack& AlchemicalBagItem::use(ItemStack& stack, Player& player) const
{
	bool isClientSide = player.isClientSide();
	auto& clientInstance = *Amethyst::GetContext().mClientInstance;
	auto& game = *clientInstance.mMinecraftGame;
	auto& minecraft = *Amethyst::GetMinecraft();
	if (isClientSide) {
		auto& factory = *clientInstance.mSceneFactory;
		auto model = SceneCreationUtils::_createModel<ClientInstanceScreenModel>(
			factory,
			game,
			clientInstance,
			factory.mAdvancedGraphicOptions
		);
		auto interactionModel = ContainerScreenController::interactionModelFromUIProfile(model->getUIProfile());
		auto controller = std::make_shared<AlchemicalBagScreenController>(model, interactionModel, this);
		auto scene = factory.createUIScene(game, clientInstance, "alchemical_chest.alchemical_chest_screen", controller);
		auto screen = factory._createScreen(scene);
		factory.getCurrentSceneStack()->pushScreen(screen, false);
	}
	else {
		ServerPlayer& serverPlayer = static_cast<ServerPlayer&>(player);
		auto nextContainerId = serverPlayer.mContainerCounter + 1;
		if (nextContainerId >= (char)ContainerID::CONTAINER_ID_LAST)
			nextContainerId = (char)ContainerID::CONTAINER_ID_FIRST;
		serverPlayer.mContainerCounter = nextContainerId;
		auto id = ContainerID(nextContainerId);
		auto containerManager = std::make_shared<AlchemicalBagManagerModel>(id, player);
		containerManager->postInit();
		serverPlayer.setContainerManagerModel(containerManager);
		ContainerOpenPacket packet(containerManager->getContainerId(), containerManager->getContainerType(), BlockPos(0, 0, 0), player.getUniqueID());
		InventoryContentPacket invPacket = InventoryContentPacket::fromPlayerInventoryId(containerManager->getContainerId(), player);
		serverPlayer.sendNetworkPacket(packet);
		serverPlayer.sendNetworkPacket(invPacket);
	}
	
	return stack;
}
