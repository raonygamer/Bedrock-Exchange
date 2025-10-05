#include "AlchemicalChestBlock.hpp"

AlchemicalChestBlock::AlchemicalChestBlock(const std::string& nameId, short id) :
	ChestBlock(nameId, id, ChestBlock::ChestType::Default, MaterialType::Metal)
{
	mBlockEntityType = CustomBlockActorType::AlchemicalChest;
	mCreativeGroup = "itemGroup.name.ee2:alchemical_bags";
}
