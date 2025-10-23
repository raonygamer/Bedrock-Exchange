#include "features/hooks/Hooks.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"
#include "features/containers/managers/models/AlchemicalBagManagerModel.hpp"
#include "features/ModGlobals.hpp"
#include "features/emc/EMCRepository.hpp"
#include "features/emc/EMCUtils.hpp"
#include "features/items/Items.hpp"
#include "features/items/MatterPickaxe.hpp"
#include "features/items/MatterAxe.hpp"
#include "features/items/MatterShovel.hpp"
#include "features/behaviors/items/types/ChargeableItem.hpp"
#include "features/behaviors/items/types/ModeItem.hpp"
#include "features/blocks/Blocks.hpp"
#include "features/blocks/actor/AlchemicalChestBlockActor.hpp"
#include "features/utility/BlockUtils.hpp"

#include "amethyst/runtime/AmethystContext.hpp"
#include "amethyst/runtime/ModContext.hpp"
#include "amethyst/runtime/utility/InlineHook.hpp"
#include "amethyst/runtime/events/ModEvents.hpp"

#include "mc/src/common/world/containers/models/ContainerModel.hpp"
#include "mc/src/common/world/actor/player/Player.hpp"
#include "mc/src/common/world/containers/ContainerValidation.hpp"
#include "mc/src/common/world/inventory/network/SparseContainer.hpp"
#include "mc/src/common/world/inventory/simulation/validation/ContainerScreenValidation.hpp"
#include "mc/src/common/world/inventory/network/ItemStackNetManagerServer.hpp"
#include "mc/src/common/world/inventory/network/ItemStackRequestActionHandler.hpp"
#include "mc/src/common/world/inventory/simulation/ContainerValidatorFactory.hpp"
#include "mc/src/common/world/item/ItemStack.hpp"
#include "mc/src/common/world/item/crafting/Recipes.hpp"
#include "mc/src/common/world/item/alchemy/PotionBrewing.hpp"
#include "mc/src/common/world/level/Level.hpp"
#include "mc/src/common/world/item/registry/TrimMaterialRegistry.hpp"
#include "mc/src/common/world/item/registry/TrimPatternRegistry.hpp"
#include "mc/src/common/world/level/BlockSource.hpp"
#include "mc/src/common/world/level/block/Block.hpp"
#include "mc/src/common/world/inventory/transaction/ItemUseInventoryTransaction.hpp"
#include "mc/src/common/world/gamemode/GameMode.hpp"
#include "mc/src/common/world/item/registry/CreativeItemRegistry.hpp"
#include "mc/src/common/world/item/registry/CreativeGroupInfo.hpp"
#include "mc/src/common/Minecraft.hpp"
#include "mc/src-client/common/client/renderer/blockActor/ChestRenderer.hpp"
#include "mc/src-client/common/client/gui/screens/controllers/HudScreenController.hpp"
#include "mc/src-client/common/client/player/LocalPlayer.hpp"
#include "mc/src-client/common/client/gui/controls/UIPropertyBag.hpp"
#include "mc/src-client/common/client/gui/screens/InGamePlayScreen.hpp"
#include "mc/src-client/common/client/renderer/game/LevelRenderer.hpp"
#include "mc/src-client/common/client/renderer/game/LevelRendererPlayer.hpp"
#include "mc/src/common/world/item/VanillaItems.hpp"
#include "mc/src-deps/core/math/Color.hpp"

using namespace ee2::emc;

