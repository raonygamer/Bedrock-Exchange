#include "features/utility/BlockUtils.hpp"
#include "mc/src/common/world/level/BlockSource.hpp"
#include "mc/src/common/world/level/block/Block.hpp"
#include "mc/src/common/world/level/block/BlockLegacy.hpp"
#include <glm/geometric.hpp>

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

std::vector<std::pair<BlockPos, const Block*>> BlockUtils::getBlocksForMode(size_t mode, BlockSource& region, const BlockPos& center, const Directions& directions) {
	std::vector<std::pair<BlockPos, const Block*>> result = {};
	if (mode == 0) {
		result.push_back({ center, &region.getBlock(center) });
	}
	// Line modes
	else if (mode == 1 || mode == 2 || mode == 3) {
		const int lineSize = 1;
		for (int i = -lineSize; i <= lineSize; i++) {
			BlockPos newPos = center;
			Vec3 offset;

			switch (mode) {
			case 1: offset = directions.mUp * i; break;      // Tallshot
			case 2: offset = directions.mRight * i; break;   // Wideshot
			case 3: offset = directions.mForward * i; break; // Longshot
			}

			newPos.x += (int)std::round(offset.x);
			newPos.y += (int)std::round(offset.y);
			newPos.z += (int)std::round(offset.z);
			result.push_back({ newPos, &region.getBlock(newPos) });
		}
	}
	// 3x Plate
	else if (mode == 4) {
		const int plateSize = 1;
		for (int u = -plateSize; u <= plateSize; u++) {
			for (int r = -plateSize; r <= plateSize; r++) {
				BlockPos newPos = center;
				Vec3 offset = directions.mUp * u + directions.mRight * r;
				newPos.x += (int)std::round(offset.x);
				newPos.y += (int)std::round(offset.y);
				newPos.z += (int)std::round(offset.z);
				result.push_back({ newPos, &region.getBlock(newPos) });
			}
		}
	}
	// Veinminer
	else if (mode == 5) {
		auto* defaultState = region.getBlock(center).mLegacyBlock->mDefaultState;
		if (!defaultState)
			defaultState = &region.getBlock(center);
		if (!defaultState->mLegacyBlock->mNameInfo.mFullName.getString().ends_with("_ore"))
			return result;
		const int radius = 8;
		const int maxBlocks = radius * radius * radius + 1;
		result = BlockUtils::floodFillBlocks(region, center, region.getBlock(center).mLegacyBlock, radius, maxBlocks);
	}

	return result;
}

Directions Directions::fromLookVec3(const Vec3& lookDir) {
	glm::vec3 forward = glm::normalize(lookDir.operator glm::vec<3, float, glm::packed_highp>());

	float ax = std::abs(forward.x);
	float ay = std::abs(forward.y);
	float az = std::abs(forward.z);

	glm::vec3 f(0.0f), r(0.0f), u(0.0f);
	if (ax > ay && ax > az) {
		f = glm::vec3(forward.x > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
		u = glm::vec3(0.0f, 1.0f, 0.0f);
		r = glm::vec3(0.0f, 0.0f, (forward.x > 0 ? 1.0f : -1.0f));
	} else if (ay > az) {
		f = glm::vec3(0.0f, forward.y > 0 ? 1.0f : -1.0f, 0.0f);
		u = glm::vec3(0.0f, 0.0f, 1.0f);
		r = glm::vec3(forward.y > 0 ? 1.0f : -1.0f, 0.0f, 0.0f);
	} else {
		f = glm::vec3(0.0f, 0.0f, forward.z > 0 ? 1.0f : -1.0f);
		u = glm::vec3(0.0f, 1.0f, 0.0f);
		r = glm::vec3(forward.z > 0 ? -1.0f : 1.0f, 0.0f, 0.0f);
	}
	return Directions{
		.mUp = u,
		.mRight = r,
		.mForward = f
	};
}