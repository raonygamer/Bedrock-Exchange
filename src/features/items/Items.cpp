#include "features/items/Items.hpp"
#include "features/blocks/Blocks.hpp"
#include "features/items/AlchemicalBagItem.hpp"
#include "features/items/MatterSword.hpp"
#include "features/items/MatterPickaxe.hpp"
#include "features/items/MatterAxe.hpp"
#include "features/items/MatterShovel.hpp"
#include "features/items/RedMatterSword.hpp"
#include "features/behaviors/items/types/ChargeableItem.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"
#include "features/ModGlobals.hpp"

#include "mc/src-client/common/client/game/ClientInstance.hpp"
#include "mc/src-client/common/client/gui/screens/models/MinecraftScreenModel.hpp"
#include "mc/src-client/common/client/gui/screens/SceneCreationUtils.hpp"
#include "mc/src-deps/core/utility/ServiceLocator.hpp"
#include "mc/src/common/world/item/BlockItem.hpp"

ItemRegistry* Items::ItemRegistry = nullptr;
std::unordered_map<std::string, WeakPtr<AlchemicalBagItem>> Items::AlchemicalBags = {};
WeakPtr<Item> Items::PhilosophersStone = nullptr;
WeakPtr<Item> Items::AlchemicalCoal = nullptr;
WeakPtr<Item> Items::MobiusFuel = nullptr;
WeakPtr<Item> Items::AeternalisFuel = nullptr;
WeakPtr<Item> Items::LowCovalenceDust = nullptr;
WeakPtr<Item> Items::MediumCovalenceDust = nullptr;
WeakPtr<Item> Items::HighCovalenceDust = nullptr;
WeakPtr<Item> Items::DarkMatter = nullptr;
WeakPtr<Item> Items::RedMatter = nullptr;
WeakPtr<Item> Items::DarkMatterSword = nullptr;
WeakPtr<Item> Items::DarkMatterPickaxe = nullptr;
WeakPtr<Item> Items::DarkMatterAxe = nullptr;
WeakPtr<Item> Items::DarkMatterShovel = nullptr;
WeakPtr<Item> Items::RedMatterSword = nullptr;