Amethyst::InlineHook<decltype(&Player::$constructor)> _Player_$constructor;
Amethyst::InlineHook<decltype(&ContainerWeakRef::tryGetActorContainer)> _ContainerWeakRef_tryGetActorContainer;
Amethyst::InlineHook<decltype(&Player::readAdditionalSaveData)> _Player_readAdditionalSaveData;
Amethyst::InlineHook<decltype(&Player::addAdditionalSaveData)> _Player_addAdditionalSaveData;
Amethyst::InlineHook<decltype(&Recipes::init)> _Recipes_init;
Amethyst::InlineHook<decltype(&ContainerValidatorFactory::getBackingContainer)> _ContainerValidatorFactory_getBackingContainer;
Amethyst::InlineHook<decltype(&BlockActorFactory::createBlockEntity)> _BlockActorFactory_createBlockEntity;
Amethyst::InlineHook<decltype(&Item::appendFormattedHovertext)> _Item_appendFormattedHovertext;
Amethyst::InlineHook<decltype(&VanillaItems::_addItemsCategory)> _VanillaItems__addItemsCategory;
Amethyst::InlineHook<decltype(&VanillaItems::_addEquipmentCategory)> _VanillaItems__addEquipmentCategory;
Amethyst::InlineHook<decltype(&ContainerScreenController::_registerBindings)> _ContainerScreenController__registerBindings;
Amethyst::InlineHook<decltype(&HudScreenController::$constructor)> _HudScreenController_$constructor;
Amethyst::InlineHook<decltype(Amethyst::OverloadCast<bool(HudScreenController::*)(const std::string&, uint32_t, int, const std::string&, uint32_t, const std::string&, UIPropertyBag&)>(&HudScreenController::bind))> _HudScreenController_bind;
Amethyst::InlineHook<decltype(&InGamePlayScreen::_renderLevel)> _InGamePlayScreen__renderLevel;
Amethyst::InlineHook<decltype(&GameMode::destroyBlock)> _GameMode_destroyBlock;

void Player_readAdditionalSaveData(Player* self, const CompoundTag& tag, DataLoadHelper& helper) {
    _Player_readAdditionalSaveData(self, tag, helper);
}

void Player_addAdditionalSaveData(Player* self, CompoundTag& tag) {
    _Player_addAdditionalSaveData(self, tag);
}


void Player_$constructor(Player* self, void* a2, void* a3, void* a4, void* a5, void* a6, void* a7, void* a8, void* a9, void* a10, void* a11, void* a12, void* a13, void* a14) {
	_Player_$constructor(self, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);

    if (!self->isClientSide() && !_Player_readAdditionalSaveData) {
        auto& ctx = Amethyst::GetContext();
        ctx.mHookManager->CreateHookAbsolute(
			_Player_readAdditionalSaveData,
            reinterpret_cast<uintptr_t*>(GetVtable(self))[147],
			&Player_readAdditionalSaveData
		);

        ctx.mHookManager->CreateHookAbsolute(
            _Player_addAdditionalSaveData,
            reinterpret_cast<uintptr_t*>(GetVtable(self))[148],
            &Player_addAdditionalSaveData
        );
    }

    if (!self->tryGetComponent<AlchemicalBagContainerComponent>()) {
		auto& component = self->addComponent<AlchemicalBagContainerComponent>();
        auto& colors = ModGlobals::AlchemicalBagColors;
        for (int i = 0; i < colors.size(); i++) {
            auto container = std::make_unique<AlchemicalBagContainer>(self, colors[i], i);
            container->init();
			component.mContainers[i] = std::move(container);
		}
	}
}

Container* ContainerWeakRef_tryGetActorContainer(Actor& actor, ActorContainerType type) {
    if (type == CustomActorContainerType::AlchemicalBag) {
        auto* component = actor.tryGetComponent<AlchemicalBagContainerComponent>();
        if (component) {
            return component->mContainers[0].get();
        }
    }
    return _ContainerWeakRef_tryGetActorContainer(actor, type);
}

Container* ContainerValidatorFactory_getBackingContainer(ContainerEnumName name, const ContainerScreenContext& ctx) {
    if (ctx.mScreenContainerType == ContainerType::CONTAINER && name == ContainerEnumName::ContainerItems)
    {
        if (ctx.mOwner.index() == 1) {
            auto& actorId = std::get<1>(ctx.mOwner);
            if (actorId == ctx.mPlayer->getUniqueID()) {
                auto* component = ctx.mPlayer->tryGetComponent<AlchemicalBagContainerComponent>();
                if (component) {
                    return component->mContainers[0].get();
                }
            }
		}
	}
    return _ContainerValidatorFactory_getBackingContainer(name, ctx);
}

struct dot_separator : std::numpunct<char> {
protected:
    char do_thousands_sep() const override { return '.'; }
    std::string do_grouping() const override { return "\3"; }
};

