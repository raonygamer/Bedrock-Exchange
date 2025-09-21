#pragma once
#include "minecraft/src/common/world/containers/models/ContainerModel.hpp"
#include "minecraft/src/common/world/containers/models/InventoryContainerModel.hpp"
#include "minecraft/src/common/world/containers/models/LevelContainerModel.hpp"
#include "minecraft/src/common/world/inventory/FillingContainer.hpp"
#include "minecraft/src/common/world/item/ItemStack.hpp"
#include "minecraft/src/common/world/actor/player/Player.hpp"
#include "minecraft/src/common/world/containers/SlotData.hpp"

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
};