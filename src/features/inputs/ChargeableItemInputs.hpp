#pragma once
#include "amethyst/runtime/input/InputManager.hpp"
#include "amethyst/runtime/events/InputEvents.hpp"
#include <tuple>

class ChargeableItemBehavior;
class PlayerInventory;
class ItemStack;
class ChargeableItemInputs {
public:
	static void Initialize(RegisterInputsEvent& e, AmethystContext& ctx);
	static std::tuple<const ItemStack*, ChargeableItemBehavior*, PlayerInventory*> GetMainHandChargeableItem(ClientInstance& client);
	static Amethyst::InputPassthrough ChargeItem(FocusImpact focus, ClientInstance& client);
	static Amethyst::InputPassthrough UnchargeItem(FocusImpact focus, ClientInstance& client);
};