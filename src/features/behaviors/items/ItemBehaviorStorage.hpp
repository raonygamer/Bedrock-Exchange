#pragma once
#include "features/behaviors/BehaviorStorage.hpp"
#include "mc/src/common/world/item/Item.hpp"

class ItemBehaviorStorage : 
	public BehaviorStorage
{
public:
	static const HashedString STORAGE_TAG;
	Item* mOwner;

	explicit ItemBehaviorStorage(Item* owner);
	virtual std::type_index getTypeIndex() const override;

	Item* getOwner() const;

	static ItemBehaviorStorage* tryGetStorage(const ItemStackBase& stack);
	static ItemBehaviorStorage* tryGetStorage(Item& item);
	static const ItemBehaviorStorage* tryGetStorage(const Item& item);
};