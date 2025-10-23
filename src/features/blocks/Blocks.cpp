#include "Blocks.hpp"
#include "mc/src/common/world/level/block/definition/BlockDefinitionGroup.hpp"
#include "mc/src/common/world/level/block/types/ChestBlock.hpp"
#include "mc/src/common/world/level/block/registry/BlockTypeRegistry.hpp"
#include "mc/src-client/common/client/renderer/block/BlockGraphics.hpp"
#include "mc/src-client/common/client/renderer/blockActor/BlockActorRendererDispatcher.hpp"

#include "features/renderer/blockActor/AlchemicalChestRenderer.hpp"
#include "features/blocks/AlchemicalChestBlock.hpp"
#include "features/ModGlobals.hpp"

WeakPtr<BlockLegacy> Blocks::AlchemicalCoalBlock = nullptr;
WeakPtr<BlockLegacy> Blocks::MobiusFuelBlock = nullptr;
WeakPtr<BlockLegacy> Blocks::AeternalisFuelBlock = nullptr;
WeakPtr<BlockLegacy> Blocks::AlchemicalChestBlock = nullptr;
WeakPtr<BlockLegacy> Blocks::DarkMatterBlock = nullptr;
WeakPtr<BlockLegacy> Blocks::RedMatterBlock = nullptr;
WeakPtr<DarkMatterFurnaceBlock> Blocks::DarkMatterFurnaceBlock = nullptr;

#pragma optimize("", off)
void Blocks::RegisterAllBlocks(RegisterBlocksEvent& event, AmethystContext& ctx) {
	auto& blockDefs = event.blockDefinitions;
	auto& hayBlock = BlockTypeRegistry::mBlockLookupMap["minecraft:hay_block"];

	// Alchemical Coal Block
	{
		auto block = BlockTypeRegistry::registerBlock<BlockLegacy>("ee2:alchemical_coal_block", ++blockDefs.mLastBlockId, Material::getMaterial(MaterialType::Solid));
		block->mCreativeCategory = CreativeItemCategory::Items;
		AlchemicalCoalBlock = block;
	}

	// Mobius Fuel Block
	{
		auto block = BlockTypeRegistry::registerBlock<BlockLegacy>("ee2:mobius_fuel_block", ++blockDefs.mLastBlockId, Material::getMaterial(MaterialType::Solid));
		block->mCreativeCategory = CreativeItemCategory::Items;
		MobiusFuelBlock = block;
	}

	// Aeternalis Fuel Block
	{
		auto block = BlockTypeRegistry::registerBlock<BlockLegacy>("ee2:aeternalis_fuel_block", ++blockDefs.mLastBlockId, Material::getMaterial(MaterialType::Solid));
		block->mCreativeCategory = CreativeItemCategory::Items;
		AeternalisFuelBlock = block;
	}

	// Alchemical Chest Block
	{
		auto block = BlockTypeRegistry::registerBlock<::AlchemicalChestBlock>("ee2:alchemical_chest", ++blockDefs.mLastBlockId);
		block->mCreativeCategory = CreativeItemCategory::Items;
		AlchemicalChestBlock = block;
	}

	// Dark Matter Block
	{
		auto block = BlockTypeRegistry::registerBlock<BlockLegacy>("ee2:dark_matter_block", ++blockDefs.mLastBlockId, Material::getMaterial(MaterialType::Metal));
		block->mCreativeCategory = CreativeItemCategory::Items;
		DarkMatterBlock = block;
	}

	// Red Matter Block
	{
		auto block = BlockTypeRegistry::registerBlock<BlockLegacy>("ee2:red_matter_block", ++blockDefs.mLastBlockId, Material::getMaterial(MaterialType::Metal));
		block->mCreativeCategory = CreativeItemCategory::Items;
		RedMatterBlock = block;
	}

	// Dark Matter Furnace Block
	{
		auto block = BlockTypeRegistry::registerBlock<::DarkMatterFurnaceBlock>("ee2:dark_matter_furnace", ++blockDefs.mLastBlockId, false);
		block->mCreativeCategory = CreativeItemCategory::Items;
		DarkMatterFurnaceBlock = block;
	}
}
#pragma optimize("", on)

