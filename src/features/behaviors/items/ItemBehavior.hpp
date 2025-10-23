#pragma once
#include "features/behaviors/Behavior.hpp"

class ItemBehaviorStorage;
class ItemBehavior : 
	public Behavior
{
public:
	explicit ItemBehavior(BehaviorStorage* owner);
};