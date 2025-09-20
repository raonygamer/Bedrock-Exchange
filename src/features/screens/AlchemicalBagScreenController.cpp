#include "features/items/AlchemicalBagItem.hpp"
#include "features/screens/AlchemicalBagScreenController.hpp"
#include "features/containers/managers/models/AlchemicalBagManagerModel.hpp"
#include "features/containers/managers/controllers/AlchemicalBagManagerController.hpp"

#include "minecraft/src/common/world/containers/ContainerFactory.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/locale/I18n.hpp"

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
	std::vector<std::string> coalesceOrder;
	coalesceOrder.reserve(3);
	coalesceOrder.push_back(ContainerCollectionNameMap[ContainerEnumName::HotbarContainer]);
	coalesceOrder.push_back(ContainerCollectionNameMap[ContainerEnumName::LevelEntityContainer]);
	coalesceOrder.push_back(ContainerCollectionNameMap[ContainerEnumName::InventoryContainer]);
	mCoalesceOrderMap[ContainerCollectionNameMap[ContainerEnumName::HotbarContainer]] = coalesceOrder;

	coalesceOrder.clear();
	coalesceOrder.push_back(ContainerCollectionNameMap[ContainerEnumName::InventoryContainer]);
	coalesceOrder.push_back(ContainerCollectionNameMap[ContainerEnumName::LevelEntityContainer]);
	coalesceOrder.push_back(ContainerCollectionNameMap[ContainerEnumName::HotbarContainer]);
	mCoalesceOrderMap[ContainerCollectionNameMap[ContainerEnumName::InventoryContainer]] = coalesceOrder;

	coalesceOrder.clear();
	coalesceOrder.push_back(ContainerCollectionNameMap[ContainerEnumName::CombinedHotbarAndInventoryContainer]);
	coalesceOrder.push_back(ContainerCollectionNameMap[ContainerEnumName::LevelEntityContainer]);
	mCoalesceOrderMap[ContainerCollectionNameMap[ContainerEnumName::CombinedHotbarAndInventoryContainer]] = coalesceOrder;

	coalesceOrder.clear();
	coalesceOrder.push_back(ContainerCollectionNameMap[ContainerEnumName::LevelEntityContainer]);
	coalesceOrder.push_back(ContainerCollectionNameMap[ContainerEnumName::InventoryContainer]);
	coalesceOrder.push_back(ContainerCollectionNameMap[ContainerEnumName::HotbarContainer]);
	mCoalesceOrderMap[ContainerCollectionNameMap[ContainerEnumName::LevelEntityContainer]] = coalesceOrder;
}

void AlchemicalBagScreenController::_registerAutoPlaceOrder() 
{
	
}

void AlchemicalBagScreenController::_registerBindings()
{
	std::string itemName = std::format("item.{}.name", mItem->mFullName.getString());
	itemName = getI18n().get(itemName, nullptr);

	bindString({ StringToNameId("#container_name") }, [itemName]() {
		return itemName;
	}, []() { return true; });
}