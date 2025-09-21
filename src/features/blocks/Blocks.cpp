#include "Blocks.hpp"
#include "minecraft/src/common/world/level/block/definition/BlockDefinitionGroup.hpp"
#include "minecraft/src/common/world/level/block/registry/BlockTypeRegistry.hpp"
#include "minecraft/src-client/common/client/renderer/block/BlockGraphics.hpp"

std::unordered_map<std::string, WeakPtr<BlockLegacy>> Blocks::sBlocks;

void Blocks::RegisterAllBlocks(RegisterBlocksEvent& event, AmethystContext& ctx)
{
}

void Blocks::InitAllBlockGraphics(InitBlockGraphicsEvent& event, AmethystContext& ctx)
{
}
