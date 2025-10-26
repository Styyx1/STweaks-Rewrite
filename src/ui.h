#pragma once
#include "API/SKSEMenuFramework.h"
#include "settings.h"
namespace UI {

    using str = std::string;

    void Register();
    namespace STWEAKSMenu {

        void RestoreFromSettings();
        void RestoreDefaults();

        void HelpMarker(const char* desc);
        void SettingButton(const char* button_desc, const char* tooltip_desc);

        //main title 
        inline str TitleText = "sTweaks"; // f717   f4fe
        inline str Section1_Text = "Settings";
        // setting names
        inline str Button_JumpHeightMod = "Jump Height Modifier";
        inline str Button_CurseChance = "Curse Chance";
        inline str Button_CurseSwapCoolDown = "Curse Swap Cooldown";
        inline str Button_CurseSwapToggle = "Allow Curse Swapping?";
        inline str Button_StaminaAttack = "Stamina Cost Attacks";
        inline str Button_StaminaMagic = "Magic Stamina Cost Modifier";
        inline str Button_UpperRangeMelee = "Melee Upper Range";
        inline str Button_UpperRangeMagic = "Magic Upper Range";
        inline str Button_LowerRangeMelee = "Melee Lower Range";
        inline str Button_LowerRangeMagic = "Magic Lower Range";
        inline str Button_ResistanceChangeValue = "Magic Resistance Change";
        //Tooltips
        inline str Tooltip_JumpHeightMod = "Reduces the jump height while sneaking with the set modifier";
        inline str Tooltip_CurseChance = "Chance to get afflicted by a curse when hit with any spell. Half the chance for getting hit by concentration spells";
        inline str Tooltip_CurseSwapCooldown = "The amount of time it takes before a curse can be re-afflicted, means, swapped";
        inline str Tooltip_CurseSwapToggle = "Toggles the swapping of a curse. If not toggled, the cooldown is ignored";
        inline str Tooltip_StaminaAttack = "Base Stamina cost for normal attacks";
        inline str Tooltip_MagicStamina = "Spell cost modifier to define stamina cost of spells. Formula is Spell Cost divided by modifier";
        inline str Tooltip_UpperRangeMelee = "Upper range of melee damage in percent. This means damage dealt and received.";
        inline str Tooltip_LowerRangeMelee = "Lower range of melee damage in percent. This means damage dealt and received.";
        inline str Tooltip_UpperRangeMagic = "Upper range of magic damage in percent. This means damage dealt and received.";
        inline str Tooltip_LowerRangeMagic = "Lower range of magic damage in percent. This means damage dealt and received.";
        inline str Tooltip_ResistanceChange = "lowers the magic resistance of enemies by the set amount. Up to player level 5 \n and only for enemies with the same or a higher level as the player. \n Should make early game as mage a bit easier";

        //titles
        inline str Title_Curses = FontAwesome::UnicodeToUtf8(0xf717) + " Curses";
        inline str Title_JumpHeight = FontAwesome::UnicodeToUtf8(0xf52e) + " Jump Height";
        inline str Title_Stamina = FontAwesome::UnicodeToUtf8(0xf241) + " Stamina";
        inline str Title_Ranges = FontAwesome::UnicodeToUtf8(0xf338) + " Damage Ranges";
        inline str Title_Resist = FontAwesome::UnicodeToUtf8(0xe05d) + " Lower Resistance";
        
        //system settings
        inline str Title_System = FontAwesome::UnicodeToUtf8(0xf390) + " System";
        inline str Button_SaveSettings = FontAwesome::UnicodeToUtf8(0xf0c7) + " Save Settings";
        inline str Button_ResetSettings = FontAwesome::UnicodeToUtf8(0xf0e2) + " Reset Settings";

        inline float jump_heigh_temp;
        inline float curse_chance_temp;
        inline float curse_swap_cooldown_temp;
        inline float base_stamina_cost_weap_temp;
        inline float magic_stam_modifier_temp;
        inline int upper_range_melee_temp;
        inline int upper_range_mage_temp;
        inline int lower_range_melee_temp;
        inline int lower_range_mage_temp;
        inline bool allow_curse_swap_temp;
        inline float resist_change_temp;

