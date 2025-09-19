#include "AlchemyBagItem.hpp"
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

void compareVTables(void* obj, void* originalVTable, size_t count, uintptr_t baseAddr) {
	auto vtable = *reinterpret_cast<void***>(obj);
	auto orig = reinterpret_cast<uintptr_t*>(originalVTable);

	auto resolveJmp = [](uintptr_t addr) -> uintptr_t {
		unsigned char* bytes = reinterpret_cast<unsigned char*>(addr);
		if (bytes[0] == 0xFF && bytes[1] == 0x25) {
			int32_t ripOffset = *reinterpret_cast<int32_t*>(bytes + 2);
			return *reinterpret_cast<uintptr_t*>(addr + 6 + ripOffset);
		}
		return addr;
		};

	for (size_t i = 0; i < count; ++i) {
		uintptr_t funcAddr = reinterpret_cast<uintptr_t>(vtable[i]);
		uintptr_t origAddr = orig[i];

		uintptr_t finalFunc = resolveJmp(funcAddr) - baseAddr;
		uintptr_t finalOrig = origAddr - baseAddr;

		printf(
			"vtable[%2zu]: 0x%p -> orig[%2zu]: 0x%p\n",
			i, (void*)finalFunc,
			i, (void*)finalOrig
		);
	}
}

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
	auto& clientInstance = *Amethyst::GetContext().mClientInstance;
	auto& game = *clientInstance.mMinecraftGame;
	auto& minecraft = *Amethyst::GetMinecraft();
	Log::Info("AlchemyBagItem::use called client {}", isClientSide);
	if (isClientSide) {
		auto& factory = *clientInstance.mSceneFactory;
		auto model = SceneCreationUtils::_createModel<ClientInstanceScreenModel>(
			factory,
			game,
			clientInstance,
			factory.mAdvancedGraphicOptions
		);
		auto interactionModel = ContainerScreenController::interactionModelFromUIProfile(model->getUIProfile());
		auto controller = std::make_shared<AlchemicalBagScreenController>(model, interactionModel);
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
