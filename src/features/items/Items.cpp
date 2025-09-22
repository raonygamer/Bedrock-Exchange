#include "features/items/Items.hpp"
#include "features/blocks/Blocks.hpp"
#include "features/items/AlchemicalBagItem.hpp"
#include "features/ModGlobals.hpp"

#include "minecraft/src-client/common/client/game/ClientInstance.hpp"
#include "minecraft/src-client/common/client/gui/screens/models/MinecraftScreenModel.hpp"
#include "minecraft/src-client/common/client/gui/screens/SceneCreationUtils.hpp"
#include "minecraft/src-deps/core/utility/ServiceLocator.hpp"
#include "minecraft/src/common/world/item/BlockItem.hpp"

std::unordered_map<std::string, WeakPtr<AlchemicalBagItem>> sAlchemicalBags;

void Items::RegisterAllItems(RegisterItemsEvent& event, AmethystContext& ctx)
{
	for (const std::string& color : ModGlobals::AlchemicalBagColors) {
		std::string fullName = std::format("ee2:{}_alchemical_bag", color);
		auto item = event.itemRegistry.registerItemShared<AlchemicalBagItem>(fullName, ++event.itemRegistry.mMaxItemID, color);
		sAlchemicalBags[color] = item;
	}

	for (const auto& [name, block] : Blocks::sBlocks) {
		if (block.isNull()) 
			continue;
		Log::Info("Registering block item '{}'", name);
		auto item = event.itemRegistry.registerItemShared<BlockItem>(name, block->getBlockItemId(), HashedString(name));
	}
}