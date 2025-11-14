#include "features/blocks/DarkMatterFurnaceBlock.hpp"
#include "features/blocks/actor/DarkMatterFurnaceBlockActor.hpp"

#include "mc/src/common/Minecraft.hpp"
#include "mc/src/common/world/containers/managers/IContainerManager.hpp"
#include "mc/src/common/network/packet/ContainerOpenPacket.hpp"
#include "mc/src/common/network/packet/InventoryContentPacket.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src/common/server/ServerPlayer.hpp"
#include "mc/src-client/common/client/gui/screens/SceneCreationUtils.hpp"
#include "mc/src-client/common/client/gui/screens/UIScene.hpp"
#include "mc/src-client/common/client/gui/screens/ScreenController.hpp"
#include "mc/src-client/common/client/gui/screens/controllers/FurnaceScreenController.hpp"
#include "mc/src/common/world/containers/ContainerFactory.hpp"
#include "mc/src/common/world/inventory/FillingContainer.hpp"
#include "mc/src/common/world/containers/managers/models/ContainerManagerModel.hpp"
#include "mc/src/common/world/containers/models/LevelContainerModel.hpp"

DarkMatterFurnaceBlock::DarkMatterFurnaceBlock(const std::string& nameId, int id, bool isLit) :
	FurnaceBlock(nameId, id, isLit)
{
	mBlockEntityType = CustomBlockActorType::DarkMatterFurnace;
}

ItemInstance DarkMatterFurnaceBlock::asItemInstance(const Block& block, const BlockActor* actor) const
{
	return ItemInstance("ee2:dark_matter_furnace", 1, 0);
}

std::shared_ptr<BlockActor> DarkMatterFurnaceBlock::newBlockEntity(const BlockPos& pos, const Block& block) const {
	return std::make_shared<DarkMatterFurnaceBlockActor>(pos);
}

bool DarkMatterFurnaceBlock::use(Player& player, const BlockPos& pos, Facing::Name face) const {
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
		auto controller = std::make_shared<FurnaceScreenController>(model, player, pos, ActorUniqueID(), "furnace");
		auto scene = factory.createUIScene(game, clientInstance, "dm_furnace.dm_furnace_screen", controller);
		auto screen = factory._createScreen(scene);
		factory.getCurrentSceneStack()->pushScreen(screen, false);
	}
	else {
		auto& serverPlayer = static_cast<ServerPlayer&>(player);
		serverPlayer.openManagedContainer<FurnaceContainerManagerModel>(pos);
		auto containerManager = serverPlayer.getContainerManagerModel().lock();
		ContainerOpenPacket packet(containerManager->getContainerId(), containerManager->getContainerType(), pos, ActorUniqueID());
		serverPlayer.sendNetworkPacket(packet);
		InventoryContentPacket invPacket = InventoryContentPacket::fromPlayerInventoryId(containerManager->getContainerId(), serverPlayer);
		serverPlayer.sendNetworkPacket(invPacket);
	}
}
