#pragma once
#include <nlohmann/json.hpp>
#undef ERROR

namespace Config {
    namespace SettingConstants {
        inline constexpr std::string_view toml_path_default = "Data/SKSE/Plugins/stweaks.toml";
        inline constexpr std::string_view toml_path_custom = "Data/SKSE/Plugins/stweaks_custom.toml";
        inline constexpr std::string_view json_path = "Data/SKSE/Plugins/stweaks_exceptions.json";
	}

    struct Settings : REX::Singleton<Settings> {

        static inline REX::TOML::Bool debug_logging{ "DebugLogging", "bDebugLoggingEnable", false };
        static inline REX::TOML::Bool enable_damage_ranges{ "Toggles", "bEnableDamageRanges", true };
        static inline REX::TOML::Bool enable_sneak_jump_limit{ "Toggles", "bEnableSneakJumpLimit", true };
        static inline REX::TOML::Bool enable_mass_based_jump_height{ "Toggles", "bMassBasedJump", true };
        static inline REX::TOML::Bool enable_sneak_stamina{ "Toggles", "bEnableSneakStamina", true };
        static inline REX::TOML::Bool enable_foll_change{ "Toggles", "bEnableFollowerDamageChange", true };
        static inline REX::TOML::Bool enable_etheral_change{ "Toggles", "bEnableEtherealChange", true };
        static inline REX::TOML::Bool enable_diseases{ "Toggles", "bEnableDiseases", true };
        static inline REX::TOML::Bool enable_fading_actors{ "Toggles", "bEnableFadingActors", true };
        static inline REX::TOML::Bool enable_quest_item_nerf{ "Toggles", "bEnableQuestItemNerf", true };
        static inline REX::TOML::Bool allow_curse_swapping{ "Toggles", "bAllowCurseSwaps", true };
		static inline REX::TOML::Bool enable_resist_changes{ "Toggles", "bEnableResistChanges", true };
        static inline REX::TOML::Bool enable_cast_stamina{ "Toggles", "bEnableCastStamina", true };
        static inline REX::TOML::Bool enable_attack_stamina{ "Toggles", "bEnableAttackStamina", true };
        static inline REX::TOML::Bool interupt_cast_on_hit{ "Toggles", "bInteruptCastOnHit", true };

        static inline REX::TOML::F32 sneak_height_modifier{ "SettingValues", "fSneakJumpModifier", 0.55f };
        static inline REX::TOML::I32 weapon_upper_range{ "SettingValues", "iUpperRangeWeapons", 15 };
        static inline REX::TOML::I32 weapon_lower_range{ "SettingValues", "iLowerRangeWeapons", 15 };
        static inline REX::TOML::I32 magic_upper_range{ "SettingValues", "iUpperRangeMagic", 15 };
        static inline REX::TOML::I32 magic_lower_range{ "SettingValues", "iLowerRangeMagic", 15 };
        static inline REX::TOML::F32 curse_chance{ "SettingValues", "fCurseChance", 1.0f };
        static inline REX::TOML::F64 curse_swap_cooldown{ "SettingValues", "fCurseSwapCooldown", 60.0 };
        static inline REX::TOML::F64 base_stamina_cost_attacks{ "SettingValues", "fBaseStaminaCostMelee", 10.0 };
        static inline REX::TOML::F64 resist_reduction_value{ "SettingValues", "fResistReduceValue", 20.0 };
        static inline REX::TOML::F64 magic_stamina_cost_divider{ "SettingValues", "fMagicStamCostDivider", 2.0 };

        /*void LoadSettings();
		void SaveSettings();*/

        inline void LoadSettings()
        {
            REX::INFO("Loading settings...");
            const auto toml = REX::TOML::SettingStore::GetSingleton();
            toml->Init(Config::SettingConstants::toml_path_default.data(), Config::SettingConstants::toml_path_custom.data());
            toml->Load();
        }

        inline void SaveSettings()
        {
            REX::INFO("Save settings...");
            const auto toml = REX::TOML::SettingStore::GetSingleton();
            toml->Init(Config::SettingConstants::toml_path_default.data(), Config::SettingConstants::toml_path_custom.data());
            toml->Save();
        }

        //not happy with how it saves every setting even if the custom file had only 1 overwrite. maybe i'll revisit the idea at some point
        inline void SaveToCustom() {
            REX::INFO("Save Custom settings...");
            const auto toml = REX::TOML::SettingStore::GetSingleton();
            toml->Init(Config::SettingConstants::toml_path_custom.data(), Config::SettingConstants::toml_path_custom.data());
            toml->Save();
        }

    };

    namespace Exceptions {

        static inline std::set<RE::TESObjectWEAP*> weapon_exceptions;

        static inline RE::TESForm* GetFormFromString(const std::string& spellName)
        {
            std::istringstream ss{ spellName };
            std::string plugin, id;
            std::getline(ss, id, '|');
            std::getline(ss, plugin);

            RE::FormID rawFormID;
            std::istringstream(id) >> std::hex >> rawFormID;

            auto dataHandler = RE::TESDataHandler::GetSingleton();
            return dataHandler->LookupForm(rawFormID, plugin);
        }

        static inline void LoadExceptionWeapons(const std::string& configFilePath)
        {
            std::ifstream file(configFilePath);
            if (!file.is_open())
            {
                REX::ERROR("Failed to open the file: {}", configFilePath);
                return;
            }

            nlohmann::json j;
            file >> j;

            //load Exceptions
            if (j.contains("quest_weapon_exceptions") && j["quest_weapon_exceptions"].is_array()) {
                for (const auto& str : j["quest_weapon_exceptions"])
                {
                    const std::string& formStr = str.get<std::string>();
                    REX::DEBUG("Loading exception weapon: {}", formStr);

                    RE::TESForm* form = GetFormFromString(formStr);
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

            REX::INFO("Loaded {} weapons from {}.",
                weapon_exceptions.size(),
                configFilePath);
        }

        static void LoadJson() {
            const auto path = "Data/SKSE/Plugins/stweaks_exceptions.json";
            LoadExceptionWeapons(path);
        }

        inline static bool IsQuestWeaponException(RE::TESObjectWEAP* form)
        {
            return form && weapon_exceptions.contains(form);
        }
    }

}
