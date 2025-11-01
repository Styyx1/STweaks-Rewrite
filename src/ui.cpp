#include "ui.h"
#include "utility.h"
#include "cache.h"

namespace UI {
	void Register()
	{
        if (!SKSEMenuFramework::IsInstalled()) {
            return;
        }
        SKSEMenuFramework::SetSection(STWEAKSMenu::TitleText.c_str());        
        SKSEMenuFramework::AddSectionItem(STWEAKSMenu::Section1_Text.c_str(), STWEAKSMenu::Render);
        SKSEMenuFramework::AddSectionItem(STWEAKSMenu::Title_SectionToggles.c_str(), STWEAKSMenu::RenderToggles);
        STWEAKSMenu::RestoreFromSettings();
        STWEAKSMenu::LoadTogglesFromSettings();
	}

    void STWEAKSMenu::RestoreFromSettings()
    {
        using set = Config::Settings;
        jump_heigh_temp = set::sneak_height_modifier.GetValue();
        curse_chance_temp = set::curse_chance.GetValue();
        curse_swap_cooldown_temp = set::curse_swap_cooldown.GetValue();
        base_stamina_cost_weap_temp = set::base_stamina_cost_attacks.GetValue();
        magic_stam_modifier_temp = set::magic_stamina_cost_divider.GetValue();
        upper_range_melee_temp = set::weapon_upper_range.GetValue();
        upper_range_mage_temp = set::magic_upper_range.GetValue();
        lower_range_melee_temp = set::weapon_lower_range.GetValue();
        lower_range_mage_temp = set::magic_lower_range.GetValue();   
        allow_curse_swap_temp = set::allow_curse_swapping.GetValue();
        resist_change_temp = set::resist_reduction_value.GetValue();
    }

    void STWEAKSMenu::RestoreDefaults()
    {
        using set = Config::Settings;
        jump_heigh_temp = 0.55;  
        set::sneak_height_modifier.SetValue(jump_heigh_temp);
        curse_chance_temp = 1.0f;
        set::curse_chance.SetValue(curse_chance_temp);
        curse_swap_cooldown_temp = 60.0;
        set::curse_swap_cooldown.SetValue(curse_swap_cooldown_temp);
        base_stamina_cost_weap_temp = 20.0f;
        set::base_stamina_cost_attacks.SetValue(base_stamina_cost_weap_temp);
        magic_stam_modifier_temp = 2.0;
        set::magic_stamina_cost_divider.SetValue(magic_stam_modifier_temp);
        upper_range_melee_temp = 15;
        set::weapon_upper_range.SetValue(upper_range_melee_temp);
        upper_range_mage_temp = 15;
        set::magic_upper_range.SetValue(upper_range_mage_temp);
        lower_range_melee_temp = 15;
        set::weapon_lower_range.SetValue(lower_range_melee_temp);
        lower_range_mage_temp = 15;
        set::magic_lower_range.SetValue(lower_range_mage_temp);
        allow_curse_swap_temp = true;
        set::allow_curse_swapping.SetValue(allow_curse_swap_temp);
        resist_change_temp = 20.0f;
        set::resist_reduction_value.SetValue(resist_change_temp);
    }