void Item_appendFormattedHovertext(const Item* self, const ItemStackBase& stack, Level& level, std::string& hovertext, bool showCategory)
{
    _Item_appendFormattedHovertext(self, stack, level, hovertext, showCategory);

    uint64_t emc = EMCUtils::calculateItemEMC(
        EMCRepository::getItemEMC(ItemID::fromStack(stack)).emc,
        stack,
        level
    );

	if (emc == 0)
		return;
	uint64_t stackEMC = EMCUtils::calculateStackEMC(emc, stack);

	std::stringstream ss;
    ss.imbue(std::locale(ss.getloc(), new dot_separator));
	ss << emc;
    hovertext += std::format("\n§eEMC: §f{}§r", ss.str());
    if (stack.mCount > 1) {
        ss.str("");
        ss.clear();
		ss << stackEMC;
        hovertext += std::format("\n§eStack EMC: §f{}§r", ss.str());
	}
}

void Recipes_init(Recipes* self, ResourcePackManager& rpm, ExternalRecipeStore& ers, const BaseGameVersion& bgv, const Experiments& exp) {
    _Recipes_init(self, rpm, ers, bgv, exp);
    Log::Info("Mapping EMC values...");
	ee2::emc::EMCRepository::init(*self);
}

std::shared_ptr<BlockActor> BlockActorFactory_createBlockEntity(BlockActorType type, const BlockPos& pos, const BlockLegacy& block) {
    if (type == CustomBlockActorType::AlchemicalChest) {
		return std::make_shared<AlchemicalChestBlockActor>(pos);
    }
    return _BlockActorFactory_createBlockEntity(type, pos, block);
}

void VanillaItems__addItemsCategory(CreativeItemRegistry* creativeItemRegistry, ItemRegistryRef registry, const BaseGameVersion& version, const Experiments& experiments) {
    _VanillaItems__addItemsCategory(creativeItemRegistry, registry, version, experiments);
    Item::addCreativeItem(registry, *Blocks::AlchemicalCoalBlock->mDefaultState);
    Item::addCreativeItem(registry, *Blocks::MobiusFuelBlock->mDefaultState);
    Item::addCreativeItem(registry, *Blocks::AeternalisFuelBlock->mDefaultState);
    Item::addCreativeItem(registry, *Blocks::AlchemicalChestBlock->mDefaultState);
    Item::addCreativeItem(registry, *Blocks::DarkMatterBlock->mDefaultState);
    Item::addCreativeItem(registry, *Blocks::RedMatterBlock->mDefaultState);
    Item::addCreativeItem(registry, *Blocks::DarkMatterFurnaceBlock->mDefaultState);
}

void ContainerScreenController__registerBindings(ContainerScreenController* self) {
    _ContainerScreenController__registerBindings(self);
    self->bindBoolForAnyCollection("#item_charge_visible", [self](const std::string& collection, int index) {
        const ItemStackBase& stack = self->_getVisualItemStack(collection, index);
		auto* behavior = ChargeableItem::tryGet(stack);
        if (!behavior)
            return false;
        return true;
    }, [&](const std::string&, int) { 
        return true; 
    });


    self->bindBool("#selected_item_charge_visible", [self]() {
        const ItemGroup& cursorItemGroup = self->mMinecraftScreenModel->getCursorSelectedItemGroup();
        const ItemStackBase& stack = cursorItemGroup.getItemInstance();
		auto* behavior = ChargeableItem::tryGet(stack);
        if (behavior == nullptr)
            return false;
        return true;
    }, [] {
        return true;
    });


    self->bindFloatForAnyCollection("#item_charge_current_amount", [self](const std::string& collection, int index) {
        const ItemStackBase& stack = self->_getVisualItemStack(collection, index);
		auto* behavior = ChargeableItem::tryGet(stack);
        if (!behavior)
            return 0.0f;
        return static_cast<float>(behavior->getCharge(stack));
    }, [&](const std::string&, int) {
        return true;
    });

    self->bindFloat("#selected_item_charge_current_amount", [self]() {
        const ItemGroup& cursorItemGroup = self->mMinecraftScreenModel->getCursorSelectedItemGroup();
        const ItemStackBase& stack = cursorItemGroup.getItemInstance();
		auto* behavior = ChargeableItem::tryGet(stack);
        if (!behavior)
            return 0.0f;
        return static_cast<float>(behavior->getCharge(stack));
    }, [] {
        return true;
    });

    self->bindFloatForAnyCollection("#item_charge_total_amount", [self](const std::string& collection, int index) {
        const ItemStackBase& stack = self->_getVisualItemStack(collection, index);
		auto* behavior = ChargeableItem::tryGet(stack);
        if (!behavior)
            return 0.0f;
        return static_cast<float>(behavior->mMaxCharge);
    }, [&](const std::string&, int) {
        return true;
    });

    self->bindFloat("#selected_item_charge_total_amount", [self]() {
        const ItemGroup& cursorItemGroup = self->mMinecraftScreenModel->getCursorSelectedItemGroup();
        const ItemStackBase& stack = cursorItemGroup.getItemInstance();
		auto* behavior = ChargeableItem::tryGet(stack);
        if (!behavior)
            return 0.0f;
        return static_cast<float>(behavior->mMaxCharge);
    }, [] {
        return true;
    });
}

