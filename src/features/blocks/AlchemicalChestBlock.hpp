#pragma once
#include "features/ModGlobals.hpp"
#include "features/blocks/actor/AlchemicalChestBlockActor.hpp"

#include "mc/src/common/world/level/block/types/ChestBlock.hpp"
#include "mc/src/common/world/level/block/actor/ChestBlockActor.hpp"

class AlchemicalChestBlock : 
	public ChestBlock
{
public:
	AlchemicalChestBlock(const std::string& nameId, short id);
};