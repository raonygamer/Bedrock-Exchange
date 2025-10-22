#pragma once
#include <vector>

class Block;
class BlockPos;
class BlockLegacy;
class BlockSource;
class BlockUtils {
public:
	static bool inCubeRange(const BlockPos& a, const BlockPos& b, int radius);
	static std::vector<std::pair<BlockPos, const Block*>> floodFillBlocks(BlockSource& region, const BlockPos& startPos, BlockLegacy* type, int radius, int maxBlocks = 1000);
	static std::vector<std::pair<BlockPos, const Block*>> floodFillLiquidBlocks(BlockSource& region, const BlockPos& startPos, BlockLegacy* type, int radius, int maxBlocks = 1000);
};