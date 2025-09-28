#include "Blocks.hpp"
#include "mc/src/common/world/level/block/definition/BlockDefinitionGroup.hpp"
#include "mc/src/common/world/level/block/registry/BlockTypeRegistry.hpp"
#include "mc/src-client/common/client/renderer/block/BlockGraphics.hpp"

std::unordered_map<std::string, WeakPtr<BlockLegacy>> Blocks::sBlocks;

void Blocks::RegisterAllBlocks(RegisterBlocksEvent& event, AmethystContext& ctx)
{
}

void Blocks::InitAllBlockGraphics(InitBlockGraphicsEvent& event, AmethystContext& ctx)
{
}
