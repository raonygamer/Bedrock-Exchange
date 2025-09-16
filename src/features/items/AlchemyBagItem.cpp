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

class TestContainerScreenController :
	public ContainerScreenController
{
public:
	TestContainerScreenController(std::shared_ptr<ClientInstanceScreenModel> model, InteractionModel interaction) :
		ContainerScreenController(model, interaction)
	{
		
	}

	virtual void addStaticScreenVars(Json::Value& value) override {
		value["$container_title"] = std::string("Alchemic Bag");
		ScreenController::addStaticScreenVars(value);
	}

	virtual void _registerCoalesceOrder() {

	}

	virtual void _registerAutoPlaceOrder() {

	}

	virtual ui::ViewRequest _onContainerSlotHovered(const std::string& collection, int index) override {
		Log::Info("Hovered slot {} in collection {}", index, collection);
		return ui::ViewRequest::ConsumeEvent;
	}

	virtual ui::ViewRequest _onContainerSlotUnhovered(const std::string& collection, int index) override {
		Log::Info("Unhovered slot {} in collection {}", index, collection);
		return ui::ViewRequest::ConsumeEvent;
	}

	virtual ui::ViewRequest _onContainerSlotSelected(const std::string& collection, int index) override {
		Log::Info("Selected slot {} in collection {}", index, collection);
		return ui::ViewRequest::ConsumeEvent;
	}

	virtual ui::ViewRequest _onContainerSlotPressed(const std::string& collection, int index) override {
		Log::Info("Pressed slot {} in collection {}", index, collection);
		return ui::ViewRequest::ConsumeEvent;
	}

	virtual std::string _getButtonADescription() override {
		return "Test Button A";
	}

	virtual std::string _getButtonXDescription() override {
		return "Test Button X";
	}

	virtual std::string _getButtonYDescription() override {
		return "Test Button Y";
	}

	virtual bool _shouldSwap(const std::string& collectionLhs, int indexLhs, const std::string& collectionRhs, int indexRhs) override {
		Log::Info("Should swap slot {} in collection {} with slot {} in collection {}", indexLhs, collectionLhs, indexRhs, collectionRhs);
		return false;
	}
};

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
