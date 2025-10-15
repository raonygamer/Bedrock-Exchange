#include "DarkMatterFurnaceBlock.hpp"

DarkMatterFurnaceBlock::DarkMatterFurnaceBlock(const std::string& nameId, int id, bool isLit) :
	FurnaceBlock(nameId, id, isLit)
{
	mBlockEntityType = BlockActorType::Furnace;
}

ItemInstance DarkMatterFurnaceBlock::asItemInstance(const Block& block, const BlockActor* actor) const
{
	return ItemInstance("ee2:dark_matter_furnace", 1, 0);
}
