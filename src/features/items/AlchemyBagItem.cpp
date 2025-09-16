#include "AlchemyBagItem.hpp"
#include "amethyst/runtime/ModContext.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/server/ServerPlayer.hpp"
#include "minecraft/src-client/common/client/gui/screens/SceneCreationUtils.hpp"
#include "minecraft/src-client/common/client/gui/screens/UIScene.hpp"
#include "minecraft/src-client/common/client/gui/screens/ScreenController.hpp"
#include "minecraft/src-client/common/client/gui/screens/controllers/MinecraftScreenController.hpp"
#include "minecraft/src-client/common/client/gui/screens/controllers/ClientInstanceScreenController.hpp"
#include "minecraft/src-client/common/client/gui/screens/controllers/ContainerScreenController.hpp"
#include "minecraft/src-client/common/client/gui/screens/controllers/BlockContainerScreenController.hpp"
#include "minecraft/src-client/common/client/gui/screens/controllers/ChestScreenController.hpp"
#include "minecraft/src-client/common/client/gui/screens/models/ClientInstanceScreenModel.hpp"
#include <string>

std::vector<std::string> AlchemyBagItem::sAlchemyBagColors = {
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

AlchemyBagItem::AlchemyBagItem(const std::string& name, short id, const std::string& color) :
	Item(name, id)
{
	int32_t index = -1;
	auto it = std::find(sAlchemyBagColors.begin(), sAlchemyBagColors.end(), color);
	if (it != sAlchemyBagColors.end()) {
		index = static_cast<int32_t>(std::distance(sAlchemyBagColors.begin(), it));
	}
	if (index != -1)
		setIconInfo("equivalent_exchange:alchemy_bag", index);
	mMaxStackSize = 1;
	mCreativeCategory = CreativeItemCategory::Items;
	mCreativeGroup = "equivalent_exchange.alchemy_bags";
}

ItemStack& AlchemyBagItem::use(ItemStack& stack, Player& player) const
{
	bool isClientSide = player.isClientSide();
	Log::Info("AlchemyBagItem::use called client {}", isClientSide);
	if (isClientSide) {
		auto& clientInstance = *Amethyst::GetContext().mClientInstance;
		auto& factory = *clientInstance.mSceneFactory;
		auto& game = *clientInstance.mMinecraftGame;
		auto model = SceneCreationUtils::_createModel<ClientInstanceScreenModel>(
			factory,
			game,
			clientInstance,
			factory.mAdvancedGraphicOptions
		);
		auto interactionModel = ContainerScreenController::interactionModelFromUIProfile(model->getUIProfile());
		auto controller = std::make_shared<ChestScreenController>(model, player, BlockPos{0, 0, 0}, ActorUniqueID(), BlockActorType::Chest);
		auto scene = factory.createUIScene(game, clientInstance, "alchemical_chest.alchemical_chest_screen", controller);
		auto screen = factory._createScreen(scene);
		factory.getCurrentSceneStack()->pushScreen(screen, false);
	}
	return stack;
}
