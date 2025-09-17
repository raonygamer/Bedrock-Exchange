#pragma once
#include "minecraft/src/common/world/containers/managers/models/ContainerManagerModel.hpp"

class AlchemicalBagManagerModel :
	public ContainerManagerModel
{
public:
	AlchemicalBagManagerModel(ContainerID id, Player& player);
    virtual std::vector<ItemStack> getItemCopies();
    virtual void setSlot(int slot, const ItemStack& stack, bool);
    virtual const ItemStack& getSlot(int slot);
    virtual void setData(int, int);
    virtual void broadcastChanges();
    virtual ContainerScreenContext _postInit();
};