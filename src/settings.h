#pragma once
#include <nlohmann/json.hpp>
#include "mod-data.h"
#undef ERROR

namespace Config
{
namespace SettingConstants
{

} // namespace SettingConstants

using namespace SettingConstants;
struct Settings : REX::TSingleton<Settings>
{
    // bools
    static inline REX::TOML::Bool debug_logging{"DebugLogging", "bDebugLoggingEnable", false};
    static inline REX::TOML::Bool enable_damage_ranges{stweaks::TOGGLES, "bEnableDamageRanges", true};
    static inline REX::TOML::Bool enable_sneak_jump_limit{stweaks::TOGGLES, "bEnableSneakJumpLimit", true};
    static inline REX::TOML::Bool enable_mass_based_jump_height{stweaks::TOGGLES, "bMassBasedJump", true};
    static inline REX::TOML::Bool enable_sneak_stamina{stweaks::TOGGLES, "bEnableSneakStamina", true};
    static inline REX::TOML::Bool enable_foll_change{stweaks::TOGGLES, "bEnableFollowerDamageChange", true};
    static inline REX::TOML::Bool enable_etheral_change{stweaks::TOGGLES, "bEnableEtherealChange", true};
    static inline REX::TOML::Bool enable_diseases{stweaks::TOGGLES, "bEnableCurses", true};
    static inline REX::TOML::Bool enable_fading_actors{stweaks::TOGGLES, "bEnableFadingActors", true};
    static inline REX::TOML::Bool enable_quest_item_nerf{stweaks::TOGGLES, "bEnableQuestItemNerf", true};
    static inline REX::TOML::Bool allow_curse_swapping{stweaks::TOGGLES, "bAllowCurseSwaps", true};
    static inline REX::TOML::Bool enable_resist_changes{stweaks::TOGGLES, "bEnableResistChanges", true};
    static inline REX::TOML::Bool enable_cast_stamina{stweaks::TOGGLES, "bEnableCastStamina", true};
    static inline REX::TOML::Bool enable_attack_stamina{stweaks::TOGGLES, "bEnableAttackStamina", true};
    static inline REX::TOML::Bool interupt_cast_on_hit{stweaks::TOGGLES, "bInteruptCastOnHit", true};
    static inline REX::TOML::Bool stamina_regen_changes{stweaks::TOGGLES, "bChangeStaminaRegen", true};
    static inline REX::TOML::Bool magicka_regen_changes{stweaks::TOGGLES, "bChangeMagickaRegen", true};
    static inline REX::TOML::Bool one_shot_protection{stweaks::TOGGLES, "bEnableOneHitProtection", true};
    static inline REX::TOML::Bool enable_damage_caps{stweaks::TOGGLES, "bEnableDamageCaps", true};
    static inline REX::TOML::Bool enable_skill_based_cast_speed{stweaks::TOGGLES, "bEnableSkillBasedCastSpeed", true};
    static inline REX::TOML::Bool level_up_low_levels{stweaks::TOGGLES, "bLevelUpLowLevelEnemies", true};
    static inline REX::TOML::Bool enable_automatic_attributes{stweaks::TOGGLES, "bEnableAutoAttributes", true};
    static inline REX::TOML::Bool enable_mass_equip_changes{stweaks::TOGGLES, "bEnableMassChangesWithEquipment", true};
    static inline REX::TOML::Bool jump_stamina_cost{stweaks::TOGGLES, "bEnableJumpStaminaCost", true};
    static inline REX::TOML::Bool tall_grass_sneak{stweaks::TOGGLES, "bEnableTallGrassEnhancesSneak", true};
    static inline REX::TOML::Bool attacks_of_opp{stweaks::TOGGLES, "bEnableAttacksOfOpportunity", true};
    static inline REX::TOML::Bool show_opp_notif{stweaks::TOGGLES, "bEnableOpportunityNotifications", true};
    static inline REX::TOML::Bool play_opp_sound{stweaks::TOGGLES, "bEnableOpportunitySound", true};
	static inline REX::TOML::Bool use_exhaustion{ stweaks::TOGGLES, "bEnableExhaustionSystem", true };

    static inline REX::TOML::I32 vanilla_attribute_leveling{stweaks::SETTINGS, "iVanillaAttributesOnLevelUp", 5};
    static inline REX::TOML::F32 sneak_height_modifier{stweaks::SETTINGS, "fSneakJumpModifier", 0.55f};
    static inline REX::TOML::I32 weapon_upper_range{stweaks::SETTINGS, "iUpperRangeWeapons", 15};
    static inline REX::TOML::I32 weapon_lower_range{stweaks::SETTINGS, "iLowerRangeWeapons", 15};
    static inline REX::TOML::I32 magic_upper_range{stweaks::SETTINGS, "iUpperRangeMagic", 15};
    static inline REX::TOML::I32 magic_lower_range{stweaks::SETTINGS, "iLowerRangeMagic", 15};
    static inline REX::TOML::F32 curse_chance{stweaks::SETTINGS, "fCurseChance", 1.0f};
    static inline REX::TOML::F64 curse_swap_cooldown{stweaks::SETTINGS, "fCurseSwapCooldown", 60.0};
    static inline REX::TOML::F64 base_stamina_cost_attacks{stweaks::SETTINGS, "fBaseStaminaCostMelee", 10.0};
    static inline REX::TOML::F64 resist_reduction_value{stweaks::SETTINGS, "fResistReduceValue", 20.0};
    static inline REX::TOML::F64 magic_stamina_cost_divider{stweaks::SETTINGS, "fMagicStamCostDivider", 2.0};
    static inline REX::TOML::F64 stamina_regen_base_calc{stweaks::SETTINGS, "fStaminaBaseValue", 150.0};
    static inline REX::TOML::F64 magicka_regen_base_calc{stweaks::SETTINGS, "fMagickaBaseValue", 150.0};
    static inline REX::TOML::F64 max_cast_speed{stweaks::SETTINGS, "fMaxCastSpeedModifier", 2.0};
    static inline REX::TOML::F64 min_cast_speed{stweaks::SETTINGS, "fMinCastSpeedModifier", 0.33};

