#pragma once
#include <amethyst/Log.hpp>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <functional>

class BehaviorStorage;
class Behavior {
public:
	class Event {
	public:
		BehaviorStorage& mStorage;
		Behavior& mSender;

		explicit Event(BehaviorStorage& storage, Behavior& sender);
		virtual ~Event() = default;
		virtual std::type_index getTypeIndex() const = 0;

		template<typename T = Event>
		requires std::is_base_of_v<Event, T>
		T* as() {
			T* ptr = nullptr;
			if (typeid(T) == getTypeIndex() && ptr = dynamic_cast<T*>(this))
				return ptr;
			return nullptr;
		}
	};

	BehaviorStorage* mOwner;
	std::unordered_map<std::type_index, std::vector<std::function<bool(Event*)>>> mEventListeners = {};

	explicit Behavior(BehaviorStorage* owner);
	virtual ~Behavior() = default;
	virtual std::string getBehaviorName() const = 0;
	virtual bool isSinglePerStorage() const = 0;
	virtual std::type_index getTypeIndex() const = 0;

	template<typename T = Event>
	void triggerEvent(T* event) {
		auto type = event->getTypeIndex();
		if (mEventListeners.contains(type)) {
			for (const auto& listener : mEventListeners[type]) {
				auto handled = listener(event);
				if (handled)
					break;
			}
		}
	}

	template<typename T = Event>
	void addEventListener(const std::function<bool(T*)>& listener) {
		auto& type = typeid(T);
		mEventListeners[type].push_back([listener](Event* event) -> bool {
			if (!event)
				AssertFail("Event pointer was null.");
			if (auto castedEvent = dynamic_cast<T*>(event))
				return listener(castedEvent);
			AssertFail("Failed to cast event '{}' to '{}'.", event->getTypeIndex().name(), typeid(T).name());
		});
	}
};