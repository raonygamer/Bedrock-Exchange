#pragma once
#include "mc/src/common/world/level/block/FurnaceBlock.hpp"

class DarkMatterFurnaceBlock :
	public FurnaceBlock 
{
public:
	DarkMatterFurnaceBlock(const std::string& nameId, int id, bool isLit);
	virtual ItemInstance asItemInstance(const Block& block, const BlockActor* actor) const;
};