bool HudScreenController_bind(
    HudScreenController* self, 
    const std::string& collectionName, 
    uint32_t collectionNameHash, 
    int collectionIndex, 
    const std::string& bindingName, 
    uint32_t bindingNameHash, 
    const std::string& bindingNameOverride, 
    UIPropertyBag& bag) 
{
    static uint32_t itemChargeVisibleHash = StringToNameId("#item_charge_visible");
    static uint32_t itemChargeCurrentHash = StringToNameId("#item_charge_current_amount");
    static uint32_t itemChargeTotalHash = StringToNameId("#item_charge_total_amount");

    if (bindingNameHash == itemChargeVisibleHash) {
        const ItemStack& stack = self->mHudScreenManagerController->getItemStack(collectionName, collectionIndex);
		auto* behavior = ChargeableItem::tryGet(stack);
		bool value = false;
        if (behavior)
            value = true;
        bag.set<bool>(bindingNameOverride, value);
        return true;
    }
    else if (bindingNameHash == itemChargeCurrentHash) {
        const ItemStack& stack = self->mHudScreenManagerController->getItemStack(collectionName, collectionIndex);
		auto* behavior = ChargeableItem::tryGet(stack);
		float value = 0.0f;
        if (behavior)
			value = static_cast<float>(behavior->getCharge(stack));
        bag.set<float>(bindingNameOverride, value);
		return true;
    }
    else if (bindingNameHash == itemChargeTotalHash) {
        const ItemStack& stack = self->mHudScreenManagerController->getItemStack(collectionName, collectionIndex);
		auto* behavior = ChargeableItem::tryGet(stack);
		float value = 0.0f;
		if (behavior)
            value = static_cast<float>(behavior->mMaxCharge);
		bag.set<float>(bindingNameOverride, value);
		return true;
    }

    return _HudScreenController_bind(
        self, 
        collectionName, 
        collectionNameHash, 
        collectionIndex, 
        bindingName, 
        bindingNameHash, 
        bindingNameOverride,
        bag
    );
}

