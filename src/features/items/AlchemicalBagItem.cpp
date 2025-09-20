#include "AlchemicalBagItem.hpp"
#include "amethyst/runtime/ModContext.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/server/ServerPlayer.hpp"
#include "minecraft/src-client/common/client/gui/screens/SceneCreationUtils.hpp"
#include "minecraft/src-client/common/client/gui/screens/UIScene.hpp"
#include "minecraft/src-client/common/client/gui/screens/ScreenController.hpp"
#include "minecraft/src/common/world/containers/ContainerFactory.hpp"
#include "minecraft/src/common/world/inventory/FillingContainer.hpp"
#include <string>
#include <cstdint>
#include <cstdio>

#include "features/screens/AlchemicalBagScreenController.hpp"
#include "features/containers/managers/models/AlchemicalBagManagerModel.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"
#include "minecraft/src/common/Minecraft.hpp"
#include "minecraft/src/common/network/packet/ContainerOpenPacket.hpp"

std::vector<std::string> AlchemicalBagItem::sAlchemicalBagColors = {
	"black",
	"blue",
	"brown",
	"cyan",
	"gray",
	"green",
	"light_blue",
	"light_gray",
	"lime",
	"magenta",
	"orange",
	"pink",
	"purple",
	"red",
	"white",
	"yellow"
};

AlchemicalBagItem::AlchemicalBagItem(const std::string& name, short id, const std::string& color) :
	Item(name, id),
	mBagColor(color)
{
	int32_t index = -1;
	auto it = std::find(sAlchemicalBagColors.begin(), sAlchemicalBagColors.end(), color);
	if (it != sAlchemicalBagColors.end()) {
		index = static_cast<int32_t>(std::distance(sAlchemicalBagColors.begin(), it));
	}
	if (index != -1)
		setIconInfo("ee2:alchemical_bag", index);
	mMaxStackSize = 1;
	mCreativeCategory = CreativeItemCategory::Items;
	mCreativeGroup = "ee2:itemGroup.name.alchemical_bags";
	AmethystContext& ctx = Amethyst::GetContext();
	ctx.mItemOwnerNameRegistry->RegisterItemOwnerName(mFullName.getString(), "Equivalent Exchange 2");
}

ItemStack& AlchemicalBagItem::use(ItemStack& stack, Player& player) const
{
	bool isClientSide = player.isClientSide();
	auto& clientInstance = *Amethyst::GetContext().mClientInstance;
	auto& game = *clientInstance.mMinecraftGame;
	auto& minecraft = *Amethyst::GetMinecraft();
	Log::Info("AlchemicalBagItem::use called client {}", isClientSide);
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
		auto packet = ContainerOpenPacket(id, ContainerType::CONTAINER, BlockPos(0, 0, 0), player.getUniqueID());
		serverPlayer.sendNetworkPacket(packet);
	}
	
	return stack;
}
