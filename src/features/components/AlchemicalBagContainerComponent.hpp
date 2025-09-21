#pragma once
#include <memory>
#include <array>
#include "features/containers/AlchemicalBagContainer.hpp"

class FillingContainer;

struct AlchemicalBagContainerComponent {
	std::array<std::unique_ptr<AlchemicalBagContainer>, 16> mContainers;
};