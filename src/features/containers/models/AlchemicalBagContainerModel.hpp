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

class LevelContainerManagerModel;
class AlchemicalBagContainerModel :
	public LevelContainerModel
{
public:
	int mBagIndex;

	AlchemicalBagContainerModel(ContainerEnumName name, int size, Player& player, int bagIndex);
	virtual Container* _getContainer() override;
	virtual int _getContainerOffset() override;
	virtual ContainerWeakRef getContainerWeakRef() override;
};