#pragma once
#include "amethyst/runtime/AmethystContext.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"
#include "minecraft/src/common/world/level/block/BlockLegacy.hpp"

class Blocks {
public:
	static std::unordered_map<std::string, WeakPtr<BlockLegacy>> sBlocks;

	static void RegisterAllBlocks(RegisterBlocksEvent& event, AmethystContext& ctx);
	static void InitAllBlockGraphics(InitBlockGraphicsEvent& event, AmethystContext& ctx);
};