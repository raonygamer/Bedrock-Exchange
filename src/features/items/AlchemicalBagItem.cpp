#include "features/items/AlchemicalBagItem.hpp"
#include "features/screens/AlchemicalBagScreenController.hpp"
#include "features/containers/managers/models/AlchemicalBagManagerModel.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"
#include "features/ModGlobals.hpp"

#include "amethyst/runtime/ModContext.hpp"

#include <string>
#include <cstdint>
#include <cstdio>

#include "mc/src/common/Minecraft.hpp"
#include "mc/src/common/network/packet/ContainerOpenPacket.hpp"
#include "mc/src/common/network/packet/InventoryContentPacket.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src/common/server/ServerPlayer.hpp"
#include "mc/src-client/common/client/gui/screens/SceneCreationUtils.hpp"
#include "mc/src-client/common/client/gui/screens/UIScene.hpp"
#include "mc/src-client/common/client/gui/screens/ScreenController.hpp"
#include "mc/src/common/world/containers/ContainerFactory.hpp"
#include "mc/src/common/world/inventory/FillingContainer.hpp"

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
	mCreativeGroup = "itemGroup.name.ee2:alchemical_bags";
}

ItemStack& AlchemicalBagItem::use(ItemStack& stack, Player& player) const
{
	bool isClientSide = player.isClientSide();
	auto& clientInstance = *Amethyst::GetClientCtx().mClientInstance;
	auto& game = *clientInstance.mMinecraftGame;
	auto& minecraft = *Amethyst::GetServerCtx().mMinecraft;
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
		auto id = serverPlayer.nextContainerId();
		auto containerManager = std::make_shared<AlchemicalBagManagerModel>(id, player);
		containerManager->postInit();
		serverPlayer.setContainerManagerModel(containerManager);
		ContainerOpenPacket packet(containerManager->getContainerId(), containerManager->getContainerType(), BlockPos(0, 0, 0), player.getUniqueID());
		serverPlayer.sendNetworkPacket(packet);
		InventoryContentPacket invPacket = InventoryContentPacket::fromPlayerInventoryId(containerManager->getContainerId(), serverPlayer);
		serverPlayer.sendNetworkPacket(invPacket);
	}
	
	return stack;
}
