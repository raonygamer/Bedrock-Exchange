#pragma once
#include "amethyst/runtime/networking/CustomPacket.hpp"

class UpdateItemChargePacketHandler : public Amethyst::CustomPacketHandler {
public:
	virtual void handle(const NetworkIdentifier& networkId, NetEventCallback& netEvent, const Amethyst::CustomPacket& _packet) const override;
};