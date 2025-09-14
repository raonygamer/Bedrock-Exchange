#include "AlchemyBagItem.hpp"
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
}
