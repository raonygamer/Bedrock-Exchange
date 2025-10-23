#pragma once
#include <unordered_map>
#include <typeindex>
#include <vector>
#include <memory>
#include <utility>

#include "features/behaviors/Behavior.hpp"
#include "amethyst/Log.hpp"

class Behavior;
class ItemBehaviorStorage;
class Item;
class BehaviorStorage {
public:
	std::unordered_map<std::type_index, std::vector<std::unique_ptr<Behavior>>> mBehaviors = {};

	virtual ~BehaviorStorage() = default;
	virtual Behavior& _addBehavior(std::unique_ptr<Behavior>&& behavior);
	virtual std::vector<Behavior*> getBehaviors(const std::type_index& type) const;
	virtual Behavior* getFirstBehavior(const std::type_index& type) const;
	virtual bool hasBehavior(const std::type_index& type) const;
	virtual void removeAllBehaviors(const std::type_index& type);
	virtual void removeBehavior(Behavior* behavior);
	virtual std::type_index getTypeIndex() const = 0;

	template<typename T = Behavior, typename... TArgs>
	requires std::is_base_of_v<Behavior, T> && std::is_constructible_v<T, BehaviorStorage*, TArgs...>
	T& addBehavior(TArgs&&... args) {
		auto unique = std::unique_ptr<T>(new T(this, std::forward<TArgs>(args)...));
		return static_cast<T&>(_addBehavior(std::move(unique)));
	}

	template<typename T = Behavior>
	requires std::is_base_of_v<Behavior, T>
	T& addBehavior(std::unique_ptr<T>&& behavior) {
		return static_cast<T&>(_addBehavior(std::move(behavior)));
	}

	template<typename T = Behavior>
	requires std::is_base_of_v<Behavior, T>
	std::vector<T*> getBehaviors() const {
		std::vector<T*> behaviors;
		for (const auto& behavior : getBehaviors(typeid(T))) {
			if (auto ptr = dynamic_cast<T*>(behavior))
				behaviors.push_back(ptr);
			else
				AssertFail("Failed to cast '{}' to '{}'.", behavior->getTypeIndex().name(), typeid(T).name());
		}
		return behaviors;
	}

	template<typename T = Behavior>
	requires std::is_base_of_v<Behavior, T>
	T* getFirstBehavior() const {
		if (auto behavior = getFirstBehavior(typeid(T))) {
			if (auto ptr = dynamic_cast<T*>(behavior))
				return ptr;
			else
				AssertFail("Failed to cast '{}' to '{}'.", behavior->getTypeIndex().name(), typeid(T).name());
		}
		return nullptr;
	}

	template<typename T = Behavior>
	requires std::is_base_of_v<Behavior, T>
	bool hasBehavior() const {
		return hasBehavior(typeid(T));
	}

	template<typename T = Behavior>
	requires std::is_base_of_v<Behavior, T>
	void removeAllBehaviors() {
		removeAllBehaviors(typeid(T));
	}

	template<typename T = Behavior>
	requires std::is_base_of_v<Behavior, T>
	void removeBehavior(T* behavior) {
		removeBehavior(static_cast<Behavior*>(behavior));
	}

	static ItemBehaviorStorage* getForItem(Item& item);
	static const ItemBehaviorStorage* getForItem(const Item& item);
};