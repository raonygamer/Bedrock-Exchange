#pragma once
#include "amethyst/runtime/AmethystContext.hpp"
#include "amethyst/runtime/events/RegisterEvents.hpp"
#include "mc/src/common/world/level/block/BlockLegacy.hpp"
#include "features/blocks/DarkMatterFurnaceBlock.hpp"

class Blocks {
public:
	static WeakPtr<BlockLegacy> AlchemicalCoalBlock;
	static WeakPtr<BlockLegacy>	MobiusFuelBlock;
	static WeakPtr<BlockLegacy> AeternalisFuelBlock;
	static WeakPtr<BlockLegacy> AlchemicalChestBlock;
	static WeakPtr<BlockLegacy> DarkMatterBlock;
	static WeakPtr<BlockLegacy> RedMatterBlock;
	static WeakPtr<DarkMatterFurnaceBlock> DarkMatterFurnaceBlock;

	static void RegisterAllBlocks(RegisterBlocksEvent& event, AmethystContext& ctx);
	static void InitAllBlockGraphics(InitBlockGraphicsEvent& event, AmethystContext& ctx);
	static void InitAllBlockEntityRenderers(InitBlockEntityRenderersEvent& event, AmethystContext& ctx);
};