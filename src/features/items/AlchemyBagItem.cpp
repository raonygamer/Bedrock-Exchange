#include "AlchemyBagItem.hpp"
#include "amethyst/runtime/ModContext.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/server/ServerPlayer.hpp"
#include "minecraft/src-client/common/client/gui/screens/SceneCreationUtils.hpp"
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
		auto& game = *clientInstance.minecraftGame;
		SceneCreationUtils::_createModel<MinecraftScreenModel>(
			factory,
			game,
			clientInstance,
			factory.mAdvancedGraphicOptions,
			"chest.small_chest_screen");
	}
	return stack;
}
