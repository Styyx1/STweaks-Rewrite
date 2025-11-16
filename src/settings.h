#pragma once
#include <nlohmann/json.hpp>
#undef ERROR

namespace Config
{
namespace SettingConstants
{
inline constexpr std::string_view TOML_PATH_DEFAULT = "Data/SKSE/Plugins/stweaks.toml";
inline constexpr std::string_view TOML_PATH_CUSTOM = "Data/SKSE/Plugins/stweaks_custom.toml";
inline constexpr std::string_view JSON_PATH = "Data/SKSE/Plugins/stweaks_exceptions.json";
inline constexpr std::string TOGGLES = "Toggles";
inline constexpr std::string SETTINGS = "SettingValues";
} // namespace SettingConstants

using namespace SettingConstants;
struct Settings : REX::Singleton<Settings>
{
    // bools
    static inline REX::TOML::Bool debug_logging{"DebugLogging", "bDebugLoggingEnable", false};
    static inline REX::TOML::Bool enable_damage_ranges{TOGGLES, "bEnableDamageRanges", true};
    static inline REX::TOML::Bool enable_sneak_jump_limit{TOGGLES, "bEnableSneakJumpLimit", true};
    static inline REX::TOML::Bool enable_mass_based_jump_height{TOGGLES, "bMassBasedJump", true};
    static inline REX::TOML::Bool enable_sneak_stamina{TOGGLES, "bEnableSneakStamina", true};
    static inline REX::TOML::Bool enable_foll_change{TOGGLES, "bEnableFollowerDamageChange", true};
    static inline REX::TOML::Bool enable_etheral_change{TOGGLES, "bEnableEtherealChange", true};
    static inline REX::TOML::Bool enable_diseases{TOGGLES, "bEnableDiseases", true};
    static inline REX::TOML::Bool enable_fading_actors{TOGGLES, "bEnableFadingActors", true};
    static inline REX::TOML::Bool enable_quest_item_nerf{TOGGLES, "bEnableQuestItemNerf", true};
    static inline REX::TOML::Bool allow_curse_swapping{TOGGLES, "bAllowCurseSwaps", true};
    static inline REX::TOML::Bool enable_resist_changes{TOGGLES, "bEnableResistChanges", true};
    static inline REX::TOML::Bool enable_cast_stamina{TOGGLES, "bEnableCastStamina", true};
    static inline REX::TOML::Bool enable_attack_stamina{TOGGLES, "bEnableAttackStamina", true};
    static inline REX::TOML::Bool interupt_cast_on_hit{TOGGLES, "bInteruptCastOnHit", true};
    static inline REX::TOML::Bool stamina_regen_changes{TOGGLES, "bChangeStaminaRegen", true};
    static inline REX::TOML::Bool magicka_regen_changes{TOGGLES, "bChangeMagickaRegen", true};
    static inline REX::TOML::Bool cap_damage_output{TOGGLES, "bToggleDamageCaps", true};
    static inline REX::TOML::Bool one_shot_protection{TOGGLES, "bEnableOneHitProtection", true};
    static inline REX::TOML::Bool enable_damage_caps{TOGGLES, "bEnableDamageCaps", true};
    static inline REX::TOML::Bool enable_skill_based_cast_speed{TOGGLES, "bEnableSkillBasedCastSpeed", true};
    static inline REX::TOML::Bool level_up_low_levels{TOGGLES, "bLevelUpLowLevelEnemies", true};
    static inline REX::TOML::Bool enable_automatic_attributes{TOGGLES, "bEnableAutoAttributes", true};
    static inline REX::TOML::Bool enable_mass_equip_changes{TOGGLES, "bEnableMassChangesWithEquipment", true};
    static inline REX::TOML::Bool jump_stamina_cost{TOGGLES, "bEnableJumpStaminaCost", true};

    static inline REX::TOML::F32 sneak_height_modifier{SETTINGS, "fSneakJumpModifier", 0.55f};
    static inline REX::TOML::I32 weapon_upper_range{SETTINGS, "iUpperRangeWeapons", 15};
    static inline REX::TOML::I32 weapon_lower_range{SETTINGS, "iLowerRangeWeapons", 15};
    static inline REX::TOML::I32 magic_upper_range{SETTINGS, "iUpperRangeMagic", 15};
    static inline REX::TOML::I32 magic_lower_range{SETTINGS, "iLowerRangeMagic", 15};
    static inline REX::TOML::F32 curse_chance{SETTINGS, "fCurseChance", 1.0f};
    static inline REX::TOML::F64 curse_swap_cooldown{SETTINGS, "fCurseSwapCooldown", 60.0};
    static inline REX::TOML::F64 base_stamina_cost_attacks{SETTINGS, "fBaseStaminaCostMelee", 10.0};
    static inline REX::TOML::F64 resist_reduction_value{SETTINGS, "fResistReduceValue", 20.0};
    static inline REX::TOML::F64 magic_stamina_cost_divider{SETTINGS, "fMagicStamCostDivider", 2.0};
    static inline REX::TOML::F64 stamina_regen_base_calc{SETTINGS, "fStaminaBaseValue", 150.0};
    static inline REX::TOML::F64 magicka_regen_base_calc{SETTINGS, "fMagickaBaseValue", 150.0};
    static inline REX::TOML::F64 max_cast_speed{SETTINGS, "fMaxCastSpeedModifier", 2.0};
    static inline REX::TOML::F64 min_cast_speed{SETTINGS, "fMinCastSpeedModifier", 0.33};

    inline void UpdateSettings(bool save = false)
    {
        const auto toml = REX::TOML::SettingStore::GetSingleton();
        toml->Init(TOML_PATH_DEFAULT.data(), TOML_PATH_CUSTOM.data());
        if (!save)
            toml->Load();
        else
            toml->Save();
    };
};

namespace Exceptions
{

static inline std::set<RE::TESObjectWEAP *> weapon_exceptions;

static inline RE::TESForm *GetFormFromString(const std::string &spellName)
{
    std::istringstream ss{spellName};
    std::string plugin, id;
    std::getline(ss, id, '|');
    std::getline(ss, plugin);

    RE::FormID rawFormID;
    std::istringstream(id) >> std::hex >> rawFormID;

    auto dataHandler = RE::TESDataHandler::GetSingleton();
    return dataHandler->LookupForm(rawFormID, plugin);
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
                weapon_exceptions.insert(form->As<RE::TESObjectWEAP>());
                REX::INFO("loaded {} as exception", form->GetName());
            }
            else
            {
                REX::WARN("Invalid or non-weapon form: {}", formStr);
            }
        }
    }

    REX::INFO("Loaded {} weapons from {}.", weapon_exceptions.size(), configFilePath);
}

static void LoadJson()
{
    LoadExceptionWeapons(JSON_PATH.data());
}

inline static bool IsQuestWeaponException(RE::TESObjectWEAP *form)
{
    return form && weapon_exceptions.contains(form);
}
} // namespace Exceptions

} // namespace Config
