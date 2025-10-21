#include "features/networking/SwitchItemModePacket.hpp"

SwitchItemModePacket::SwitchItemModePacket(size_t mode) :
	mMode(mode) {
}

std::string SwitchItemModePacket::getName() const {
	return "SwitchItemModePacket";
}

void SwitchItemModePacket::write(BinaryStream& out) {
	out.write(mMode);
}

Bedrock::Result<void, std::error_code> SwitchItemModePacket::read(ReadOnlyBinaryStream& in) {
	auto modeResult = in.get<size_t>();
	if (!modeResult.has_value())
		AssertFail("SwitchItemModePacket::read: Failed to read mMode from stream.");
	mMode = modeResult.value();
	return Bedrock::Result<void, std::error_code>();
}