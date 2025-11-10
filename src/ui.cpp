#include "ui.h"
#include "cache.h"
#include "utility.h"

namespace UI
{
void Register()
{
    if (!SKSEMenuFramework::IsInstalled())
    {
        return;
    }
    SKSEMenuFramework::SetSection(Titles::MOD_NAME);
    SKSEMenuFramework::AddSectionItem(Titles::SETTING_TAB, Settings::RenderSettings);
    SKSEMenuFramework::AddSectionItem(Titles::TOGGLE_TAB, Toggles::RenderToggles);
    SKSEMenuFramework::AddSectionItem(Titles::STAMINA_SYSTEM, Attributes::RenderAttributes);
    RestoreFromSettings(true, true, true);
}

void RestoreFromSettings(bool settings, bool toggles, bool attributes)
{
    using set = Config::Settings;
    if (settings)
    {
        using namespace UI::Settings::vars;

        jump_height_mod = set::sneak_height_modifier.GetValue();
        curse_chance = set::curse_chance.GetValue();
        curse_swap_cooldown = set::curse_swap_cooldown.GetValue();
        upper_range_melee = set::weapon_upper_range.GetValue();
        upper_range_magic = set::magic_upper_range.GetValue();
        lower_range_melee = set::weapon_lower_range.GetValue();
        lower_range_magic = set::magic_lower_range.GetValue();
        curse_swap_toggle = set::allow_curse_swapping.GetValue();
        resistance_change_value = set::resist_reduction_value.GetValue();
    }
    if (toggles)
    {
        using namespace UI::Toggles::vars;
        damage_ranges = set::enable_damage_ranges.GetValue();
        sneak_jump_limit = set::enable_sneak_jump_limit.GetValue();
        ethereal_change = set::enable_etheral_change.GetValue();
        mass_based_jump = set::enable_mass_based_jump_height.GetValue();
        fade_out_actors = set::enable_fading_actors.GetValue();
        quest_item_nerf = set::enable_quest_item_nerf.GetValue();
        curses = set::enable_diseases.GetValue();
        resist_change = set::enable_resist_changes.GetValue();
        interupt_cast = set::interupt_cast_on_hit.GetValue();
        follower_damage = set::enable_foll_change.GetValue();
        one_shot_protec = set::one_shot_protection.GetValue();
        enable_damage_caps = set::enable_damage_caps.GetValue();
        level_up_lows = set::level_up_low_levels.GetValue();
    }
    if (attributes)
    {
        using namespace UI::Attributes;
        vars::stamina_attack = set::base_stamina_cost_attacks.GetValue();
        vars::stamina_magic = set::magic_stamina_cost_divider.GetValue();
        vars::sneak_stamina = set::enable_sneak_stamina.GetValue();
        vars::stamina_regen_toggle = set::stamina_regen_changes.GetValue();
        vars::magicka_regen_toggle = set::magicka_regen_changes.GetValue();
        vars::attack_stamina_toggle = set::enable_attack_stamina.GetValue();
        vars::magic_stamina_toggle = set::enable_cast_stamina.GetValue();
        vars::base_for_stamina_regen = set::stamina_regen_base_calc.GetValue();
        vars::base_for_magicka_regen = set::magicka_regen_base_calc.GetValue();
        vars::min_cast_speed = set::min_cast_speed.GetValue();
        vars::max_cast_speed = set::max_cast_speed.GetValue();
        vars::toggle_cast_speed = set::enable_skill_based_cast_speed.GetValue();
    }
}

void RestoreDefaults(bool settings, bool toggles, bool attributes)
{
    using set = Config::Settings;
    if (settings)
    {
        using namespace UI::Settings::vars;

        jump_height_mod = 0.55;
        curse_chance = 1.0f;
        curse_swap_cooldown = 60.0;
        upper_range_melee = 15;
        upper_range_magic = 15;
        lower_range_melee = 15;
        lower_range_magic = 15;
        curse_swap_toggle = true;
        resistance_change_value = 20.0f;

        set::sneak_height_modifier.SetValue(jump_height_mod);
        set::curse_chance.SetValue(curse_chance);
        set::curse_swap_cooldown.SetValue(curse_swap_cooldown);
        set::weapon_upper_range.SetValue(upper_range_melee);
        set::magic_upper_range.SetValue(upper_range_magic);
        set::weapon_lower_range.SetValue(lower_range_melee);
        set::magic_lower_range.SetValue(lower_range_magic);
        set::allow_curse_swapping.SetValue(curse_swap_toggle);
        set::resist_reduction_value.SetValue(resistance_change_value);
    }
    if (toggles)
    {
        using namespace UI::Toggles::vars;
        damage_ranges = true;
        sneak_jump_limit = true;
        ethereal_change = true;
        mass_based_jump = true;
        fade_out_actors = true;
        quest_item_nerf = true;
        curses = true;
        resist_change = false;
        interupt_cast = true;
        follower_damage = true;
        one_shot_protec = true;
        enable_damage_caps = true;
        level_up_lows = true;

        set::enable_damage_ranges.SetValue(damage_ranges);
        set::enable_sneak_jump_limit.SetValue(sneak_jump_limit);
        set::enable_etheral_change.SetValue(ethereal_change);
        set::enable_mass_based_jump_height.SetValue(mass_based_jump);
        set::enable_fading_actors.SetValue(fade_out_actors);
        set::enable_quest_item_nerf.SetValue(quest_item_nerf);
        set::enable_diseases.SetValue(curses);
        set::enable_resist_changes.SetValue(resist_change);
        set::interupt_cast_on_hit.SetValue(interupt_cast);
        set::enable_foll_change.SetValue(follower_damage);
        set::one_shot_protection.SetValue(one_shot_protec);
        set::enable_damage_caps.SetValue(enable_damage_caps);
        set::level_up_low_levels.SetValue(level_up_lows);
    }

    if (attributes)
    {
        using namespace UI::Attributes::vars;

        stamina_attack = 20.0f;
        stamina_magic = 2.0;
        magic_stamina_toggle = true;
        attack_stamina_toggle = true;
        sneak_stamina = true;
        stamina_regen_toggle = true;
        magicka_regen_toggle = true;
        base_for_stamina_regen = 150.0;
        base_for_magicka_regen = 150.0;
        min_cast_speed = 0.5;
        max_cast_speed = 3.0;
        toggle_cast_speed = true;

        set::base_stamina_cost_attacks.SetValue(stamina_attack);
        set::magic_stamina_cost_divider.SetValue(stamina_magic);
        set::enable_cast_stamina.SetValue(magic_stamina_toggle);
        set::enable_attack_stamina.SetValue(attack_stamina_toggle);
        set::enable_sneak_stamina.SetValue(sneak_stamina);
        set::stamina_regen_changes.SetValue(stamina_regen_toggle);
        set::magicka_regen_changes.SetValue(magicka_regen_toggle);
        set::stamina_regen_base_calc.SetValue(base_for_stamina_regen);
        set::magicka_regen_base_calc.SetValue(base_for_magicka_regen);
        set::min_cast_speed.SetValue(min_cast_speed);
        set::max_cast_speed.SetValue(max_cast_speed);
        set::enable_skill_based_cast_speed.SetValue(toggle_cast_speed);
    }

    Config::Settings::GetSingleton()->UpdateSettings(true);
}

// https://github.com/QTR-Modding/SaveManagerSKSE/blob/af53d32f57a3fc4a0c0e22828b66e4592338826e/src/UI.cpp#L3
void HelpMarker(const char *desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

namespace Settings
{
void __stdcall RenderSettings()
{
    FontAwesome::PushSolid();

    //
    // === Jump Height ===
    //
    ImGui::NewLine();
    ImGui::SeparatorText(Titles::jump_height.c_str());

    ImGui::SetNextItemWidth(300.0f);
    if (ImGui::SliderFloat(Label::jump_height_mod.c_str(), &vars::jump_height_mod, 0.0, 1.0, "%.2f"))
    {
        Config::Settings::sneak_height_modifier.SetValue(vars::jump_height_mod);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::jump_height_mod.c_str());

    //
    // === Curse Settings ===
    //
    ImGui::NewLine();
    ImGui::SeparatorText(Titles::curses.c_str());

    if (ImGui::Checkbox(Label::curse_swap_toggle.c_str(), &vars::curse_swap_toggle))
    {
        Config::Settings::allow_curse_swapping.SetValue(vars::curse_swap_toggle);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::curse_swap_toggle.c_str());

    ImGui::SetNextItemWidth(200.0);
    if (ImGui::SliderFloat(Label::curse_chance.c_str(), &vars::curse_chance, 0.0, 100.0f, "%.2f%%"))
    {
        Config::Settings::curse_chance.SetValue(vars::curse_chance);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::curse_chance.c_str());

    ImGui::SetNextItemWidth(200.0);
    if (ImGui::SliderFloat(Label::curse_swap_cooldown.c_str(), &vars::curse_swap_cooldown, 3.0, 120.0, "%.2fsec"))
    {
        Config::Settings::curse_swap_cooldown.SetValue(vars::curse_swap_cooldown);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::curse_swap_cooldown.c_str());

    //
    // === Damage Ranges ===
    //
    ImGui::NewLine();
    ImGui::SeparatorText(Titles::ranges.c_str());

    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::SliderInt(Label::upper_range_melee.c_str(), &vars::upper_range_melee, 0, 99, "%d%%"))
    {
        Config::Settings::weapon_upper_range.SetValue(vars::upper_range_melee);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::upper_range_melee.c_str());

    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::SliderInt(Label::lower_range_melee.c_str(), &vars::lower_range_melee, 0, 99, "%d%%"))
    {
        Config::Settings::weapon_lower_range.SetValue(vars::lower_range_melee);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::lower_range_melee.c_str());

    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::SliderInt(Label::upper_range_magic.c_str(), &vars::upper_range_magic, 0, 99, "%d%%"))
    {
        Config::Settings::magic_upper_range.SetValue(vars::upper_range_magic);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::upper_range_magic.c_str());

    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::SliderInt(Label::lower_range_magic.c_str(), &vars::lower_range_magic, 0, 99, "%d%%"))
    {
        Config::Settings::magic_lower_range.SetValue(vars::lower_range_magic);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::lower_range_magic.c_str());

    //
    // === Resistance Change ===
    //
    ImGui::NewLine();
    ImGui::SeparatorText(Titles::resist.c_str());

    ImGui::SetNextItemWidth(300.0f);
    if (ImGui::SliderFloat(Label::resistance_change_value.c_str(), &vars::resistance_change_value, 0.0, 50.0f,
                           "%.2f%%"))
    {
        Config::Settings::resist_reduction_value.SetValue(vars::resistance_change_value);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::resistance_change_value.c_str());

    //
    // === Save / Reset System ===
    //
    ImGui::NewLine();
    ImGui::SeparatorText(Titles::system.c_str());

    if (ImGui::Button(Label::save_settings.c_str()))
    {
        Config::Settings::GetSingleton()->UpdateSettings(true);
    }

    ImGui::SameLine();
    if (ImGui::Button(Label::restore_defaults.c_str()))
    {
        RestoreDefaults(true, false);
    }

    FontAwesome::Pop();
}
} // namespace Settings
namespace Toggles
{
void __stdcall RenderToggles()
{

    FontAwesome::PushSolid();

    // === Toggles Title ===
    ImGui::SeparatorText(Titles::toggles.c_str());
    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), Titles::system_note.c_str());
    ImGui::NewLine();

    // === Damage Range & Ethereal Changes ===
    if (ImGui::Checkbox(Label::damage_ranges.c_str(), &vars::damage_ranges))
        Config::Settings::enable_damage_ranges.SetValue(vars::damage_ranges);
    ImGui::SameLine();
    HelpMarker(Tooltip::damage_ranges.c_str());

    ImGui::SameLine();
    if (ImGui::Checkbox(Label::ethereal_change.c_str(), &vars::ethereal_change))
        Config::Settings::enable_etheral_change.SetValue(vars::ethereal_change);
    ImGui::SameLine();
    HelpMarker(Tooltip::ethereal_change.c_str());

    // === Sneak Jump & Mass Based Jump ===
    if (ImGui::Checkbox(Label::sneak_jump_limit.c_str(), &vars::sneak_jump_limit))
        Config::Settings::enable_sneak_jump_limit.SetValue(vars::sneak_jump_limit);
    ImGui::SameLine();
    HelpMarker(Tooltip::sneak_jump_limit.c_str());

    ImGui::SameLine();
    if (ImGui::Checkbox(Label::mass_based_jump.c_str(), &vars::mass_based_jump))
        Config::Settings::enable_mass_based_jump_height.SetValue(vars::mass_based_jump);
    ImGui::SameLine();
    HelpMarker(Tooltip::mass_based_jump.c_str());

    // === Quest Item Nerf & Curses ===
    if (ImGui::Checkbox(Label::quest_item_nerf.c_str(), &vars::quest_item_nerf))
        Config::Settings::enable_quest_item_nerf.SetValue(vars::quest_item_nerf);
    ImGui::SameLine();
    HelpMarker(Tooltip::quest_item_nerf.c_str());

    ImGui::SameLine();
    if (ImGui::Checkbox(Label::curses.c_str(), &vars::curses))
    {
        Config::Settings::enable_diseases.SetValue(vars::curses);
        Utility::Curses::CleanseCurse(Cache::GetPlayerSingleton());
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::curses.c_str());

    // === Resistance & Spell/Attack Toggles ===
    if (ImGui::Checkbox(Label::resist_change.c_str(), &vars::resist_change))
        Config::Settings::enable_resist_changes.SetValue(vars::resist_change);
    ImGui::SameLine();
    HelpMarker(Tooltip::resist_change.c_str());

    ImGui::SameLine();
    if (ImGui::Checkbox(Label::interupt_cast.c_str(), &vars::interupt_cast))
        Config::Settings::interupt_cast_on_hit.SetValue(vars::interupt_cast);
    ImGui::SameLine();
    HelpMarker(Tooltip::interupt_cast.c_str());

    // === Actor Fade Out & Follower Damage ===
    if (ImGui::Checkbox(Label::fade_out_actors.c_str(), &vars::fade_out_actors))
        Config::Settings::enable_fading_actors.SetValue(vars::fade_out_actors);
    ImGui::SameLine();
    HelpMarker(Tooltip::fade_out_actors.c_str());
    ImGui::SameLine();

    if (ImGui::Checkbox(Label::follower_damage.c_str(), &vars::follower_damage))
        Config::Settings::enable_foll_change.SetValue(vars::follower_damage);
    ImGui::SameLine();
    HelpMarker(Tooltip::follower_damage.c_str());

    // === One Shot Protection & Damage Caps ===
    if (ImGui::Checkbox(Label::one_shot_protec.c_str(), &vars::one_shot_protec))
    {

        Config::Settings::one_shot_protection.SetValue(vars::one_shot_protec);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::one_shot_protec.c_str());
    ImGui::SameLine();
    if (ImGui::Checkbox(Label::enable_damage_caps.c_str(), &vars::enable_damage_caps))
    {
        Config::Settings::enable_damage_caps.SetValue(vars::enable_damage_caps);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::enable_damage_caps.c_str());

    // === Level Up Low Levels ===

    if (ImGui::Checkbox(Label::level_up_lows.c_str(), &vars::level_up_lows)) {
      Config::Settings::level_up_low_levels.SetValue(vars::level_up_lows);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::level_up_lows.c_str());




    // === Save / Reset System ===
    ImGui::NewLine();
    ImGui::SeparatorText(Titles::system.c_str());

    if (ImGui::Button(Label::save_settings.c_str()))
        Config::Settings::GetSingleton()->UpdateSettings(true);

    ImGui::SameLine();
    if (ImGui::Button(Label::restore_defaults.c_str()))
        RestoreDefaults(false, true);

    FontAwesome::Pop();
}

} // namespace Toggles

