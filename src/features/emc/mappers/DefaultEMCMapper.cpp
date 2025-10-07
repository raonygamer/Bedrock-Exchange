#include "features/emc/mappers/DefaultEMCMapper.hpp"

namespace ee2::emc {
	void DefaultEMCMapper::init() {
		mEMCValues = {
			{ ItemID{ "minecraft:redstone" }, 64 },
			{ ItemID{ "minecraft:enchanted_book" }, 160 },
			{ ItemID{ "minecraft:coal" }, 128 },
			{ ItemID{ "minecraft:charcoal" }, 32 },
			{ ItemID{ "minecraft:copper_ingot" }, 128 },
			{ ItemID{ "minecraft:iron_ingot" }, 256 },
			{ ItemID{ "minecraft:gold_ingot" }, 2048 },
			{ ItemID{ "minecraft:emerald" }, 16384 },
			{ ItemID{ "minecraft:diamond" }, 8192 },
			{ ItemID{ "minecraft:netherite_scrap" }, 12288 },
			{ ItemID{ "minecraft:stone" }, 1 },
			{ ItemID{ "minecraft:dirt" }, 1 },
			{ ItemID{ "minecraft:grass_block" }, 1 },
			{ ItemID{ "minecraft:cobblestone" }, 1 },
			{ ItemID{ "minecraft:netherrack" }, 1 },
			{ ItemID{ "minecraft:netherite_upgrade_smithing_template" }, 7497 },
			{ ItemID{ "minecraft:bamboo" }, 32 },
			{ ItemID{ "minecraft:nether_wart" }, 24 },
			{ ItemID{ "minecraft:glass_bottle" }, 1 },
			{ ItemID{ "minecraft:potion", {}, 0 }, 1 },
			{ ItemID{ "minecraft:gunpowder" }, 192 },
			{ ItemID{ "minecraft:glowstone_dust" }, 384 },
			{ ItemID{ "minecraft:sugar_cane" }, 32 },
			{ ItemID{ "minecraft:shulker_shell" }, 2048 },
			{ ItemID{ "minecraft:ink_sac" }, 16 },
			{ ItemID{ "minecraft:glow_ink_sac" }, 400 },
			{ ItemID{ "minecraft:leather" }, 64 },
			{ ItemID{ "minecraft:obsidian" }, 64 },
			{ ItemID{ "minecraft:sentry_armor_trim_smithing_template" }, 57345 },
		};

		mTagEMCValues = {
			{ "minecraft:logs", 32 },
			{ "minecraft:wool", 48 },
		};

		Log::Info("[{}] mapped {} EMC values for default tags", getMapperName(), mTagEMCValues.size());
		Log::Info("[{}] mapped {} EMC values for default items", getMapperName(), mEMCValues.size());
	}

	EMCValue DefaultEMCMapper::getEMC(const ItemID& identifier) {
		// Yeah, I know this is O(n) but CPUs don't care about that these days
		// And I can't do a exact match with unordered_map
		for (const auto& [id, emc] : mEMCValues) {
			if (id.name == identifier.name && (id.aux == 0x7fff || identifier.aux == 0x7fff || id.aux == identifier.aux)) {
				return EMCValue(emc);
			}
		}

		return EMCValue::EMPTY;
	}

	bool DefaultEMCMapper::canHandleTags() {
		return true;
	}

	EMCValue DefaultEMCMapper::getEMCByTag(const ItemTag& tag) {
		auto tagIt = mTagEMCValues.find(tag);
		if (tagIt != mTagEMCValues.end()) {
			return EMCValue(tagIt->second);
		}
		return EMCValue::EMPTY;
	}

	std::string DefaultEMCMapper::getMapperName() {
		return "DefaultEMCMapper";
	}

	void DefaultEMCMapper::setEMC(const ItemID& identifier, uint64_t emc) {
		// Try to find an existing entry
		auto it = std::find_if(
			mEMCValues.begin(),
			mEMCValues.end(),
			[&](const auto& pair) { return pair.first.equals(identifier); }
		);

		if (it != mEMCValues.end()) {
			// Update existing entry if it exists
			it->second = emc;
		}
		else {
			// If there was no existing entry, add a new one
			mEMCValues.push_back({ identifier, emc });
		}
	}
} // namespace ee2::emc