#pragma once
#include "amethyst/runtime/ModContext.hpp"

#include "mc/src/common/world/level/block/actor/BlockActorRendererId.hpp"
#include "mc/src/common/world/containers/models/ContainerModel.hpp"

#include <string>
#include <vector>

class CustomBlockActorRendererId {
public:
	inline static BlockActorRendererId AlchemicalChest;

	inline static void Initialize() {
		auto& ctx = Amethyst::GetContext();
		ctx.mEnumAllocator->RegisterEnum<BlockActorRendererId>(BlockActorRendererId::Count);
		AlchemicalChest = ctx.mEnumAllocator->GetNextValue<BlockActorRendererId>();
	}
};

class CustomBlockActorType {
public:
	inline static BlockActorType AlchemicalChest;
	inline static BlockActorType DarkMatterFurnace;

	inline static void Initialize() {
		auto& ctx = Amethyst::GetContext();
		ctx.mEnumAllocator->RegisterEnum<BlockActorType>(BlockActorType::Count);
		AlchemicalChest = ctx.mEnumAllocator->GetNextValue<BlockActorType>();
		DarkMatterFurnace = ctx.mEnumAllocator->GetNextValue<BlockActorType>();
	}
};

class CustomActorContainerType {
public:
	inline static int FirstAlchemicalBag;
	inline static int LastAlchemicalBag;
	inline static void Initialize() {
		auto& ctx = Amethyst::GetContext();
		ctx.mEnumAllocator->RegisterEnum<ActorContainerType>(ActorContainerType::PlayerEnderChest);
		FirstAlchemicalBag = int(ctx.mEnumAllocator->GetNextValue<ActorContainerType>());
		for (int i = 0; i < 16; i++) {
			// Allocate 16 values for the 16 alchemical bag colors
			(void)ctx.mEnumAllocator->GetNextValue<ActorContainerType>();
		}
		LastAlchemicalBag = int(ctx.mEnumAllocator->GetNextValue<ActorContainerType>());
	}
};

class CustomContainerType {
public:
	inline static ContainerType AlchemicalBag;
	inline static void Initialize() {
		auto& ctx = Amethyst::GetContext();
		ctx.mEnumAllocator->RegisterEnum<ContainerType>(ContainerType::CHEST_BOAT);
		AlchemicalBag = ctx.mEnumAllocator->GetNextValue<ContainerType>();
	}
};

class ModGlobals {
public:
	inline static std::vector<std::string> AlchemicalBagColors = {
		"black",
		"blue",
		"brown",
		"cyan",
		"gray",
		"green",
		"light_blue",
		"light_gray",
		"lime",
		"magenta",
		"orange",
		"pink",
		"purple",
		"red",
		"white",
		"yellow"
	};

	static void InitializeEnums() {
		CustomBlockActorRendererId::Initialize();
		CustomBlockActorType::Initialize();
		CustomActorContainerType::Initialize();
		CustomContainerType::Initialize();
	}
};