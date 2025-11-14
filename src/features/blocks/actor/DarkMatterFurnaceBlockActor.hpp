#pragma once
#include "features/ModGlobals.hpp"
#include "mc/src/common/world/level/block/actor/FurnaceBlockActor.hpp"
#include "mc/src-deps/shared_types/legacy/LevelSoundEvent.hpp"
#include "mc/src/common/world/level/block/registry/BlockTypeRegistry.hpp"

class DarkMatterFurnaceBlockActor : 
	public FurnaceBlockActor 
{
public:
	DarkMatterFurnaceBlockActor(const BlockPos& pos);
	virtual std::string getName() const override;
};