    static void UpdateSettings(const bool save = false)
    {
        const auto toml = REX::TOML::SettingStore::GetSingleton();
        toml->Init(stweaks::TOML_PATH_DEFAULT.data(), stweaks::TOML_PATH_CUSTOM.data());
        if (!save)
            toml->Load();
        else
            toml->Save();
    };
};

namespace Exceptions
{
struct Sets
{
    static inline std::set<RE::TESObjectWEAP *> weapon_exceptions;
    static inline std::set<RE::TESLandTexture *> land_textures;
};

static inline RE::TESForm *GetFormFromString(const std::string &formIDstring)
{
    if (formIDstring.empty())
    {
        REX::WARN("Empty form string");
        return nullptr;
    }

    if (formIDstring.find('|') != std::string::npos)
    {
        std::istringstream ss{formIDstring};
        std::string plugin, id;
        std::getline(ss, plugin, '|');
        std::getline(ss, id);

        if (plugin.empty() || id.empty())
        {
            return nullptr;
        }

        RE::FormID rawFormID{};
        std::istringstream(id) >> std::hex >> rawFormID;

        if (!rawFormID)
        {
            return nullptr;
        }

        if (auto *dataHandler = RE::TESDataHandler::GetSingleton())
        {
            auto *form = dataHandler->LookupForm(rawFormID, plugin);
            return form;
        }
        return nullptr;
    }

    if (auto *form = RE::TESForm::LookupByEditorID(formIDstring))
    {
        return form;
    }

    REX::WARN("Could not find form '{}'", formIDstring);
    return nullptr;
}

static inline void LoadTallGrass(const std::string &configFilePath)
{
    std::ifstream file(configFilePath);
    if (!file.is_open())
    {
        REX::ERROR("Failed to open the file: {}", configFilePath);
        return;
    }
    nlohmann::json j;
    file >> j;
    if (j.contains("TallGrass") && j["TallGrass"].is_array())
    {
        for (const auto &str : j["TallGrass"])
        {
            const std::string &formStr = str.get<std::string>();
            REX::DEBUG("Loading Tall Grass: {}", formStr);

            RE::TESForm *form = GetFormFromString(formStr);
            if (form && form->GetFormType() == RE::FormType::LandTexture)
            {
                Sets::land_textures.insert(form->As<RE::TESLandTexture>());
                REX::DEBUG("loaded {} as land texture", std::string(editorID::get_editorID(form)));
            }
            else
            {
                REX::WARN("Invalid or non-land texture form: {}", formStr);
            }
        }
    }

    REX::DEBUG("Loaded {} land textures from {}.", Sets::land_textures.size(), configFilePath);
}

static inline void LoadExceptionWeapons(const std::string &configFilePath)
{
    std::ifstream file(configFilePath);
    if (!file.is_open())
    {
        REX::ERROR("Failed to open the file: {}", configFilePath);
        return;
    }

    nlohmann::json j;
    file >> j;

    // load Exceptions
    if (j.contains("quest_weapon_exceptions") && j["quest_weapon_exceptions"].is_array())
    {
        for (const auto &str : j["quest_weapon_exceptions"])
        {
            const std::string &formStr = str.get<std::string>();
            REX::DEBUG("Loading exception weapon: {}", formStr);

            RE::TESForm *form = GetFormFromString(formStr);
            if (form && form->GetFormType() == RE::FormType::Weapon)
            {
                Sets::weapon_exceptions.insert(form->As<RE::TESObjectWEAP>());
                REX::DEBUG("loaded {} as exception", form->GetName());
            }
            else
            {
                REX::WARN("Invalid or non-weapon form: {}", formStr);
            }
        }
    }

    REX::DEBUG("Loaded {} weapons from {}.", Sets::weapon_exceptions.size(), configFilePath);
}

static void LoadJson()
{
    LoadExceptionWeapons(stweaks::JSON_PATH.data());
    LoadTallGrass(stweaks::JSON_PATH_LAND.data());
}

inline static bool IsQuestWeaponException(RE::TESObjectWEAP *form)
{
    REX::DEBUG("checking exceptions");
    if (Sets::weapon_exceptions.empty())
        REX::DEBUG("weapon exceptions is empty");

    return form && Sets::weapon_exceptions.contains(form);
}
inline static bool IsTallGrass(RE::TESLandTexture *form)
{
    if (Sets::land_textures.empty())
        REX::DEBUG("land textures is empty");
    return form && Sets::land_textures.contains(form);
}
} // namespace Exceptions

} // namespace Config
