#include "features/hooks/Hooks.hpp"
#include "features/components/AlchemicalBagContainerComponent.hpp"
#include "features/containers/managers/models/AlchemicalBagManagerModel.hpp"
#include "features/ModGlobals.hpp"
#include "features/emc/EMCRepository.hpp"
#include "features/emc/EMCUtils.hpp"
#include "features/items/Items.hpp"
#include "features/items/ChargeableItem.hpp"
#include "features/blocks/Blocks.hpp"
#include "features/blocks/actor/AlchemicalChestBlockActor.hpp"

#include "amethyst/runtime/AmethystContext.hpp"
#include "amethyst/runtime/ModContext.hpp"

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
#include "mc/src-client/common/client/renderer/blockActor/ChestRenderer.hpp"
#include "mc/src-client/common/client/gui/screens/controllers/HudScreenController.hpp"
#include "mc/src-client/common/client/gui/controls/UIPropertyBag.hpp"
#include "mc/src/common/world/item/VanillaItems.hpp"

using namespace ee2::emc;

extern ActorContainerType AlchemicalBagContainerType;
SafetyHookInline _Player_$constructor;
SafetyHookInline _ContainerWeakRef_tryGetActorContainer;
SafetyHookInline _Player_readAdditionalSaveData;
SafetyHookInline _Player_addAdditionalSaveData;
SafetyHookInline _Recipes_init;
SafetyHookInline _ContainerValidatorFactory_getBackingContainer;
SafetyHookInline _Item_appendFormattedHovertext;
SafetyHookInline _BlockActorFactory_createBlockEntity;
SafetyHookInline _VanillaItems__addItemsCategory;
SafetyHookInline _ContainerScreenController__registerBindings;
SafetyHookInline _HudScreenController_$constructor;
SafetyHookInline _HudScreenController_bind;

void Player_readAdditionalSaveData(Player* self, const CompoundTag& tag, DataLoadHelper& helper) {
    _Player_readAdditionalSaveData.thiscall<
        void,
		Player*,
		const CompoundTag&,
		DataLoadHelper&
    >(self, tag, helper);
}

void Player_addAdditionalSaveData(Player* self, CompoundTag& tag) {
    _Player_addAdditionalSaveData.thiscall<
        void,
        Player*,
        CompoundTag&
    >(self, tag);
}