        void __stdcall Render();
        // Toggles
        // Title
        inline str Title_SectionToggles = "Mod Toggles";
        inline str Button_Toggles = FontAwesome::UnicodeToUtf8(0xf14a) + " Toggles";
        inline str Tooltip_TogglesGeneral = "Toggle various options of the mod. This requires you to save the settings and restart the game";
        inline str NOTE_Restart = "The toggles on this page require you to save the settings and to restart your game.\nThis may not work if you have a sTweaks_custom.toml file to overwrite settings";
        //setting
        inline str tog = "Toggle ";
        inline str Button_DamageRanges_Toggles = tog + "Damage Ranges";
        inline str Button_SneakJump_Toggles = tog + "Sneak Jump Limit";
        inline str Button_EtherealChange_Toggles = tog + "Ethereal Changes";
        inline str Button_MassBased_Toggles = tog + "Mass Based Jump";
        inline str Button_SneakStamina_Toggles = tog + "Sneak Stamina Cost";
        inline str Button_FadeOutActors_Toggles = tog + "Actor Fade Out";
        inline str Button_QuestItemNerf_Toggles = tog + "Quest Weapon Nerf";
        inline str Button_Curses_Toggles = tog + "Curses";
        inline str Button_ResistChange_Toggles = tog + "Resistance Reduction";
        inline str Button_AttackStamina_Toggles = tog + "Attack Stamina Cost";
        inline str Button_InteruptCast_Toggles = tog + "Interupt Spell Casting";
        inline str Button_MagicStamina_Toggles = tog + "Magic Stamina Cost";

        inline str Tooltip_DamageRanges_Toggles = "Damage Ranges: \nYour attacks don't always deal the same damage.";
        inline str Tooltip_SneakJump_Toggles = "Sneak Jump Limit: \nLowers the jump height while you're sneaking";
        inline str Tooltip_EtherealChange_Toggles = "Ethereal Changes: \nIn vanilla, the first hit, that breaks the ethereal effect can still cause damage. This fixes it.";
        inline str Tooltip_MassBased_Toggles = "Mass Based Jump: \nJump height changes depending on your mass";
        inline str Tooltip_SneakStamina_Toggles = "Sneak Stamina Cost: \nSneaking and moving costs stamina. So does sneaking with a bow equipped even without moving (to nerf the godlike Skyrim Stealth Archer)";
        inline str Tooltip_FadeOutActors_Toggles = "Actor Fade Out: \nActors become invisible when they're further away (not an actual invisibility effect)";
        inline str Tooltip_QuestItemNerf_Toggles = "Quest Weapon Nerf: \nQuest items don't weight anything, so in a setup where carry weight is important, those are free backup weapons.";
        inline str Tooltip_Curses_Toggles = "Curses: \nThis toggles the Curse mechanic of the mod, please check the modpage to read about them";
        inline str Tooltip_ResistChange_Toggles = "Resistance Reduction: \nEnemies with a higher or equal level as the player \nget their magic resistance lowered to make early game magic a bit easier if needed.";
        inline str Tooltip_AttackStamina_Toggles = "Attack Stamina Cost: \nNormal attacks cost stamina.";
        inline str Tooltip_InteruptCast_Toggles = "Interupt Spell Casting: \nAttacks will interupt casting of spells";
        inline str Tooltip_MagicStamina_Toggles = "Magic Stamina Cost: \nCasting spells will cost stamina based on the magicka cost of the spells";

        inline bool damage_ranges_temp;
        inline bool sneak_jump_limit_temp;
        inline bool set_ethereal_change_temp;
        inline bool mass_based_jump_temp;
        inline bool sneak_stamina_temp;
        inline bool fade_out_actors_temp;
        inline bool quest_item_nerf_temp;
        inline bool toggle_curses_temp;
        inline bool toggle_resist_change_temp;
        inline bool spell_stamina_cost_temp;
        inline bool attack_stamina_cost_temp;
        inline bool interupt_cast_temp;

        void __stdcall RenderToggles();
        void SetToggleDefaults();
        void LoadTogglesFromSettings();
    } 
};