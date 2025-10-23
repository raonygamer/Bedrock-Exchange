#pragma once
#include "features/behaviors/items/ItemBehavior.hpp"

class ItemStackBase;
class ChargeableItem :
	public ItemBehavior 
{
public:
	class SetChargeEvent :
		public Behavior::Event
	{
	public:
		ItemStackBase& mStack;
		short mLastCharge;
		short mNewCharge;

		SetChargeEvent(BehaviorStorage& storage, Behavior& sender, ItemStackBase& stack, short lastCharge, short newCharge);
		std::type_index getTypeIndex() const override;
		short delta() const;
	};

	const short mMaxCharge;
	const short mChargeSteps;
	const short mChargePerStep;
	const short mStartingCharge = 0;

	ChargeableItem(BehaviorStorage* owner, short maxCharge, short steps, short startingCharge = 0, bool shouldPlaySound = true);
	virtual std::string getBehaviorName() const override;
	virtual bool isSinglePerStorage() const override;
	virtual std::type_index getTypeIndex() const override;
	
	void setCharge(ItemStackBase& stack, short charge);
	short getCharge(const ItemStackBase& stack) const;
	void charge(ItemStackBase& stack);
	void uncharge(ItemStackBase& stack);
	void playChargeSound(short charge);
	void playUnchargeSound(short charge);
};