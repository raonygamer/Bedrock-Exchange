#pragma once
#include <vector>
#include "mc/src/common/world/phys/Vec3.hpp"

class Block;
class BlockPos;
class BlockLegacy;
class BlockSource;

struct Directions {
	Vec3 mUp;
	Vec3 mRight;
	Vec3 mForward;

	static Directions fromLookVec3(const Vec3& lookDir);
};

class BlockUtils {
public:
	static std::vector<std::pair<BlockPos, const Block*>> floodFillBlocks(BlockSource& region, const BlockPos& startPos, BlockLegacy* type, int radius, int maxBlocks = 1000);
	static std::vector<std::pair<BlockPos, const Block*>> getBlocksForMode(size_t mode, BlockSource& region, const BlockPos& center, const Directions& directions);
};