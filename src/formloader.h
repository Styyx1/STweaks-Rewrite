#pragma once

#include "settings.h"

namespace Forms
{
namespace FormConstants
{
static inline std::vector<std::string> diseases{"stweaksDisease_Health", "stweaksDisease_Stamina",
                                                "stweaksDisease_Magicka"};

constexpr const char *mod_name = "STweaks.esp";
constexpr const char *diseases_name = "Stweaks - Diseases.esp";
const int sneak_stamina_spell_ID = 0x804;
const int health_curse_ID = 0x3;
const int stamina_curse_ID = 0x6;
const int magicka_curse_ID = 0x9;
const int silence_curse_ID = 0xD;
const int melee_weakness_curse_ID = 0x10;
const int bow_weakness_curse_ID = 0x13;
const int jump_curse_ID = 0x16;
const int spell_allow_list_ID = 0x21;
const int tall_grass_perk_ID = 0x805;

constexpr const char *cure_keyword = "cleanse_curse";
constexpr const char *curse_keyword = "stweaks_curse";
constexpr const char *silence_key = "curse_silence";
constexpr const char *bow_curse_key = "curse_bow";
constexpr const char *jump_curse_key = "curse_jump";

} // namespace FormConstants
struct FormLoader : REX::Singleton<FormLoader>
{

    bool disease_mod_active = false;
    static inline RE::SpellItem *sneak_stamina_spell{nullptr};
    static inline RE::SpellItem *health_curse{nullptr};
    static inline RE::SpellItem *stamina_curse{nullptr};
    static inline RE::SpellItem *magicka_curse{nullptr};
    static inline RE::SpellItem *silence_curse{nullptr};
    static inline RE::SpellItem *melee_damage_curse{nullptr};
    static inline RE::SpellItem *bow_damage_curse{nullptr};
    static inline RE::SpellItem *jump_curse{nullptr};
    static inline RE::BGSListForm *spell_allow_list{nullptr};
    static inline RE::BGSPerk *tall_grass_perk{nullptr};

    static inline std::vector<RE::SpellItem *> curse_list{};
    static inline std::vector<RE::SpellItem *> spell_allow_vector{};

    void LoadForms();
};
} // namespace Forms
