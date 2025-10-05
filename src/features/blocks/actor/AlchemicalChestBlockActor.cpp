#include "AlchemicalChestBlockActor.hpp"

AlchemicalChestBlockActor::AlchemicalChestBlockActor(const BlockPos& pos) :
	ChestBlockActor(
		CustomBlockActorType::AlchemicalChest,
		"AlchemicalChest",
		CustomBlockActorRendererId::TR_ALCHEMICAL_CHEST,
		pos,
		false
	)
{
}