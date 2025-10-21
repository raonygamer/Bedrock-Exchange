#pragma once
#include "amethyst/runtime/networking/CustomPacket.hpp"

class SwitchItemModePacket : public Amethyst::CustomPacket {
public:
	size_t mMode = 0;

	SwitchItemModePacket() = default;
	SwitchItemModePacket(size_t mode);

	virtual std::string getName() const;
	virtual void write(BinaryStream& out);
	virtual Bedrock::Result<void, std::error_code> read(ReadOnlyBinaryStream& in);
};