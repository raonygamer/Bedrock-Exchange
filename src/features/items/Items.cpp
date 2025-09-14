#include "Items.hpp"
#include "AlchemyBagItem.hpp"

std::unordered_map<std::string, WeakPtr<AlchemyBagItem>> sAlchemyBags;

void Items::RegisterAllItems(RegisterItemsEvent& event)
{
	for (const std::string& color : AlchemyBagItem::sAlchemyBagColors) {
		std::string fullName = std::format("equivalent_exchange:{}_alchemy_bag", color);
		auto ptr = event.itemRegistry.registerItemShared<AlchemyBagItem>(fullName, ++event.itemRegistry.mMaxItemID, color);
		sAlchemyBags[color] = ptr;
	}
}
