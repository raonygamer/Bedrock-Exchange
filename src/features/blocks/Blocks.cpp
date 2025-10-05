#include "Blocks.hpp"
#include "mc/src/common/world/level/block/definition/BlockDefinitionGroup.hpp"
#include "mc/src/common/world/level/block/types/ChestBlock.hpp"
#include "mc/src/common/world/level/block/registry/BlockTypeRegistry.hpp"
#include "mc/src-client/common/client/renderer/block/BlockGraphics.hpp"
#include "mc/src-client/common/client/renderer/blockActor/BlockActorRendererDispatcher.hpp"

#include "features/renderer/blockActor/AlchemicalChestRenderer.hpp"
#include "features/blocks/AlchemicalChestBlock.hpp"
#include "features/ModGlobals.hpp"

std::unordered_map<std::string, WeakPtr<BlockLegacy>> Blocks::sBlocks;

void Blocks::RegisterAllBlocks(RegisterBlocksEvent& event, AmethystContext& ctx) {
	auto& blockDefs = event.blockDefinitions;
	auto block = BlockTypeRegistry::registerBlock<AlchemicalChestBlock>("ee2:alchemical_chest", ++blockDefs.mLastBlockId);
	block->mCreativeCategory = CreativeItemCategory::Items;
	sBlocks["ee2:alchemical_chest"] = block;
}

void Blocks::InitAllBlockGraphics(InitBlockGraphicsEvent& event, AmethystContext& ctx) {
	{
		HashedString blockName("ee2:alchemical_chest");
		auto* graphics = BlockGraphics::createBlockGraphics(blockName, BlockShape::CHEST);
		if (graphics->mTextureItems.size() == 0) {
			graphics->setTextureItem(
				"alchemical_chest_inventory_top",
				"alchemical_chest_inventory_top",
				"alchemical_chest_inventory_side",
				"alchemical_chest_inventory_front",
				"alchemical_chest_inventory_side",
				"alchemical_chest_inventory_side"
			);
		}
		graphics->setDefaultCarriedTextures();
	}
}

void Blocks::InitAllBlockEntityRenderers(InitBlockEntityRenderersEvent& event, AmethystContext& ctx) {
	event.renderDispatcher.mRenderers[CustomBlockActorRendererId::TR_ALCHEMICAL_CHEST] = std::make_unique<AlchemicalChestRenderer>(event.textures);
}