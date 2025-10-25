#include "features/items/Tiers.hpp"

namespace ee2 {
	std::unique_ptr<Item::Tier> Tiers::DARK_MATTER = nullptr;
	std::unique_ptr<Item::Tier> Tiers::RED_MATTER = nullptr;

	void Tiers::Initialize() {
		DARK_MATTER = std::make_unique<Item::Tier>(
			5,     // level
			-1,    // uses
			18.5f, // speed
			8,     // damage
			20     // enchantment value
		);

		RED_MATTER =  std::make_unique<Item::Tier>(
			6,     // level
			-1,    // uses
			25.0f, // speed
			11,     // damage
			25     // enchantment value
		);
	}
}