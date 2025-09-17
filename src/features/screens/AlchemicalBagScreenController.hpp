#pragma once
#include "minecraft/src-client/common/client/gui/screens/controllers/ContainerScreenController.hpp"

class AlchemicalBagScreenController : 
	public ContainerScreenController 
{
public:
	AlchemicalBagScreenController(std::shared_ptr<ClientInstanceScreenModel> model, InteractionModel interaction);
	virtual void _registerCoalesceOrder() override;
	virtual void _registerAutoPlaceOrder() override;
};