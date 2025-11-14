#include "AlchemicalChestBlockActor.hpp"

AlchemicalChestBlockActor::AlchemicalChestBlockActor(const BlockPos& pos) :
	ChestBlockActor(
		CustomBlockActorType::AlchemicalChest,
		"AlchemicalChest",
		CustomBlockActorRendererId::AlchemicalChest,
		pos,
		false
	)
{
}