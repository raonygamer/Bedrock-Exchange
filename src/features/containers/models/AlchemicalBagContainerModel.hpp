#pragma once
#include "mc/src/common/world/containers/models/ContainerModel.hpp"
#include "mc/src/common/world/containers/models/InventoryContainerModel.hpp"
#include "mc/src/common/world/containers/models/LevelContainerModel.hpp"
#include "mc/src/common/world/inventory/FillingContainer.hpp"
#include "mc/src/common/world/item/ItemStack.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src/common/world/containers/SlotData.hpp"
#include "mc/src/common/world/inventory/network/SparseContainer.hpp"

#include "features/components/AlchemicalBagContainerComponent.hpp"

extern ActorContainerType AlchemicalBagContainerType;
class LevelContainerManagerModel;

class AlchemicalBagContainerModel :
	public LevelContainerModel
{
public:
	AlchemicalBagContainerModel(ContainerEnumName name, int size, Player& player);
	virtual Container* _getContainer() override;
	virtual int _getContainerOffset() override;
	virtual ContainerWeakRef getContainerWeakRef() override;
	virtual void _onItemChanged(int slot, const ItemStack& a, const ItemStack& b) override {
		//Log::Info("Item changed in alchemical bag: slot {}, old: {}, new: {}", slot, a.toString(), b.toString());
		LevelContainerModel::_onItemChanged(slot, a, b);
		if (mIsClientSide)
			_getContainer()->setItem(slot + _getContainerOffset(), b);
	}
};