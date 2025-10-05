#pragma once
#include "features/ModGlobals.hpp"
#include "mc/src/common/world/level/block/actor/ChestBlockActor.hpp"

class AlchemicalChestBlockActor :
	public ChestBlockActor
{
public:
	AlchemicalChestBlockActor(const BlockPos& pos);
};