namespace Attributes
{
void __stdcall RenderAttributes()
{
    FontAwesome::PushSolid();
    ImGui::SeparatorText(Titles::attributes_header.c_str());
    ImGui::NewLine();

    ImGui::SeparatorText(Titles::stamina.c_str());

    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::SliderFloat(Label::stamina_attack.c_str(), &vars::stamina_attack, 3.0, 50.0, "%.2f"))
    {
        Config::Settings::base_stamina_cost_attacks.SetValue(vars::stamina_attack);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::stamina_attack.c_str());

    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::SliderFloat(Label::stamina_magic.c_str(), &vars::stamina_magic, 0.1, 10.0f, "%.2f"))
    {
        Config::Settings::magic_stamina_cost_divider.SetValue(vars::stamina_magic);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::stamina_magic.c_str());

    if (ImGui::Checkbox(Label::attack_stamina_toggle.c_str(), &vars::attack_stamina_toggle))
        Config::Settings::enable_attack_stamina.SetValue(vars::attack_stamina_toggle);
    ImGui::SameLine();
    HelpMarker(Tooltip::attack_stamina_toggle.c_str());

    ImGui::SameLine();
    if (ImGui::Checkbox(Label::magic_stamina_toggle.c_str(), &vars::magic_stamina_toggle))
        Config::Settings::enable_cast_stamina.SetValue(vars::magic_stamina_toggle);
    ImGui::SameLine();
    HelpMarker(Tooltip::magic_stamina_toggle.c_str());

    if (ImGui::Checkbox(Label::sneak_stamina.c_str(), &vars::sneak_stamina))
        Config::Settings::enable_sneak_stamina.SetValue(vars::sneak_stamina);
    ImGui::SameLine();
    HelpMarker(Tooltip::sneak_stamina.c_str());

    ImGui::NewLine();
    ImGui::SeparatorText(Titles::regen.c_str());

    if (ImGui::Checkbox(Label::stamina_regen_toggle.c_str(), &vars::stamina_regen_toggle))
        Config::Settings::stamina_regen_changes.SetValue(vars::stamina_regen_toggle);
    ImGui::SameLine();
    HelpMarker(Tooltip::stamina_regen_toggle.c_str());

    ImGui::SameLine();
    if (ImGui::Checkbox(Label::magicka_regen_toggle.c_str(), &vars::magicka_regen_toggle))
        Config::Settings::magicka_regen_changes.SetValue(vars::magicka_regen_toggle);
    ImGui::SameLine();
    HelpMarker(Tooltip::magicka_regen_toggle.c_str());

    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::SliderFloat(Label::base_for_stamina_regen.c_str(), &vars::base_for_stamina_regen, 10.0f, 500.0f, "%.2f"))
        Config::Settings::stamina_regen_base_calc.SetValue(vars::base_for_stamina_regen);
    ImGui::SameLine();
    HelpMarker(Tooltip::base_for_stamina_regen.c_str());

    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::SliderFloat(Label::base_for_magicka_regen.c_str(), &vars::base_for_magicka_regen, 10.0f, 500.0f, "%.2f"))
        Config::Settings::magicka_regen_base_calc.SetValue(vars::base_for_magicka_regen);
    ImGui::SameLine();
    HelpMarker(Tooltip::base_for_magicka_regen.c_str());

    ImGui::NewLine();
    ImGui::Separator();
    ImGui::Text(Titles::magicka.c_str());

    ImGui::SetNextItemWidth(200.0f);
    if (ImGui::SliderFloat(Label::min_cast_speed.c_str(), &vars::min_cast_speed, 0.05, vars::max_cast_speed,
                           " % .2fsec "))
    {
      Config::Settings::min_cast_speed.SetValue(vars::min_cast_speed);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::min_cast_speed.c_str());

    ImGui::SameLine();
    ImGui::SetNextItemWidth(200.f);
    if (ImGui::SliderFloat(Label::max_cast_speed.c_str(), &vars::max_cast_speed,
                           vars::min_cast_speed, 7.0f, " % .2fsec ")) {
      Config::Settings::max_cast_speed.SetValue(vars::max_cast_speed);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::max_cast_speed.c_str());

    if (ImGui::Checkbox(Label::toggle_cast_speed.c_str(),
                        &vars::toggle_cast_speed)) {
      Config::Settings::enable_skill_based_cast_speed.SetValue(
          vars::toggle_cast_speed);
    }
    ImGui::SameLine();
    HelpMarker(Tooltip::toggle_cast_speed.c_str());



    // === Save / Reset System ===
    ImGui::NewLine();
    ImGui::SeparatorText(Titles::system.c_str());

    if (ImGui::Button(Label::save_settings.c_str()))
        Config::Settings::GetSingleton()->UpdateSettings(true);

    ImGui::SameLine();
    if (ImGui::Button(Label::restore_defaults.c_str()))
        RestoreDefaults(false, false, true);

    FontAwesome::Pop();
}
} // namespace Attributes
} // namespace UI
