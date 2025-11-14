#pragma once
#include "mc/src/common/world/level/block/FurnaceBlock.hpp"

class DarkMatterFurnaceBlock :
	public FurnaceBlock 
{
public:
	DarkMatterFurnaceBlock(const std::string& nameId, int id, bool isLit);
	virtual ItemInstance asItemInstance(const Block& block, const BlockActor* actor) const;
	virtual std::shared_ptr<BlockActor> newBlockEntity(const BlockPos& pos, const Block& block) const override;
	virtual bool use(Player& player, const BlockPos& pos, Facing::Name face) const override;
};