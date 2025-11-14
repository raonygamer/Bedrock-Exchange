#include "features/blocks/actor/DarkMatterFurnaceBlockActor.hpp"

DarkMatterFurnaceBlockActor::DarkMatterFurnaceBlockActor(const BlockPos& pos) :
	FurnaceBlockActor(
		BlockActorType::Furnace,
		pos,
		HashedString("furnace"),
		Puv::Legacy::LevelSoundEvent::FurnaceUse,
		ContainerType::FURNACE,
		9,
		*BlockTypeRegistry::getDefaultBlockState("ee2:dark_matter_furnace"),
		*BlockTypeRegistry::getDefaultBlockState("ee2:dark_matter_furnace_lit")
	)
{
}

std::string DarkMatterFurnaceBlockActor::getName() const {
	return "container.ee2:dark_matter_furnace.name";
}
