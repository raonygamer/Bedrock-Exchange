#include "features/behaviors/items/ItemBehavior.hpp"
#include "features/behaviors/BehaviorStorage.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"

ItemBehavior::ItemBehavior(BehaviorStorage* owner) :
	Behavior(owner)
{
	if (owner->getTypeIndex() != typeid(ItemBehaviorStorage)) {
		AssertFail("ItemBehavior can only be attached to ItemBehaviorStorage, but got '{}'.", owner->getTypeIndex().name());
	}
}