void Player_$constructor(Player* self, void* a2, void* a3, void* a4, void* a5, void* a6, void* a7, void* a8, void* a9, void* a10, void* a11, void* a12, void* a13, void* a14) {
	_Player_$constructor.thiscall(self, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14);

    if (!self->isClientSide() && !_Player_readAdditionalSaveData.original<void*>() && !_Player_addAdditionalSaveData.original<void*>()) {
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
    if (type == AlchemicalBagContainerType) {
        auto* component = actor.tryGetComponent<AlchemicalBagContainerComponent>();
        if (component) {
            return component->mContainers[0].get();
        }
    }
    return _ContainerWeakRef_tryGetActorContainer.call<
        Container*,
        Actor&,
        ActorContainerType
    >(actor, type);
}

Container* ContainerValidatorFactory_getBackingContainer(ContainerEnumName name, const ContainerScreenContext& ctx) {
    if (ctx.mScreenContainerType == ContainerType::CONTAINER && name == ContainerEnumName::LevelEntityContainer)
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
    return _ContainerValidatorFactory_getBackingContainer.call<
        Container*, 
        ContainerEnumName, 
        const ContainerScreenContext&
    >(name, ctx);
}

struct dot_separator : std::numpunct<char> {
protected:
    char do_thousands_sep() const override { return '.'; }
    std::string do_grouping() const override { return "\3"; }
};

void Item_appendFormattedHovertext(const Item* self, const ItemStackBase& stack, Level& level, std::string& hovertext, bool showCategory)
{
    _Item_appendFormattedHovertext.thiscall<
        void,
        const Item*,
        const ItemStackBase&,
        Level&,
        std::string&,
        bool
    >(self, stack, level, hovertext, showCategory);

    uint64_t emc = EMCUtils::calculateItemEMC(
        EMCRepository::getItemEMC(ItemID::fromStack(stack)).emc,
        stack,
        level
    );

	if (emc == 0)
		return;
	uint64_t stackEMC = EMCUtils::calculateStackEMC(emc, stack);

	static bool first = true;
    static std::stringstream ss;
    if (first) {
        ss.imbue(std::locale(ss.getloc(), new dot_separator));
		first = false;
    }
    ss.str("");
	ss.clear();
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
    _Recipes_init.thiscall<
        void,
		Recipes*,
		ResourcePackManager&,
		ExternalRecipeStore&,
		const BaseGameVersion&,
		const Experiments&
    >(self, rpm, ers, bgv, exp);
    Log::Info("Mapping EMC values...");
	ee2::emc::EMCRepository::init(*self);
}

std::shared_ptr<BlockActor> BlockActorFactory_createBlockEntity(BlockActorType type, const BlockPos& pos, const BlockLegacy& block) {
    if (type == CustomBlockActorType::AlchemicalChest) {
		return std::make_shared<AlchemicalChestBlockActor>(pos);
    }
    return _BlockActorFactory_createBlockEntity.call<
        std::shared_ptr<BlockActor>,
        BlockActorType,
        const BlockPos&,
		const BlockLegacy&
    >(type, pos, block);

}

void VanillaItems__addItemsCategory(CreativeItemRegistry* creativeItemRegistry, ItemRegistryRef registry, const BaseGameVersion& version, const Experiments& experiments) {
    _VanillaItems__addItemsCategory.call<
        void,
        CreativeItemRegistry*,
        ItemRegistryRef,
		const BaseGameVersion&,
		const Experiments&
	>(creativeItemRegistry, registry, version, experiments);
    Item::addCreativeItem(registry, *Blocks::sBlocks["ee2:alchemical_chest"]->mDefaultState);
}

static const ChargeableItem* getChargeableItem(const ItemStackBase& stack) {
    if (stack.isNull())
        return nullptr;
    const Item* item = stack.getItem();
    if (!item->hasTag("ee2:chargeable_item"))
        return nullptr;
    return static_cast<const ChargeableItem*>(item);
};

void ContainerScreenController__registerBindings(ContainerScreenController* self) {
    _ContainerScreenController__registerBindings.thiscall(self);
    self->bindBoolForAnyCollection("#item_charge_visible", [self](const std::string& collection, int index) {
        const ItemStackBase& stack = self->_getVisualItemStack(collection, index);
        const ChargeableItem* chargeableItem = getChargeableItem(stack);
        if (!chargeableItem)
            return false;
        short currentCharge = chargeableItem->getCharge(stack);
        if (currentCharge == chargeableItem->mMaxCharge)
            return false;
        return true;
    }, [&](const std::string&, int) { 
        return true; 
    });

    self->bindBool("#selected_item_charge_visible", [self]() {
        const ItemGroup& cursorItemGroup = self->mMinecraftScreenModel->getCursorSelectedItemGroup();
        const ItemStackBase& stack = cursorItemGroup.getItemInstance();
        const ChargeableItem* chargeableItem = getChargeableItem(stack);
        if (!chargeableItem)
            return false;
        short currentCharge = chargeableItem->getCharge(stack);
        if (currentCharge == chargeableItem->mMaxCharge)
            return false;
        return true;
    }, [] {
        return true;
    });

    self->bindFloatForAnyCollection("#item_charge_current_amount", [self](const std::string& collection, int index) {
        const ItemStackBase& stack = self->_getVisualItemStack(collection, index);
        const ChargeableItem* chargeableItem = getChargeableItem(stack);
        if (!chargeableItem)
            return 0.0f;
        return static_cast<float>(chargeableItem->getCharge(stack));
    }, [&](const std::string&, int) {
        return true;
    });

    self->bindFloat("#selected_item_charge_current_amount", [self]() {
        const ItemGroup& cursorItemGroup = self->mMinecraftScreenModel->getCursorSelectedItemGroup();
        const ItemStackBase& stack = cursorItemGroup.getItemInstance();
        const ChargeableItem* chargeableItem = getChargeableItem(stack);
        if (!chargeableItem)
            return 0.0f;
        return static_cast<float>(chargeableItem->getCharge(stack));
    }, [] {
        return true;
    });

    self->bindFloatForAnyCollection("#item_charge_total_amount", [self](const std::string& collection, int index) {
        const ItemStackBase& stack = self->_getVisualItemStack(collection, index);
        const ChargeableItem* chargeableItem = getChargeableItem(stack);
        if (!chargeableItem)
            return 0.0f;
        return static_cast<float>(chargeableItem->mMaxCharge);
    }, [&](const std::string&, int) {
        return true;
    });

    self->bindFloat("#selected_item_charge_total_amount", [self]() {
        const ItemGroup& cursorItemGroup = self->mMinecraftScreenModel->getCursorSelectedItemGroup();
        const ItemStackBase& stack = cursorItemGroup.getItemInstance();
        const ChargeableItem* chargeableItem = getChargeableItem(stack);
        if (!chargeableItem)
            return 0.0f;
        return static_cast<float>(chargeableItem->mMaxCharge);
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
		const ChargeableItem* chargeableItem = getChargeableItem(stack);
		bool value = false;
        if (chargeableItem)
            value = chargeableItem->getCharge(stack) < chargeableItem->mMaxCharge;
        bag.set<bool>(bindingNameOverride, value);
        return true;
    }
    else if (bindingNameHash == itemChargeCurrentHash) {
        const ItemStack& stack = self->mHudScreenManagerController->getItemStack(collectionName, collectionIndex);
        const ChargeableItem* chargeableItem = getChargeableItem(stack);
		float value = 0.0f;
        if (chargeableItem)
			value = static_cast<float>(chargeableItem->getCharge(stack));
        bag.set<float>(bindingNameOverride, value);
		return true;
    }
    else if (bindingNameHash == itemChargeTotalHash) {
        const ItemStack& stack = self->mHudScreenManagerController->getItemStack(collectionName, collectionIndex);
        const ChargeableItem* chargeableItem = getChargeableItem(stack);
		float value = 0.0f;
		if (chargeableItem)
            value = static_cast<float>(chargeableItem->mMaxCharge);
		bag.set<float>(bindingNameOverride, value);
		return true;
    }

    return _HudScreenController_bind.thiscall<
        bool,
		HudScreenController*,
		const std::string&,
		uint32_t,
		int,
		const std::string&,
		uint32_t,
		const std::string&,
		UIPropertyBag&
	>(
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

void CreateAllHooks(AmethystContext& ctx) {
    auto& hooks = *ctx.mHookManager;
    HOOK(Player, $constructor);
    HOOK(ContainerWeakRef, tryGetActorContainer);
	HOOK(ContainerValidatorFactory, getBackingContainer);
    VHOOK(Item, appendFormattedHovertext, this);
	HOOK(Recipes, init);
	HOOK(BlockActorFactory, createBlockEntity);
	HOOK(VanillaItems, _addItemsCategory);
    HOOK(ContainerScreenController, _registerBindings);

	using BindFn = bool(HudScreenController::*)(
        const std::string&,
        uint32_t,
        int,
        const std::string&,
        uint32_t,
        const std::string&,
        UIPropertyBag&
    );

    hooks.CreateHookAbsolute(
        _HudScreenController_bind,
        reinterpret_cast<uintptr_t*>(HudScreenController::$vtable_for_this)[
			GetVirtualFunctionOffset<static_cast<BindFn>(&HudScreenController::bind)>() / sizeof(void*) + 1 // +1 because overloads are weird
        ],
        &HudScreenController_bind
	);
}