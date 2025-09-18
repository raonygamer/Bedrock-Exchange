#include "features/screens/AlchemicalBagScreenController.hpp"
#include "minecraft/src/common/world/containers/ContainerFactory.hpp"
#include "features/containers/managers/controllers/AlchemicalBagManagerController.hpp"
#include "features/containers/managers/models/AlchemicalBagManagerModel.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"

#include "Json.hpp"

AlchemicalBagScreenController::AlchemicalBagScreenController(std::shared_ptr<ClientInstanceScreenModel> model, InteractionModel interaction) :
	ContainerScreenController(model, interaction)
{
	auto& player = *model->getPlayer();
	setAssociatedEntityUniqueID(player.getUniqueID());
	auto managerModel = ContainerFactory::clientCreateContainerManagerModel<AlchemicalBagManagerModel>(player, ContainerID::CONTAINER_ID_INVENTORY, player);
	mContainerManagerController = std::make_shared<AlchemicalBagManagerController>(managerModel);
	mContainerManagerController->registerContainerCallbacks();
	mContainerManagerController->postInit(mContainerManagerController);
	_registerCoalesceOrder();
}

void AlchemicalBagScreenController::_registerCoalesceOrder() 
{
	auto vec = std::vector<std::string>{};
	vec.push_back(ContainerCollectionNameMap[ContainerEnumName::HotbarContainer]);
	//vec.push_back(ContainerCollectionNameMap[ContainerEnumName::LevelEntityContainer]);
	vec.push_back(ContainerCollectionNameMap[ContainerEnumName::InventoryContainer]);
	mCoalesceOrderMap[ContainerCollectionNameMap[ContainerEnumName::HotbarContainer]] = vec;

	vec = std::vector<std::string>{};
	vec.push_back(ContainerCollectionNameMap[ContainerEnumName::InventoryContainer]);
	//vec.push_back(ContainerCollectionNameMap[ContainerEnumName::LevelEntityContainer]);
	vec.push_back(ContainerCollectionNameMap[ContainerEnumName::HotbarContainer]);
	mCoalesceOrderMap[ContainerCollectionNameMap[ContainerEnumName::InventoryContainer]] = vec;

	vec = std::vector<std::string>{};
	vec.push_back(ContainerCollectionNameMap[ContainerEnumName::CombinedHotbarAndInventoryContainer]);
	//vec.push_back(ContainerCollectionNameMap[ContainerEnumName::LevelEntityContainer]);
	mCoalesceOrderMap[ContainerCollectionNameMap[ContainerEnumName::CombinedHotbarAndInventoryContainer]] = vec;

	vec = std::vector<std::string>{};
	//vec.push_back(ContainerCollectionNameMap[ContainerEnumName::LevelEntityContainer]);
	vec.push_back(ContainerCollectionNameMap[ContainerEnumName::InventoryContainer]);
	vec.push_back(ContainerCollectionNameMap[ContainerEnumName::HotbarContainer]);
	mCoalesceOrderMap[ContainerCollectionNameMap[ContainerEnumName::CombinedHotbarAndInventoryContainer]] = vec;
}

void AlchemicalBagScreenController::_registerAutoPlaceOrder() 
{
	Log::Info("AlchemicalBagScreenController::_registerAutoPlaceOrder called");
}

#pragma optimize("", off)
void AlchemicalBagScreenController::addStaticScreenVars(Json::Value& root)
{
	ContainerScreenController::addStaticScreenVars(root);

	//std::string titleStr = "Alchemical Bag";
	//Json::Value containerTitle;
	//containerTitle["__string"] = 1;
	//containerTitle["__rawtext"] = 1;
	//containerTitle["value"] = titleStr;
	//root["$container_title"] = Json::nullValue;
	//auto& val = root["$container_title"];
	//Log::Info("Dump: {}", root.toStyledString());
}
#pragma optimize("", on)