void Items::RegisterAllItems(RegisterItemsEvent& event, AmethystContext& ctx)
{
	ItemRegistry = &event.itemRegistry;
	{
		auto* item = ItemRegistry->mNameToItemMap["minecraft:diamond_shovel"].get();
		Log::Info("Diamond Shovel Item Ptr: {:x}", GetVtable(item) - GetMinecraftBaseAddress());
	}
	{
		auto* item = ItemRegistry->mNameToItemMap["minecraft:diamond_hoe"].get();
		Log::Info("Diamond Hoe Item Ptr: {:x}", GetVtable(item) - GetMinecraftBaseAddress());
	}
	{
		auto* item = ItemRegistry->mNameToItemMap["minecraft:shears"].get();
		Log::Info("Shears Item Ptr: {:x}", GetVtable(item) - GetMinecraftBaseAddress());
	}

	// Alchemical Bags
	for (const std::string& color : ModGlobals::AlchemicalBagColors) {
		std::string fullName = std::format("ee2:{}_alchemical_bag", color);
		auto item = event.itemRegistry.registerItemShared<AlchemicalBagItem>(fullName, ++event.itemRegistry.mMaxItemID, color);
		AlchemicalBags[color] = item;
	}

	// Philosopher's Stone
	{
		auto item = event.itemRegistry.registerItemShared<BasicBehaviorItem>("ee2:philosophers_stone", ++event.itemRegistry.mMaxItemID);
		item->addBehavior<ChargeableItem>(4, 4);
		item->setIconInfo("ee2:philosophers_stone", 0)
			.setMaxStackSize(1);
		item->mCreativeCategory = CreativeItemCategory::Items;
		PhilosophersStone = item;
	}

	// Alchemical Coal
	{
		auto item = event.itemRegistry.registerItemShared<Item>("ee2:alchemical_coal", ++event.itemRegistry.mMaxItemID);
		item->setIconInfo("ee2:alchemical_coal", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		AeternalisFuel = item;
	}

	// Mobius Fuel
	{
		auto item = event.itemRegistry.registerItemShared<Item>("ee2:mobius_fuel", ++event.itemRegistry.mMaxItemID);
		item->setIconInfo("ee2:mobius_fuel", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		AeternalisFuel = item;
	}

	// Aeternalis Fuel
	{
		auto item = event.itemRegistry.registerItemShared<Item>("ee2:aeternalis_fuel", ++event.itemRegistry.mMaxItemID);
		item->setIconInfo("ee2:aeternalis_fuel", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		AeternalisFuel = item;
	}

	// Low Covalence Dust
	{
		auto item = event.itemRegistry.registerItemShared<Item>("ee2:low_covalence_dust", ++event.itemRegistry.mMaxItemID);
		item->setIconInfo("ee2:low_covalence_dust", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		LowCovalenceDust = item;
	}

	// Medium Covalence Dust
	{
		auto item = event.itemRegistry.registerItemShared<Item>("ee2:medium_covalence_dust", ++event.itemRegistry.mMaxItemID);
		item->setIconInfo("ee2:medium_covalence_dust", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		MediumCovalenceDust = item;
	}

	// High Covalence Dust
	{
		auto item = event.itemRegistry.registerItemShared<Item>("ee2:high_covalence_dust", ++event.itemRegistry.mMaxItemID);
		item->setIconInfo("ee2:high_covalence_dust", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		HighCovalenceDust = item;
	}

	// Dark Matter
	{
		auto item = event.itemRegistry.registerItemShared<Item>("ee2:dark_matter", ++event.itemRegistry.mMaxItemID);
		item->setIconInfo("ee2:dark_matter", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		DarkMatter = item;
	}

	// Red Matter
	{
		auto item = event.itemRegistry.registerItemShared<Item>("ee2:red_matter", ++event.itemRegistry.mMaxItemID);
		item->setIconInfo("ee2:red_matter", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		RedMatter = item;
	}

	// Dark Matter Sword
	{
		auto item = event.itemRegistry.registerItemShared<MatterSword>(
			"ee2:dark_matter_sword",
			++event.itemRegistry.mMaxItemID,
			*ee2::Tiers::DARK_MATTER,
			13
		);
		item->addBehavior<ChargeableItem>(2, 2);
		item->setIconInfo("ee2:dark_matter_sword", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		DarkMatterSword = item;
	}

	// Dark Matter Pìckaxe
	{
		auto item = event.itemRegistry.registerItemShared<MatterPickaxe>(
			"ee2:dark_matter_pickaxe",
			++event.itemRegistry.mMaxItemID,
			*ee2::Tiers::DARK_MATTER
		);
		item->addBehavior<ChargeableItem>(2, 2);
		item->addBehavior<ModeItem>(std::vector<std::string> { 
			"mode.standard", 
			"mode.3x_tallshot", 
			"mode.3x_wideshot", 
			"mode.3x_longshot",
			"mode.3x_plate",
			"mode.veinminer"
		});
		item->setIconInfo("ee2:dark_matter_pickaxe", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		DarkMatterPickaxe = item;
	}

	// Dark Matter Axe
	{
		auto item = event.itemRegistry.registerItemShared<MatterAxe>(
			"ee2:dark_matter_axe",
			++event.itemRegistry.mMaxItemID,
			*ee2::Tiers::DARK_MATTER
		);
		item->addBehavior<ChargeableItem>(2, 2);
		item->addBehavior<ModeItem>(std::vector<std::string> {
			"mode.standard",
			"mode.multi_chop"
		});
		item->setIconInfo("ee2:dark_matter_axe", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		DarkMatterAxe = item;
	}

	// Dark Matter Shovel
	{
		auto item = event.itemRegistry.registerItemShared<MatterShovel>(
			"ee2:dark_matter_shovel",
			++event.itemRegistry.mMaxItemID,
			*ee2::Tiers::DARK_MATTER
		);
		item->addBehavior<ChargeableItem>(2, 2);
		item->addBehavior<ModeItem>(std::vector<std::string> {
			"mode.standard",
			"mode.3x_tallshot",
			"mode.3x_wideshot",
			"mode.3x_longshot",
			"mode.3x_plate"
		});
		item->setIconInfo("ee2:dark_matter_shovel", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		DarkMatterShovel = item;
	}

	// Red Matter Sword
	{
		auto item = event.itemRegistry.registerItemShared<::RedMatterSword>(
			"ee2:red_matter_sword",
			++event.itemRegistry.mMaxItemID
		);
		item->addBehavior<ChargeableItem>(3, 3);
		item->setIconInfo("ee2:red_matter_sword", 0);
		item->mCreativeCategory = CreativeItemCategory::Items;
		RedMatterSword = item;
	}

	// Blocks
	{
		// Alchemical Coal Block
		{
			event.itemRegistry.registerItemShared<BlockItem>(
				Blocks::AlchemicalCoalBlock->mNameInfo.mFullName.getString(),
				Blocks::AlchemicalCoalBlock->getBlockItemId(),
				HashedString::EMPTY
			)->mCreativeCategory = CreativeItemCategory::Items;
		}

		// Mobius Fuel Block
		{
			event.itemRegistry.registerItemShared<BlockItem>(
				Blocks::MobiusFuelBlock->mNameInfo.mFullName.getString(),
				Blocks::MobiusFuelBlock->getBlockItemId(),
				HashedString::EMPTY
			)->mCreativeCategory = CreativeItemCategory::Items;
		}

		// Aeternalis Fuel Block
		{
			event.itemRegistry.registerItemShared<BlockItem>(
				Blocks::AeternalisFuelBlock->mNameInfo.mFullName.getString(),
				Blocks::AeternalisFuelBlock->getBlockItemId(),
				HashedString::EMPTY
			)->mCreativeCategory = CreativeItemCategory::Items;
		}

		// Alchemical Chest Block
		{
			event.itemRegistry.registerItemShared<BlockItem>(
				Blocks::AlchemicalChestBlock->mNameInfo.mFullName.getString(),
				Blocks::AlchemicalChestBlock->getBlockItemId(),
				HashedString::EMPTY
			)->mCreativeCategory = CreativeItemCategory::Items;
		}

		// Dark Matter Block
		{
			event.itemRegistry.registerItemShared<BlockItem>(
				Blocks::DarkMatterBlock->mNameInfo.mFullName.getString(),
				Blocks::DarkMatterBlock->getBlockItemId(),
				HashedString::EMPTY
			)->mCreativeCategory = CreativeItemCategory::Items;
		}

		// Red Matter Block
		{
			event.itemRegistry.registerItemShared<BlockItem>(
				Blocks::RedMatterBlock->mNameInfo.mFullName.getString(),
				Blocks::RedMatterBlock->getBlockItemId(),
				HashedString::EMPTY
			)->mCreativeCategory = CreativeItemCategory::Items;
		}

		// Dark Matter Furnace Block
		{
			auto item = event.itemRegistry.registerItemShared<BlockItem>(
				Blocks::DarkMatterFurnaceBlock->mNameInfo.mFullName.getString(),
				Blocks::DarkMatterFurnaceBlock->getBlockItemId(),
				HashedString::EMPTY
			);
			item->mCreativeCategory = CreativeItemCategory::Items;
			item->setIsHiddenInCommands(ItemCommandVisibility::Visible);
		}
	}
}