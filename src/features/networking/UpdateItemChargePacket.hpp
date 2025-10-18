#pragma once
#include "amethyst/runtime/networking/CustomPacket.hpp"

class UpdateItemChargePacket : public Amethyst::CustomPacket {
public:
	bool mCharge = false;

	UpdateItemChargePacket() = default;
	UpdateItemChargePacket(bool charge);

	virtual std::string getName() const;
	virtual void write(BinaryStream& out);
	virtual Bedrock::Result<void, std::error_code> read(ReadOnlyBinaryStream& in);
};