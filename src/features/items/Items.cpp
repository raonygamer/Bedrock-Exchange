#include "Items.hpp"
#include "AlchemyBagItem.hpp"
#include "minecraft/src-client/common/client/game/ClientInstance.hpp"
#include "minecraft/src-client/common/client/gui/screens/models/MinecraftScreenModel.hpp"
#include "minecraft/src-client/common/client/gui/screens/SceneCreationUtils.hpp"
#include "minecraft/src-deps/core/utility/ServiceLocator.hpp"

std::unordered_map<std::string, WeakPtr<AlchemyBagItem>> sAlchemyBags;

void Items::RegisterAllItems(RegisterItemsEvent& event, AmethystContext& ctx)
{
	for (const std::string& color : AlchemyBagItem::sAlchemyBagColors) {
		std::string fullName = std::format("equivalent_exchange:{}_alchemy_bag", color);
		auto ptr = event.itemRegistry.registerItemShared<AlchemyBagItem>(fullName, ++event.itemRegistry.mMaxItemID, color);
		sAlchemyBags[color] = ptr;
	}
}