    //https://github.com/QTR-Modding/SaveManagerSKSE/blob/af53d32f57a3fc4a0c0e22828b66e4592338826e/src/UI.cpp#L3
    void STWEAKSMenu::HelpMarker(const char* desc) {
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip()) {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    void __stdcall UI::STWEAKSMenu::Render() {      

        // Jump Height
        FontAwesome::PushSolid();
        ImGui::NewLine();
        ImGui::Text(Title_JumpHeight.c_str());
        ImGui::SetNextItemWidth(300.0f);
        if (ImGui::SliderFloat(Button_JumpHeightMod.c_str(), &jump_heigh_temp, 0.0, 1.0, "%.2f")) {
            Config::Settings::sneak_height_modifier.SetValue(jump_heigh_temp);
        }       
        FontAwesome::Pop();

        //Curse Settings
        FontAwesome::PushSolid();
        ImGui::NewLine();
        ImGui::Text(Title_Curses.c_str());        
        
        if (ImGui::Checkbox(Button_CurseSwapToggle.c_str(), &allow_curse_swap_temp)) {                           
            Config::Settings::allow_curse_swapping.SetValue(allow_curse_swap_temp);            
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_CurseSwapToggle.c_str());
        
        ImGui::SetNextItemWidth(200.0);
        if (ImGui::SliderFloat(Button_CurseChance.c_str(), &curse_chance_temp, 0.0, 100.0f, "%.2f%%")) {
            Config::Settings::curse_chance.SetValue(curse_chance_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_CurseChance.c_str());
        
        ImGui::SetNextItemWidth(200.0);        
        if (ImGui::SliderFloat(Button_CurseSwapCoolDown.c_str(), &curse_swap_cooldown_temp, 3.0, 120.0, "%.2fsec")) {
            Config::Settings::curse_swap_cooldown.SetValue(curse_swap_cooldown_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_CurseSwapCooldown.c_str());
        FontAwesome::Pop();

        //Stamina Settings
        FontAwesome::PushSolid();
        ImGui::NewLine();
        ImGui::Text(Title_Stamina.c_str());
        

        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::SliderFloat(Button_StaminaAttack.c_str(), &base_stamina_cost_weap_temp, 3.0, 50.0, "%.2f")) {
            Config::Settings::base_stamina_cost_attacks.SetValue(base_stamina_cost_weap_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_StaminaAttack.c_str());
        ImGui::SameLine();

        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::SliderFloat(Button_StaminaMagic.c_str(), &magic_stam_modifier_temp, 0.1, 10.0f, "%.2f")) {
            Config::Settings::magic_stamina_cost_divider.SetValue(magic_stam_modifier_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_MagicStamina.c_str());
        FontAwesome::Pop();

        // Damage Ranges
        FontAwesome::PushSolid();
        ImGui::NewLine();
        ImGui::Text(Title_Ranges.c_str());

        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::SliderInt(Button_UpperRangeMelee.c_str(), &upper_range_melee_temp, 0, 99, "%d%%")) {
            Config::Settings::weapon_upper_range.SetValue(upper_range_melee_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_UpperRangeMelee.c_str());
        ImGui::SameLine();

        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::SliderInt(Button_LowerRangeMelee.c_str(), &lower_range_melee_temp, 0, 99, "%d%%")) {
            Config::Settings::weapon_lower_range.SetValue(lower_range_melee_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_LowerRangeMelee.c_str());

        //magic ranges
        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::SliderInt(Button_UpperRangeMagic.c_str(), &upper_range_mage_temp, 0, 99, "%d%%")) {
            Config::Settings::magic_upper_range.SetValue(upper_range_mage_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_UpperRangeMagic.c_str());
        ImGui::SameLine();

        ImGui::SetNextItemWidth(200.0f);
        if (ImGui::SliderInt(Button_LowerRangeMagic.c_str(), &lower_range_mage_temp, 0, 99, "%d%%")) {
            Config::Settings::magic_lower_range.SetValue(lower_range_mage_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_LowerRangeMagic.c_str());
        
        //resistance change
        FontAwesome::PushSolid();
        ImGui::NewLine();
        ImGui::Text(Title_Resist.c_str());
        ImGui::SetNextItemWidth(300.0f);
        if (ImGui::SliderFloat(Button_ResistanceChangeValue.c_str(), &resist_change_temp, 0.0, 50.0f, "%.2f%%")) {
            Config::Settings::resist_reduction_value.SetValue(resist_change_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_ResistanceChange.c_str());

        //Save Config and Default Settings
        FontAwesome::PushSolid();
        ImGui::NewLine();
        ImGui::Text(Title_System.c_str());
        if (ImGui::Button(Button_SaveSettings.c_str())) {
            Config::Settings::GetSingleton()->SaveSettings();
        }
        ImGui::SameLine();
        if (ImGui::Button(Button_ResetSettings.c_str())) {
            STWEAKSMenu::RestoreDefaults();
        }
        FontAwesome::Pop();
    }
    void __stdcall STWEAKSMenu::RenderToggles()
    {
        
        FontAwesome::PushSolid();

        ImGui::Text(Button_Toggles.c_str());
        ImGui::SameLine();
        HelpMarker(Tooltip_TogglesGeneral.c_str());

        ImGui::NewLine();
        ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), NOTE_Restart.c_str());
        ImGui::NewLine();

        if (ImGui::Checkbox(Button_DamageRanges_Toggles.c_str(), &damage_ranges_temp)) {
            Config::Settings::enable_damage_ranges.SetValue(damage_ranges_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_DamageRanges_Toggles.c_str());
        ImGui::SameLine();
        if (ImGui::Checkbox(Button_EtherealChange_Toggles.c_str(), &set_ethereal_change_temp)) {
            Config::Settings::enable_etheral_change.SetValue(set_ethereal_change_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_EtherealChange_Toggles.c_str());

        if (ImGui::Checkbox(Button_SneakJump_Toggles.c_str(), &sneak_jump_limit_temp)) {
            Config::Settings::enable_sneak_jump_limit.SetValue(sneak_jump_limit_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_SneakJump_Toggles.c_str());
        ImGui::SameLine();
        if (ImGui::Checkbox(Button_MassBased_Toggles.c_str(), &mass_based_jump_temp)) {
            Config::Settings::enable_mass_based_jump_height.SetValue(mass_based_jump_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_MassBased_Toggles.c_str());

        if (ImGui::Checkbox(Button_SneakStamina_Toggles.c_str(), &sneak_stamina_temp)) {
            Config::Settings::enable_sneak_stamina.SetValue(sneak_stamina_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_SneakStamina_Toggles.c_str());

        ImGui::SameLine();
        if (ImGui::Checkbox(Button_FadeOutActors_Toggles.c_str(), &fade_out_actors_temp)) {
            Config::Settings::enable_fading_actors.SetValue(fade_out_actors_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_FadeOutActors_Toggles.c_str());

        if (ImGui::Checkbox(Button_QuestItemNerf_Toggles.c_str(), &quest_item_nerf_temp)) {
            Config::Settings::enable_quest_item_nerf.SetValue(quest_item_nerf_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_QuestItemNerf_Toggles.c_str());
        ImGui::SameLine();
        if (ImGui::Checkbox(Button_Curses_Toggles.c_str(), &toggle_curses_temp)) {
            Config::Settings::enable_diseases.SetValue(toggle_curses_temp);
            Utility::Curses::CleanseCurse(Cache::GetPlayerSingleton());
            
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_Curses_Toggles.c_str());

        if (ImGui::Checkbox(Button_ResistChange_Toggles.c_str(), &toggle_resist_change_temp)) {
            Config::Settings::enable_resist_changes.SetValue(toggle_resist_change_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_ResistChange_Toggles.c_str());
        ImGui::SameLine();
        if (ImGui::Checkbox(Button_InteruptCast_Toggles.c_str(), &interupt_cast_temp)) {
            Config::Settings::interupt_cast_on_hit.SetValue(interupt_cast_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_InteruptCast_Toggles.c_str());

        if (ImGui::Checkbox(Button_AttackStamina_Toggles.c_str(), &attack_stamina_cost_temp)) {
            Config::Settings::enable_attack_stamina.SetValue(attack_stamina_cost_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_AttackStamina_Toggles.c_str());
        ImGui::SameLine();
        if (ImGui::Checkbox(Button_MagicStamina_Toggles.c_str(), &spell_stamina_cost_temp)) {
            Config::Settings::enable_cast_stamina.SetValue(spell_stamina_cost_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_MagicStamina_Toggles.c_str());

        if (ImGui::Checkbox(Button_FollowerDamage_Toggles.c_str(), &follower_damage_temp)) {
            Config::Settings::enable_foll_change.SetValue(follower_damage_temp);
        }
        ImGui::SameLine();
        HelpMarker(Tooltip_FollowerDamage_Toggles.c_str());


        //Save Config and Default Settings
        FontAwesome::PushSolid();
        ImGui::NewLine();
        ImGui::Text(Title_System.c_str());
        if (ImGui::Button(Button_SaveSettings.c_str())) {
            Config::Settings::GetSingleton()->SaveSettings();
            //Config::Settings::GetSingleton()->SaveToCustom();
        }
        ImGui::SameLine();
        if (ImGui::Button(Button_ResetSettings.c_str())) {
            STWEAKSMenu::SetToggleDefaults();
        }
        FontAwesome::Pop();

    }
    void STWEAKSMenu::SetToggleDefaults()
    {
        using set = Config::Settings;

        damage_ranges_temp = true;
        set::enable_damage_ranges.SetValue(damage_ranges_temp);
        sneak_jump_limit_temp = true;
        set::enable_sneak_jump_limit.SetValue(sneak_jump_limit_temp);
        set_ethereal_change_temp = true;
        set::enable_etheral_change.SetValue(set_ethereal_change_temp);
        mass_based_jump_temp = true;
        set::enable_mass_based_jump_height.SetValue(mass_based_jump_temp);
        sneak_stamina_temp = true;
        set::enable_sneak_stamina.SetValue(sneak_stamina_temp);
        fade_out_actors_temp = true;
        set::enable_fading_actors.SetValue(fade_out_actors_temp);
        quest_item_nerf_temp = true;
        set::enable_quest_item_nerf.SetValue(quest_item_nerf_temp);
        toggle_curses_temp = true;
        set::enable_diseases.SetValue(toggle_curses_temp);
        toggle_resist_change_temp = false;
        set::enable_resist_changes.SetValue(toggle_resist_change_temp);
        spell_stamina_cost_temp = true;
        set::enable_cast_stamina.SetValue(spell_stamina_cost_temp);
        attack_stamina_cost_temp = true;
        set::enable_attack_stamina.SetValue(attack_stamina_cost_temp);
        interupt_cast_temp = true;
        set::interupt_cast_on_hit.SetValue(interupt_cast_temp);
        follower_damage_temp = true;
        set::enable_foll_change.SetValue(follower_damage_temp);
    }
    void STWEAKSMenu::LoadTogglesFromSettings()
    {
        using set = Config::Settings;
       
        damage_ranges_temp = set::enable_damage_ranges.GetValue();
        sneak_jump_limit_temp = set::enable_sneak_jump_limit.GetValue();
        set_ethereal_change_temp = set::enable_etheral_change.GetValue();
        mass_based_jump_temp = set::enable_mass_based_jump_height.GetValue();
        sneak_stamina_temp = set::enable_sneak_stamina.GetValue();
        fade_out_actors_temp = set::enable_fading_actors.GetValue();
        quest_item_nerf_temp = set::enable_quest_item_nerf.GetValue();
        toggle_curses_temp = set::enable_diseases.GetValue();
        toggle_resist_change_temp = set::enable_resist_changes.GetValue();
        spell_stamina_cost_temp = set::enable_cast_stamina.GetValue();
        attack_stamina_cost_temp = set::enable_attack_stamina.GetValue();
        interupt_cast_temp = set::interupt_cast_on_hit.GetValue();
        follower_damage_temp = set::enable_foll_change.GetValue();
    }
}