void Blocks::InitAllBlockGraphics(InitBlockGraphicsEvent& event, AmethystContext& ctx) {
	// Alchemical Coal Block
	{
		BlockGraphics& graphics = *BlockGraphics::createBlockGraphics(AlchemicalCoalBlock->mNameInfo.mFullName, BlockShape::BLOCK);
		if (graphics.mTextureItems.size() == 0) {
			graphics.setTextureItem(
				"ee2:alchemical_coal_block",
				"ee2:alchemical_coal_block",
				"ee2:alchemical_coal_block",
				"ee2:alchemical_coal_block",
				"ee2:alchemical_coal_block",
				"ee2:alchemical_coal_block"
			);
		}
		graphics.setDefaultCarriedTextures();
	}

	// Mobius Fuel Block
	{
		BlockGraphics& graphics = *BlockGraphics::createBlockGraphics(MobiusFuelBlock->mNameInfo.mFullName, BlockShape::BLOCK);
		if (graphics.mTextureItems.size() == 0) {
			graphics.setTextureItem(
				"ee2:mobius_fuel_block",
				"ee2:mobius_fuel_block",
				"ee2:mobius_fuel_block",
				"ee2:mobius_fuel_block",
				"ee2:mobius_fuel_block",
				"ee2:mobius_fuel_block"
			);
		}
		graphics.setDefaultCarriedTextures();
	}

	// Aeternalis Fuel Block
	{
		BlockGraphics& graphics = *BlockGraphics::createBlockGraphics(AeternalisFuelBlock->mNameInfo.mFullName, BlockShape::BLOCK);
		if (graphics.mTextureItems.size() == 0) {
			graphics.setTextureItem(
				"ee2:aeternalis_fuel_block",
				"ee2:aeternalis_fuel_block",
				"ee2:aeternalis_fuel_block",
				"ee2:aeternalis_fuel_block",
				"ee2:aeternalis_fuel_block",
				"ee2:aeternalis_fuel_block"
			);
		}
		graphics.setDefaultCarriedTextures();
	}
	
	// Alchemical Chest Block
	{
		BlockGraphics& graphics = *BlockGraphics::createBlockGraphics(AlchemicalChestBlock->mNameInfo.mFullName, BlockShape::CHEST);
		if (graphics.mTextureItems.size() == 0) {
			graphics.setTextureItem(
				"ee2:alchemical_chest_inventory_top",
				"ee2:alchemical_chest_inventory_top",
				"ee2:alchemical_chest_inventory_side",
				"ee2:alchemical_chest_inventory_front",
				"ee2:alchemical_chest_inventory_side",
				"ee2:alchemical_chest_inventory_side"
			);
		}
		graphics.setDefaultCarriedTextures();
	}

	// Dark Matter Block
	{
		BlockGraphics& graphics = *BlockGraphics::createBlockGraphics(DarkMatterBlock->mNameInfo.mFullName, BlockShape::BLOCK);
		if (graphics.mTextureItems.size() == 0) {
			graphics.setTextureItem(
				"ee2:dark_matter_block",
				"ee2:dark_matter_block",
				"ee2:dark_matter_block",
				"ee2:dark_matter_block",
				"ee2:dark_matter_block",
				"ee2:dark_matter_block"
			);
		}
		graphics.setDefaultCarriedTextures();
	}

	// Red Matter Block
	{
		BlockGraphics& graphics = *BlockGraphics::createBlockGraphics(RedMatterBlock->mNameInfo.mFullName, BlockShape::BLOCK);
		if (graphics.mTextureItems.size() == 0) {
			graphics.setTextureItem(
				"ee2:red_matter_block",
				"ee2:red_matter_block",
				"ee2:red_matter_block",
				"ee2:red_matter_block",
				"ee2:red_matter_block",
				"ee2:red_matter_block"
			);
		}
		graphics.setDefaultCarriedTextures();
	}

	// Dark Matter Furnace Block
	{
		BlockGraphics& graphics = *BlockGraphics::createBlockGraphics(DarkMatterFurnaceBlock->mNameInfo.mFullName, BlockShape::BLOCK);
		if (graphics.mTextureItems.size() == 0) {
			graphics.setTextureItem(
				"ee2:dm_furnace_top",
				"ee2:dm_furnace_top",
				"ee2:dm_furnace_side",
				"ee2:dm_furnace_front",
				"ee2:dm_furnace_side",
				"ee2:dm_furnace_side"
			);
		}
		graphics.setDefaultCarriedTextures();
	}
}

void Blocks::InitAllBlockEntityRenderers(InitBlockEntityRenderersEvent& event, AmethystContext& ctx) {
	event.renderDispatcher.mRenderers[CustomBlockActorRendererId::TR_ALCHEMICAL_CHEST] = std::make_unique<AlchemicalChestRenderer>(event.textures);
}