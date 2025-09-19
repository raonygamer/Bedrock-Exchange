#pragma once
#include <memory>
#include "features/containers/AlchemicalBagContainer.hpp"

class FillingContainer;

struct AlchemicalBagContainerComponent {
	std::unique_ptr<AlchemicalBagContainer> mContainer;
};