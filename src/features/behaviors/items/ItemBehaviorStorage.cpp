#include "features/behaviors/items/ItemBehaviorStorage.hpp"

const HashedString ItemBehaviorStorage::STORAGE_TAG = { "ee2:behavior_storage" };

ItemBehaviorStorage::ItemBehaviorStorage(Item* owner) :
	mOwner(owner)
{
	mOwner->addTag({ STORAGE_TAG });
}

Behavior::Event::Event(BehaviorStorage& storage, Behavior& sender) : 
	mStorage(storage), 
	mSender(sender) 
{
}

std::type_index ItemBehaviorStorage::getTypeIndex() const {
	return typeid(ItemBehaviorStorage);
}

Item* ItemBehaviorStorage::getOwner() const {
	return mOwner;
}