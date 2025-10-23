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

ItemBehaviorStorage* ItemBehaviorStorage::tryGetStorage(const ItemStackBase& stack) {
	if (!stack.mItem)
		return nullptr;
	return tryGetStorage(*stack.mItem);
}

ItemBehaviorStorage* ItemBehaviorStorage::tryGetStorage(Item& item) {
	if (!item.hasTag(ItemBehaviorStorage::STORAGE_TAG))
		return nullptr;
	if (auto storage = dynamic_cast<ItemBehaviorStorage*>(&item)) {
		return storage;
	}
	AssertFail("Item has behavior storage tag but is not an ItemBehaviorStorage.");
}

const ItemBehaviorStorage* ItemBehaviorStorage::tryGetStorage(const Item& item) {
	if (!item.hasTag(ItemBehaviorStorage::STORAGE_TAG))
		return nullptr;
	if (auto storage = dynamic_cast<const ItemBehaviorStorage*>(&item)) {
		return storage;
	}
	AssertFail("Item has behavior storage tag but is not an ItemBehaviorStorage.");
}