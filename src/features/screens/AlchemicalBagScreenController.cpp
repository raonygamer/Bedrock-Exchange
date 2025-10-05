#include "features/items/AlchemicalBagItem.hpp"
#include "features/screens/AlchemicalBagScreenController.hpp"
#include "features/containers/managers/models/AlchemicalBagManagerModel.hpp"
#include "features/containers/managers/controllers/AlchemicalBagManagerController.hpp"

#include "mc/src/common/world/containers/ContainerFactory.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src/common/locale/I18n.hpp"

#include "Json.hpp"

AlchemicalBagScreenController::AlchemicalBagScreenController(std::shared_ptr<ClientInstanceScreenModel> model, InteractionModel interaction, const AlchemicalBagItem* item) :
	ContainerScreenController(model, interaction),
	mItem(item)
{
	auto& player = *model->getPlayer();
	setAssociatedEntityUniqueID(player.getUniqueID());
	auto managerModel = ContainerFactory::clientCreateContainerManagerModel<AlchemicalBagManagerModel>(player, ContainerID::CONTAINER_ID_NONE, player);
	mContainerManagerController = std::make_shared<AlchemicalBagManagerController>(managerModel);
	mContainerManagerController->registerContainerCallbacks();
	mContainerManagerController->postInit(mContainerManagerController);
	_registerBindings();
	_registerCoalesceOrder();
	_registerAutoPlaceOrder();
}

void AlchemicalBagScreenController::_registerCoalesceOrder() 
{
	std::vector<std::string> order = {};
	order.reserve(3);

	order.clear();
	order.push_back("hotbar_items");
	order.push_back("container_items");
	order.push_back("inventory_items");
	mCoalesceOrderMap["hotbar_items"] = order;

	order.clear();
	order.push_back("inventory_items");
	order.push_back("container_items");
	order.push_back("hotbar_items");
	mCoalesceOrderMap["inventory_items"] = order;

	order.clear();
	order.push_back("combined_hotbar_and_inventory_items");
	order.push_back("container_items");
	mCoalesceOrderMap["combined_hotbar_and_inventory_items"] = order;

	order.clear();
	order.push_back("container_items");
	order.push_back("inventory_items");
	order.push_back("hotbar_items");
	mCoalesceOrderMap["container_items"] = order;
}

void AlchemicalBagScreenController::_registerAutoPlaceOrder() 
{
	std::vector<AutoPlaceItem> order = {};
	order.reserve(3);

	order.clear();
	order.emplace_back("hotbar_items", false);
	order.emplace_back("container_items", false);
	order.emplace_back("inventory_items", false);
	mAutoPlaceOrderMap["hotbar_items"] = order;

	order.clear();
	order.emplace_back("inventory_items", false);
	order.emplace_back("container_items", false);
	order.emplace_back("hotbar_items", false);
	mAutoPlaceOrderMap["inventory_items"] = order;

	order.clear();
	order.emplace_back("combined_hotbar_and_inventory_items", false);
	order.emplace_back("container_items", false);
	mAutoPlaceOrderMap["combined_hotbar_and_inventory_items"] = order;

	order.clear();
	order.emplace_back("container_items", false);
	order.emplace_back("inventory_items", false);
	order.emplace_back("hotbar_items", false);
	mAutoPlaceOrderMap["container_items"] = order;
}

void AlchemicalBagScreenController::_registerBindings()
{
	std::string itemName = std::format("item.{}.name", mItem->mFullName.getString());
	itemName = getI18n().get(itemName, nullptr);

	bindString("#container_name", [itemName]() {
		return itemName;
	}, []() { return true; });
}