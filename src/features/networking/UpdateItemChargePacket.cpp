#include "features/networking/UpdateItemChargePacket.hpp"

UpdateItemChargePacket::UpdateItemChargePacket(bool charge) :
	mCharge(charge) {
}

std::string UpdateItemChargePacket::getName() const {
	return "UpdateItemChargePacket";
}

void UpdateItemChargePacket::write(BinaryStream& out) {
	out.write(mCharge);
}

Bedrock::Result<void, std::error_code> UpdateItemChargePacket::read(ReadOnlyBinaryStream& in) {
	auto chargeResult = in.get<bool>();
	if (!chargeResult.has_value())
		AssertFail("UpdateItemChargePacket::read: Failed to read mCharge from stream.");
	mCharge = chargeResult.value();
	return Bedrock::Result<void, std::error_code>();
}