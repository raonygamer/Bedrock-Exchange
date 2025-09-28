#pragma once
#include "mc/src-client/common/client/gui/screens/controllers/ContainerScreenController.hpp"

class AlchemicalBagScreenController : 
	public ContainerScreenController 
{
public:
	const AlchemicalBagItem* mItem;

	AlchemicalBagScreenController(std::shared_ptr<ClientInstanceScreenModel> model, InteractionModel interaction, const AlchemicalBagItem* item);
	virtual void _registerCoalesceOrder() override;
	virtual void _registerAutoPlaceOrder() override;
	void _registerBindings();
};