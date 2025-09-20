#include "Items.hpp"
#include "AlchemicalBagItem.hpp"
#include "minecraft/src-client/common/client/game/ClientInstance.hpp"
#include "minecraft/src-client/common/client/gui/screens/models/MinecraftScreenModel.hpp"
#include "minecraft/src-client/common/client/gui/screens/SceneCreationUtils.hpp"
#include "minecraft/src-deps/core/utility/ServiceLocator.hpp"

std::unordered_map<std::string, WeakPtr<AlchemicalBagItem>> sAlchemicalBags;

void Items::RegisterAllItems(RegisterItemsEvent& event, AmethystContext& ctx)
{
	for (const std::string& color : AlchemicalBagItem::sAlchemicalBagColors) {
		std::string fullName = std::format("ee2:{}_alchemical_bag", color);
		Log::Info("Registering item '{}'", fullName);
		auto ptr = event.itemRegistry.registerItemShared<AlchemicalBagItem>(fullName, ++event.itemRegistry.mMaxItemID, color);
		sAlchemicalBags[color] = ptr;
	}
}