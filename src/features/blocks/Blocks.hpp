#pragma once
#include "amethyst/runtime/AmethystContext.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"
#include "mc/src/common/world/level/block/BlockLegacy.hpp"

class Blocks {
public:
	static WeakPtr<BlockLegacy> AlchemicalCoalBlock;
	static WeakPtr<BlockLegacy>	MobiusFuelBlock;
	static WeakPtr<BlockLegacy> AeternalisFuelBlock;
	static WeakPtr<BlockLegacy> AlchemicalChestBlock;
	static WeakPtr<BlockLegacy> DarkMatterBlock;
	static WeakPtr<BlockLegacy> RedMatterBlock;

	static void RegisterAllBlocks(RegisterBlocksEvent& event, AmethystContext& ctx);
	static void InitAllBlockGraphics(InitBlockGraphicsEvent& event, AmethystContext& ctx);
	static void InitAllBlockEntityRenderers(InitBlockEntityRenderersEvent& event, AmethystContext& ctx);
};