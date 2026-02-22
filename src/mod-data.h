//
// Created by styyx on 03/02/2026.
//
#pragma once
#include <API/PerkEntryPointExtenderAPI.h>

namespace stweaks
{

    //General Data
    inline constexpr std::string_view mod_name = "STweaks.esp";
    inline constexpr std::string_view diseases_name = "Stweaks - Diseases.esp";
    //Settings Data
    inline constexpr std::string_view TOML_PATH_DEFAULT = "Data/SKSE/Plugins/stweaks.toml";
    inline constexpr std::string_view TOML_PATH_CUSTOM = "Data/SKSE/Plugins/stweaks_custom.toml";
    inline constexpr std::string_view JSON_PATH = "Data/SKSE/Plugins/stweaks_exceptions.json";
    inline constexpr std::string_view JSON_PATH_LAND = "Data/SKSE/Plugins/stweaks_tallGrass.json";
    inline constexpr std::string_view TOGGLES = "Toggles";
    inline constexpr std::string_view SETTINGS = "Settings";

    //Forms

    //Stweaks Main Mod
    inline constexpr RE::FormID sneak_stamina_spell_ID = 0x804;
    inline constexpr RE::FormID tall_grass_perk_ID = 0x805;
    inline constexpr RE::FormID exhaustion_spell_ID = 0x80d;

    //Stweaks Diseases Mod
    inline constexpr RE::FormID health_curse_ID = 0x3;
    inline constexpr RE::FormID stamina_curse_ID = 0x6;
    inline constexpr RE::FormID magicka_curse_ID = 0x9;
    inline constexpr RE::FormID silence_curse_ID = 0xD;
    inline constexpr RE::FormID melee_weakness_curse_ID = 0x10;
    inline constexpr RE::FormID bow_weakness_curse_ID = 0x13;
    inline constexpr RE::FormID jump_curse_ID = 0x16;
    inline constexpr RE::FormID spell_allow_list_ID = 0x1c;
    inline constexpr RE::FormID curses_formlist_ID = 0x1b;

    // Constants

    inline constexpr float MIN_JUMP_STAMINA_COST = 10.0f;
    inline constexpr float MAX_JUMP_STAMINA_COST = 50.0f;
    inline constexpr float MAX_JUMP_MASS_CALCULATOR = 80.0f;
    inline constexpr float BASE_MASS_AVERAGE = 1.2f;

    inline constexpr float FADE_DISTANCE_FULL_FADE = 2400.0f;
    inline constexpr float FADE_DISTANCE_NO_FADE = 1200.0f;
    inline constexpr float FADE_LIGHT_LEVEL_THRESHOLD = 30.0f;
    inline constexpr float FADE_MIN_ALPHA = 0.05f;

    static constexpr uint16_t LEVEL_CAP = 5;

    namespace AttackStamina
    {
        static constexpr float MIN_COST = 5.0f;
        static constexpr float AVERAGE_WEAPON_WEIGHT = 12.5f;
        static constexpr float WEIGHT_SCALING = 1.2f;
        static constexpr float SKILL_SCALING = 1.10f;
    }


    namespace pepe
    {
        namespace SNEAK_STAMINA
        {
            static inline constexpr auto perkCategory = "ModSneakStamina";
            static inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kModPowerAttackStamina;
        }
        namespace JUMP_STAMINA
        {
            static inline constexpr auto perkCategory = "ModJumpStamina";
            static inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kModPowerAttackStamina;
        }

        namespace CASTING_STAMINA
        {
            static inline constexpr auto perkCategory = "ModCastStamina";
            static inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kModPowerAttackStamina;
        }

        namespace ATTACK_STAMINA
        {
            static inline constexpr auto perkCategory = "ModAttackStamina";
            static inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kModPowerAttackStamina;
        }

        namespace STEALTH_ARCHER
        {
            static inline constexpr auto perkCategory = "SetDrainWithBow";
            static inline constexpr RE::PerkEntryPoint perkEntry = RE::PerkEntryPoint::kCanDualCastSpell;
        }

    }


    namespace keywords
    {
        inline constexpr std::string_view kCurse = "stweaks_curse";
        inline constexpr std::string_view kCure = "cleanse_curse";
        inline constexpr std::string_view kSilence = "curse_silence";
        inline constexpr std::string_view kBow = "curse_bow";
        inline constexpr std::string_view kJump = "curse_jump";
        inline constexpr std::string_view kAttack = "curse_weapons";
        inline constexpr std::array<std::string_view, 3> disease_keywords{"stweaksDisease_Health", "stweaksDisease_Stamina","stweaksDisease_Magicka"};
        inline std::vector<std::string> disease_vec{"stweaksDisease_Health", "stweaksDisease_Stamina","stweaksDisease_Magicka"};

    }
}
