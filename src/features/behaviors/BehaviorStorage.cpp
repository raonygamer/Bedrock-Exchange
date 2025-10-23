#include "features/behaviors/BehaviorStorage.hpp"
#include "features/behaviors/items/ItemBehaviorStorage.hpp"

Behavior& BehaviorStorage::_addBehavior(std::unique_ptr<Behavior>&& behavior) {
	auto type = behavior->getTypeIndex();
	if (mBehaviors.find(type) == mBehaviors.end()) {
		mBehaviors[type] = std::vector<std::unique_ptr<Behavior>>{};
	}

	if (behavior->isSinglePerStorage() && mBehaviors[type].size() > 0) {
		AssertFail("Failed to add behavior '{}', there is already one of the same type {} and it's marked as single.", behavior->getBehaviorName(), type.name());
	}
	return *mBehaviors[type].emplace_back(std::move(behavior));
}

std::vector<Behavior*> BehaviorStorage::getBehaviors(const std::type_index& type) const {
	if (mBehaviors.find(type) == mBehaviors.end()) {
		return {};
	}
	std::vector<Behavior*> behaviors;
	for (const auto& behaviorPtr : mBehaviors.at(type)) {
		behaviors.push_back(behaviorPtr.get());
	}
	return behaviors;
}

Behavior* BehaviorStorage::getFirstBehavior(const std::type_index& type) const {
	if (mBehaviors.find(type) == mBehaviors.end() || mBehaviors.at(type).empty()) {
		return nullptr;
	}
	return mBehaviors.at(type).front().get();
}

bool BehaviorStorage::hasBehavior(const std::type_index& type) const {
	return mBehaviors.find(type) != mBehaviors.end() && !mBehaviors.at(type).empty();
}

void BehaviorStorage::removeAllBehaviors(const std::type_index& type) {
	if (mBehaviors.find(type) != mBehaviors.end()) {
		mBehaviors.erase(type);
	}
}

void BehaviorStorage::removeBehavior(Behavior* behavior) {
	auto type = behavior->getTypeIndex();
	if (mBehaviors.find(type) == mBehaviors.end()) {
		return;
	}
	auto& behaviors = mBehaviors[type];
	behaviors.erase(
		std::remove_if(
			behaviors.begin(),
			behaviors.end(),
			[behavior](const std::unique_ptr<Behavior>& ptr) {
				return ptr.get() == behavior;
			}
		),
		behaviors.end()
	);
}

ItemBehaviorStorage* BehaviorStorage::getForItem(Item& item) {
	if (!item.hasTag(ItemBehaviorStorage::STORAGE_TAG))
		return nullptr;
	if (auto storage = dynamic_cast<ItemBehaviorStorage*>(&item)) {
		return storage;
	}
	AssertFail("Item has behavior storage tag but is not an ItemBehaviorStorage.");
}

const ItemBehaviorStorage* BehaviorStorage::getForItem(const Item& item) {
	if (!item.hasTag(ItemBehaviorStorage::STORAGE_TAG))
		return nullptr;
	if (auto storage = dynamic_cast<const ItemBehaviorStorage*>(&item)) {
		return storage;
	}
	AssertFail("Item has behavior storage tag but is not an ItemBehaviorStorage.");
}
