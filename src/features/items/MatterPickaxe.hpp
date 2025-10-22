#pragma once
#include "mc/src/common/world/item/PickaxeItem.hpp"
#include "features/items/behaviors/ChargeableItemBehavior.hpp"
#include "features/items/behaviors/ModeItemBehavior.hpp"

struct Directions {
	Vec3 mUp;
	Vec3 mRight;
	Vec3 mForward;

	inline static Directions fromLookVec3(const Vec3& lookDir) {
		Vec3 forward = lookDir.normalized();
		float ax = std::abs(forward.x);
		float ay = std::abs(forward.y);
		float az = std::abs(forward.z);

		enum Axis {
			X, Y, Z
		};
		Axis longAxis = (ax > ay && ax > az) ? X : (ay > az ? Y : Z);
		Vec3 forwardVec(0, 0, 0);
		Vec3 rightVec(0, 0, 0);
		Vec3 upVec(0, 0, 0);

		switch (longAxis) {
		case X:
			forwardVec.x = (forward.x > 0 ? 1 : -1);
			rightVec.z = 1;
			upVec.y = 1;
			break;
		case Y:
			forwardVec.y = (forward.y > 0 ? 1 : -1);
			rightVec.x = 1;
			upVec.z = 1;
			break;
		case Z:
			forwardVec.z = (forward.z > 0 ? 1 : -1);
			rightVec.x = 1;
			upVec.y = 1;
			break;
		}

		return Directions{
			.mUp = upVec,
			.mRight = rightVec,
			.mForward = forwardVec
		};
	}
};

class MatterPickaxe :
	public PickaxeItem,
	public ChargeableItemBehavior,
	public ModeItemBehavior
{
public:
	MatterPickaxe(
		const std::string& name, 
		short id, 
		const Item::Tier& tier, 
		short maxCharge, 
		short steps, 
		short startingCharge,
		const std::vector<std::string>& modes, 
		size_t defaultMode
	);

	virtual bool isDamageable() const override;
	virtual void appendFormattedHovertext(const ItemStackBase& stack, Level& level, std::string& outText, bool showCategory) const override;
	virtual std::vector<std::pair<BlockPos, const Block*>> getBlocksForMode(const ItemStackBase& stack, BlockSource& region, const BlockPos& center, const Directions& directions);
};