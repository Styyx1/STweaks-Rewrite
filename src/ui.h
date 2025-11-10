#pragma once
#include "API/SKSEMenuFramework.h"
#include "settings.h"
namespace UI
{
void RestoreFromSettings(bool settings, bool toggles = false, bool attributes = false);
void RestoreDefaults(bool settings, bool toggles = false, bool attributes = false);
void HelpMarker(const char *desc);

using str = std::string;

void Register();

namespace Titles
{
inline str MOD_NAME = "sTweaks";
inline str SETTING_TAB = "Settings";
inline str TOGGLE_TAB = "Toggles";
inline str STAMINA_SYSTEM = "Attribute Settings";
} // namespace Titles
namespace Settings
{

void __stdcall RenderSettings();

namespace Titles
{
inline str jump_height = FontAwesome::UnicodeToUtf8(0xf52e) + " Jump Height";
inline str curses = FontAwesome::UnicodeToUtf8(0xf717) + " Curses";
inline str stamina = FontAwesome::UnicodeToUtf8(0xf241) + " Stamina";
inline str ranges = FontAwesome::UnicodeToUtf8(0xf338) + " Damage Ranges";
inline str resist = FontAwesome::UnicodeToUtf8(0xe05d) + " Lower Resistance";
inline str system = FontAwesome::UnicodeToUtf8(0xf390) + " System";
} // namespace Titles
namespace Label
{
inline str jump_height_mod = "Jump Height Modifier";
inline str curse_chance = "Curse Chance";
inline str curse_swap_cooldown = "Curse Swap Cooldown";
inline str curse_swap_toggle = "Allow Curse Swapping?";
inline str upper_range_melee = "Melee Upper Range";
inline str upper_range_magic = "Magic Upper Range";
inline str lower_range_melee = "Melee Lower Range";
inline str lower_range_magic = "Magic Lower Range";
inline str resistance_change_value = "Magic Resistance Change";

inline str save_settings = FontAwesome::UnicodeToUtf8(0xf0c7) + " Save Settings";
inline str restore_defaults = FontAwesome::UnicodeToUtf8(0xf0e2) + " Reset Settings";

} // namespace Label
namespace Tooltip
{
inline str jump_height_mod = "Reduces the jump height while sneaking with the set modifier";
inline str curse_chance = "Chance to get afflicted by a curse when hit with any spell. Half the chance for getting hit "
                          "by concentration spells";
inline str curse_swap_cooldown = "The amount of time it takes before a curse can be re-afflicted, means, swapped";
inline str curse_swap_toggle = "Toggles the swapping of a curse. If not toggled, the cooldown is ignored";
inline str upper_range_melee = "Upper range of melee damage in percent. This means damage dealt and received.";
inline str lower_range_melee = "Lower range of melee damage in percent. This means damage dealt and received.";
inline str upper_range_magic = "Upper range of magic damage in percent. This means damage dealt and received.";
inline str lower_range_magic = "Lower range of magic damage in percent. This means damage dealt and received.";
inline str resistance_change_value =
    "Lowers the magic resistance of enemies by the set amount. Up to player level 5 \n and only for enemies with the "
    "same or a higher level as the player. \n Should make early game as mage a bit easier";

} // namespace Tooltip
namespace vars
{
inline float jump_height_mod;
inline float curse_chance;
inline float curse_swap_cooldown;
inline int upper_range_melee;
inline int upper_range_magic;
inline int lower_range_melee;
inline int lower_range_magic;
inline bool curse_swap_toggle;
inline float resistance_change_value;

} // namespace vars

} // namespace Settings

namespace Attributes
{
void __stdcall RenderAttributes();
namespace Titles
{
inline str attributes = "Attributes";
inline str attributes_header = FontAwesome::UnicodeToUtf8(0xf6cf) + " Attributes";
inline str stamina = FontAwesome::UnicodeToUtf8(0xf241) + " Stamina";
inline str magicka = FontAwesome::UnicodeToUtf8(0xf6e8) + " Magicka";
inline str health = FontAwesome::UnicodeToUtf8(0xf21e) + " Health";
inline str system = FontAwesome::UnicodeToUtf8(0xf390) + " System";
inline str regen = FontAwesome::UnicodeToUtf8(0xf4be) + " Regeneration";

} // namespace Titles
namespace Label
{
inline str stamina_attack = "Stamina Cost Attacks";
inline str stamina_magic = "Magic Stamina Cost Modifier";
inline str sneak_stamina = "Sneak Stamina Cost";
inline str magic_stamina_toggle = "Magic Stamina Cost";
inline str attack_stamina_toggle = "Attack Stamina Cost";
inline str base_for_stamina_regen = "Stamina Regen Rate Base";
inline str base_for_magicka_regen = "Magicka Regen Rate Base";
inline str stamina_regen_toggle = "Toggle Stamina Regen";
inline str magicka_regen_toggle = "Toggle Magicka Regen";
inline str toggle_cast_speed = "Toggle Cast Speed";
inline str min_cast_speed = "Min Cast Speed Modifier";
inline str max_cast_speed = "Max Cast Speed Modifier";

inline str save_settings = FontAwesome::UnicodeToUtf8(0xf0c7) + " Save Settings";
inline str restore_defaults = FontAwesome::UnicodeToUtf8(0xf0e2) + " Reset Settings";
} // namespace Label
namespace Tooltip
{
inline str stamina_attack = "Base Stamina cost for normal attacks";
inline str stamina_magic = "Spell cost modifier to define stamina cost of "
                           "spells. Formula is Spell Cost divided by modifier";
inline str sneak_stamina = "Sneak Stamina Cost:\nSneaking and moving costs stamina. Sneaking with a "
                           "bow equipped "
                           "also consumes stamina (to nerf the godlike stealth archer).";
inline str magic_stamina_toggle = "Magic Stamina Cost:\nCasting spells costs stamina "
                                  "based on their magicka cost.";
inline str attack_stamina_toggle = "Attack Stamina Cost:\nNormal attacks consume stamina.";
inline str base_for_stamina_regen = "Base used for stamina regen instead of max stamina";
inline str base_for_magicka_regen = "Base used for magicka regen instead of max magicka";
inline str stamina_regen_toggle = "Toggle changes to Stamina Regen calculations";
inline str magicka_regen_toggle = "Toggle changes to Magicka Regen calculations";

inline str toggle_cast_speed = "Toggle Skill based casting speed changes";
inline str min_cast_speed = "Minimum casting speed Multiplier at low skill level. \nThe lower the value, the lower the casting speed at low skill levels";
inline str max_cast_speed = "Maximum casting speed Multiplier at high skill level. \nThe higher the value, the higher the casting speed at high skill levels";

} // namespace Tooltip
namespace vars
{
inline float stamina_attack;
inline float stamina_magic;
inline bool sneak_stamina;
inline bool magic_stamina_toggle;
inline bool attack_stamina_toggle;
inline float base_for_stamina_regen;
inline float base_for_magicka_regen;
inline bool stamina_regen_toggle;
inline bool magicka_regen_toggle;
inline bool toggle_cast_speed;
inline float min_cast_speed;
inline float max_cast_speed;

} // namespace vars

} // namespace Attributes

namespace Toggles
{

void __stdcall RenderToggles();

namespace Titles
{
inline str toggles = FontAwesome::UnicodeToUtf8(0xf14a) + " Toggles";
inline str system = FontAwesome::UnicodeToUtf8(0xf390) + " System";

inline str system_note = "This will not change settings in your stweaks_custom.toml "
                         "which will always have priority for the settings";
} // namespace Titles
namespace Label
{
inline str damage_ranges = "Damage Ranges";
inline str sneak_jump_limit = "Sneak Jump Limit";
inline str ethereal_change = "Ethereal Changes";
inline str mass_based_jump = "Mass Based Jump";
inline str fade_out_actors = "Actor Fade Out";
inline str quest_item_nerf = "Quest Weapon Nerf";
inline str curses = "Curses";
inline str resist_change = "Resistance Reduction";
inline str interupt_cast = "Interupt Spell Casting";
inline str follower_damage = "Follower Damage Changes";
inline str one_shot_protec = "One Hit Protection";
inline str enable_damage_caps = "Damage Caps";
inline str level_up_lows = "Dynamic Enemy Levels";

inline str save_settings = FontAwesome::UnicodeToUtf8(0xf0c7) + " Save Settings";
inline str restore_defaults = FontAwesome::UnicodeToUtf8(0xf0e2) + " Reset Settings";
} // namespace Label
namespace Tooltip
{
inline str damage_ranges = "Damage Ranges:\nYour attacks don't always deal the same damage.";
inline str sneak_jump_limit = "Sneak Jump Limit:\nLowers the jump height while you're sneaking.";
inline str ethereal_change = "Ethereal Changes:\nIn vanilla, the first hit that breaks the ethereal "
                             "effect can still cause damage. This fixes it.";
inline str mass_based_jump = "Mass Based Jump:\nJump height changes depending on your mass.";
inline str fade_out_actors = "Actor Fade Out:\nActors become invisible when they're far away "
                             "(not an actual invisibility effect).";
inline str quest_item_nerf = "Quest Weapon Nerf:\nQuest items don't weigh anything in vanilla. "
                             "This makes them behave more realistically when weight matters.";
inline str curses = "Curses:\nToggles the Curse mechanic. See modpage for details.";
inline str resist_change = "Resistance Reduction:\nEnemies with higher or equal level as the player "
                           "get reduced magic resistance up to player level 5.\nHelps early game "
                           "mages.";
inline str interupt_cast = "Interupt Spell Casting:\nAttacks will interrupt spell casting.";
inline str follower_damage = "Follower Damage Changes:\nThe more followers you have, the less damage "
                             "your party deals.\nSpeechcraft offsets this reduction.";
inline str one_shot_protec = "Prevents NPCs and you from being one-hit except by melee sneak attacks";
inline str enable_damage_caps = "Caps damage to 5 times the base weapon damage for normal attacks and 10 "
                                "times for power attacks. Does not cap sneak attack damage";
inline str level_up_lows = "Levels up low level enemies as they spawn up to 10 levels below your level";
} // namespace Tooltip
namespace vars
{
inline bool damage_ranges;
inline bool sneak_jump_limit;
inline bool ethereal_change;
inline bool mass_based_jump;
inline bool fade_out_actors;
inline bool quest_item_nerf;
inline bool curses;
inline bool resist_change;
inline bool interupt_cast;
inline bool follower_damage;
inline bool one_shot_protec;
inline bool enable_damage_caps;
inline bool level_up_lows;
} // namespace vars
} // namespace Toggles

}; // namespace UI
