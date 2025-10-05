#pragma once
#include "features/emc/ItemID.hpp"

class Level;

namespace ee2::emc {
class EMCUtils {
public:
	static uint64_t calculateItemEMC(uint64_t baseEMC, const ItemStackBase& stack, Level& level);
	static uint64_t calculateStackEMC(uint64_t emc, const ItemStackBase& stack);
};
}