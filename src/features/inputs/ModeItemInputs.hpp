#pragma once
#include "amethyst/runtime/input/InputManager.hpp"
#include "amethyst/runtime/events/InputEvents.hpp"
#include <tuple>

class ModeItemBehavior;
class PlayerInventory;
class ItemStack;
class ModeItemInputs {
public:
	static void Initialize(RegisterInputsEvent& e, AmethystContext& ctx);
	static std::tuple<const ItemStack*, ModeItemBehavior*, PlayerInventory*> GetMainHandModeItem(ClientInstance& client);
	static Amethyst::InputPassthrough SwitchToNextMode(FocusImpact focus, ClientInstance& client);
};