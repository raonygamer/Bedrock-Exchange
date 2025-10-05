#pragma once
#include "amethyst/runtime/ModContext.hpp"

#include "mc/src/common/world/level/block/actor/BlockActorRendererId.hpp"

#include <string>
#include <vector>

class CustomBlockActorRendererId {
public:
	inline static BlockActorRendererId TR_ALCHEMICAL_CHEST;

	inline static void Initialize() {
		auto& ctx = Amethyst::GetContext();
		ctx.mEnumAllocator->RegisterEnum<BlockActorRendererId>(BlockActorRendererId::PADDING);
		TR_ALCHEMICAL_CHEST = ctx.mEnumAllocator->GetNextValue<BlockActorRendererId>();
	}
};

class CustomBlockActorType {
public:
	inline static BlockActorType AlchemicalChest;

	inline static void Initialize() {
		auto& ctx = Amethyst::GetContext();
		ctx.mEnumAllocator->RegisterEnum<BlockActorType>(BlockActorType::Last);
		AlchemicalChest = ctx.mEnumAllocator->GetNextValue<BlockActorType>();
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
	}
};