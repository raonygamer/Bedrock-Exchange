#include "features/utility/BlockUtils.hpp"
#include "mc/src/common/world/level/BlockSource.hpp"
#include "mc/src/common/world/level/block/Block.hpp"
#include "mc/src/common/world/level/block/BlockLegacy.hpp"

std::vector<std::pair<BlockPos, const Block*>> BlockUtils::floodFillBlocks(BlockSource& region, const BlockPos& startPos, BlockLegacy* type, int radius, int maxBlocks) {
	std::vector<std::pair<BlockPos, const Block*>> result;
	const Block& startBlock = region.getBlock(startPos);
	const BlockLegacy* blockType = startBlock.mLegacyBlock;

	static std::array<BlockPos, 26> directions = [] {
		std::array<BlockPos, 26> dirs{};
		int idx = 0;
		for (int dx = -1; dx <= 1; dx++)
			for (int dy = -1; dy <= 1; dy++)
				for (int dz = -1; dz <= 1; dz++)
					if (!(dx == 0 && dy == 0 && dz == 0))
						dirs[idx++] = { dx, dy, dz };
		return dirs;
	}();

	std::queue<BlockPos> queue;
	std::unordered_set<BlockPos> visited;
	visited.reserve(maxBlocks);

	queue.push(startPos);
	visited.insert(startPos);

	int processed = 0;

	while (!queue.empty() && processed < maxBlocks) {
		BlockPos pos = queue.front();
		queue.pop();

		const Block& block = region.getBlock(pos);
		result.push_back({ pos, &block });
		processed++;

		for (auto& d : directions) {
			BlockPos neighbor = { pos.x + d.x, pos.y + d.y, pos.z + d.z };

			// Cube bounds check
			if (std::abs(neighbor.x - startPos.x) > radius ||
				std::abs(neighbor.y - startPos.y) > radius ||
				std::abs(neighbor.z - startPos.z) > radius)
				continue;

			if (visited.contains(neighbor))
				continue;

			const Block& neighborBlock = region.getBlock(neighbor);
			if (neighborBlock.mLegacyBlock == blockType) {
				visited.insert(neighbor);
				queue.push(neighbor);
			}
		}
	}
	return result;
}