#include "AlchemicalBagManagerController.hpp"
#include "features/containers/managers/models/AlchemicalBagManagerModel.hpp"

AlchemicalBagManagerController::AlchemicalBagManagerController(std::weak_ptr<AlchemicalBagManagerModel> model)
	: ContainerManagerController(model)
{
}
