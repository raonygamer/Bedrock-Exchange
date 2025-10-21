#pragma once
#include "amethyst/runtime/networking/CustomPacket.hpp"

class SwitchItemModePacketHandler : public Amethyst::CustomPacketHandler {
public:
	virtual void handle(const NetworkIdentifier& networkId, NetEventCallback& netEvent, const Amethyst::CustomPacket& _packet) const override;
};