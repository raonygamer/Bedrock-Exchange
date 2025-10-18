#pragma once
#include <mc/src/common/world/item/Item.hpp>

namespace ee2 {
	class Tiers {
	public:
		static std::unique_ptr<Item::Tier> DARK_MATTER;
		static std::unique_ptr<Item::Tier> RED_MATTER;

		static void Initialize();
	};
}