void InGamePlayScreen__renderLevel(InGamePlayScreen* self, ScreenContext& screenContext, const FrameRenderObject& renderObj) {
	_InGamePlayScreen__renderLevel(self, screenContext, renderObj);

	auto& client = *Amethyst::GetClientCtx().mClientInstance;
	auto& game = *client.mMinecraftGame;
	auto& region = *client.getRegion();
	auto& player = *client.getLocalPlayer();
	auto& hitRes = player.getLevel()->getHitResult();
	if (hitRes.mType == HitResultType::NO_HIT || hitRes.mIsHitLiquid)
		return;

	BaseActorRenderContext ctx(screenContext, client, game);
	auto& levelRendererPlayer = *client.mLevelRenderer->mLevelRendererPlayer;
	PlayerInventory& inventory = player.getSupplies();
	const ItemStack& stack = inventory.getSelectedItem();
	if (stack.isNull())
		return;

	std::vector<std::pair<BlockPos, const Block*>> blocks;
	if (stack.mItem == Items::PhilosophersStone) {
		auto* behavior = ChargeableItem::tryGet(stack);
		if (!behavior)
			return;

		int charge = behavior->getCharge(stack);
		int radius = std::clamp(charge, 0, 12);
		int maxBlocks = std::min(1000, (2 * radius + 1) * (2 * radius + 1) * (2 * radius + 1));
		
		blocks = BlockUtils::floodFillBlocks(region, hitRes.mBlock, region.getBlock(hitRes.mBlock).mLegacyBlock, radius, maxBlocks);
		for (const auto& [pos, block] : blocks) {
			levelRendererPlayer._renderHighlightSelection(ctx, region, *block, pos, false, true);
			levelRendererPlayer._renderOutlineSelection(screenContext, *block, region, pos);
		}
		return;
	}
	else if (stack.mItem == Items::DarkMatterPickaxe) {
		MatterPickaxe* item = static_cast<MatterPickaxe*>(stack.mItem.get());
		auto* chargeBehavior = ChargeableItem::tryGet(stack);
		auto* modeBehavior = ModeItem::tryGet(stack);
		if (!chargeBehavior || !modeBehavior)
			return;

		Directions dirs = Directions::fromLookVec3(player.getHeadLookVector(1.0f));
		int charge = chargeBehavior->getCharge(stack);
		if (charge < chargeBehavior->mMaxCharge)
			return;
		blocks = item->getBlocksForMode(stack, region, hitRes.mBlock, dirs);
	}
	else if (stack.mItem == Items::DarkMatterAxe) {
		MatterAxe* item = static_cast<MatterAxe*>(stack.mItem.get());
		auto* chargeBehavior = ChargeableItem::tryGet(stack);
		auto* modeBehavior = ModeItem::tryGet(stack);
		if (!chargeBehavior || !modeBehavior)
			return;

		Directions dirs = Directions::fromLookVec3(player.getHeadLookVector(1.0f));
		int charge = chargeBehavior->getCharge(stack);
		if (charge < chargeBehavior->mMaxCharge)
			return;
		blocks = item->getBlocksForMode(stack, region, hitRes.mBlock, dirs);
	}
	else if (stack.mItem == Items::DarkMatterShovel) {
		MatterShovel* item = static_cast<MatterShovel*>(stack.mItem.get());
		auto* chargeBehavior = ChargeableItem::tryGet(stack);
		auto* modeBehavior = ModeItem::tryGet(stack);
		if (!chargeBehavior || !modeBehavior)
			return;

		Directions dirs = Directions::fromLookVec3(player.getHeadLookVector(1.0f));
		int charge = chargeBehavior->getCharge(stack);
		if (charge < chargeBehavior->mMaxCharge)
			return;
		blocks = item->getBlocksForMode(stack, region, hitRes.mBlock, dirs);
	}

	for (const auto& [pos, block] : blocks) {
		if (pos == hitRes.mBlock)
			continue;
		levelRendererPlayer.renderHitSelect(ctx, region, pos, true);
	}
}

bool GameMode_destroyBlock(GameMode* self, const BlockPos& pos, FacingID face) {
	PlayerInventory& inventory = self->mPlayer.getSupplies();
	const ItemStack& stack = inventory.getSelectedItem();

	static HashedString matterPickaxeTag("ee2:dark_matter_pickaxe");
	static HashedString matterAxeTag("ee2:dark_matter_axe");
	static HashedString matterShovelTag("ee2:dark_matter_shovel");
	if (stack.isNull())
		return _GameMode_destroyBlock(self, pos, face);

	Directions dirs = Directions::fromLookVec3(self->mPlayer.getHeadLookVector(1.0f));
	auto& region = self->mPlayer.getDimensionBlockSource();
	std::vector<std::pair<BlockPos, const Block*>> blocks;
	auto* chargeBehavior = ChargeableItem::tryGet(stack);
	if (!chargeBehavior)
		return _GameMode_destroyBlock(self, pos, face);

	if (stack.mItem.get()->mFullName == matterPickaxeTag) {
		MatterPickaxe* item = static_cast<MatterPickaxe*>(stack.mItem.get());
		if (chargeBehavior->getCharge(stack) < chargeBehavior->mMaxCharge)
			return _GameMode_destroyBlock(self, pos, face);
		blocks = item->getBlocksForMode(stack, region, pos, dirs);
	}
	else if (stack.mItem.get()->mFullName == matterAxeTag) {
		MatterAxe* item = static_cast<MatterAxe*>(stack.mItem.get());
		if (chargeBehavior->getCharge(stack) < chargeBehavior->mMaxCharge)
			return _GameMode_destroyBlock(self, pos, face);
		blocks = item->getBlocksForMode(stack, region, pos, dirs);
	}
	else if (stack.mItem.get()->mFullName == matterShovelTag) {
		MatterShovel* item = static_cast<MatterShovel*>(stack.mItem.get());
		if (chargeBehavior->getCharge(stack) < chargeBehavior->mMaxCharge)
			return _GameMode_destroyBlock(self, pos, face);
		blocks = item->getBlocksForMode(stack, region, pos, dirs);
	}
	else {
		return _GameMode_destroyBlock(self, pos, face);
	}


	for (const auto& [blockPos, block] : blocks) {
		if (blockPos == pos)
			continue;
		_GameMode_destroyBlock(self, blockPos, face);
	}
	return _GameMode_destroyBlock(self, pos, face);
}

SafetyHookMid _Actor_calculateAttackDamage_additionalDmgExt;
void Actor_calculateAttackDamage_additionalDmgExt(SafetyHookContext& ctx) {
	// ctx.rbp -> ItemStackBase*
	// Since we are pinned to 1.21.0.3 here it's stable enough
	const ItemStackBase* stack = reinterpret_cast<const ItemStackBase*>(ctx.rbp);
	if (stack->isNull() || !stack->mUserData)
		return;
	auto* tag = stack->mUserData->get("AdditionalAttackDamage");
	if (!tag || tag->getId() != Tag::Type::Int)
		return;

	ctx.rax += static_cast<IntTag*>(tag)->data;
}

void CreateAllHooks(AmethystContext& ctx) {
    auto& hooks = *ctx.mHookManager;
    HOOK(Player, $constructor);
    HOOK(ContainerWeakRef, tryGetActorContainer);
	HOOK(ContainerValidatorFactory, getBackingContainer);
	HOOK(Recipes, init);
	HOOK(BlockActorFactory, createBlockEntity);
	HOOK(VanillaItems, _addItemsCategory);
    HOOK(ContainerScreenController, _registerBindings);
	HOOK(InGamePlayScreen, _renderLevel);

    VHOOK(Item, appendFormattedHovertext, this);
	VHOOK(GameMode, destroyBlock, this);

	// Extension to support tag "AdditionalAttackDamage" on ItemStack NBT
	{
		auto address = SigScanSafe("48 8B 15 ? ? ? ? 48 8B CF 66 0F 6E C0");
		if (!address)
			AssertFail("Failed to find signature for Actor_calculateAttackDamage_additionalDmgExt");
		_Actor_calculateAttackDamage_additionalDmgExt = safetyhook::create_mid(*address, &Actor_calculateAttackDamage_additionalDmgExt);
	}
	
	using BindFn = bool(HudScreenController::*)(
        const std::string&,
        uint32_t,
        int,
        const std::string&,
        uint32_t,
        const std::string&,
        UIPropertyBag&
    );

	auto bindFnAddr = GetVirtualFunctionOffset<static_cast<BindFn>(&HudScreenController::bind)>() / sizeof(void*) + 1;
    hooks.CreateHookAbsolute(
        _HudScreenController_bind,
        reinterpret_cast<uintptr_t*>(HudScreenController::$vtable_for_this)[bindFnAddr],
        &HudScreenController_bind
	);

	// Cleanup manual hooks on mod shutdown
	ctx.mEventBus->AddListener<BeforeModShutdownEvent>([](BeforeModShutdownEvent& event) {
		_Actor_calculateAttackDamage_additionalDmgExt.reset();
	});
}
