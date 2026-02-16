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
        curse_swap_cooldown = static_cast<float>(set::curse_swap_cooldown.GetValue());
        upper_range_melee = set::weapon_upper_range.GetValue();
        upper_range_magic = set::magic_upper_range.GetValue();
        lower_range_melee = set::weapon_lower_range.GetValue();
        lower_range_magic = set::magic_lower_range.GetValue();
        curse_swap_toggle = set::allow_curse_swapping.GetValue();
        resistance_change_value = static_cast<float>(set::resist_reduction_value.GetValue());
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
        auto_attributes = set::enable_automatic_attributes.GetValue();
        mass_equipment = set::enable_mass_equip_changes.GetValue();
        tall_grass_sneak = set::tall_grass_sneak.GetValue();
        attacks_of_opp = set::attacks_of_opp.GetValue();
		use_exhaustion = set::use_exhaustion.GetValue();
    }
    if (attributes)
    {
        using namespace UI::Attributes::vars;
        stamina_attack = static_cast<float>(set::base_stamina_cost_attacks.GetValue());
        stamina_magic = static_cast<float>(set::magic_stamina_cost_divider.GetValue());
        sneak_stamina = set::enable_sneak_stamina.GetValue();
        stamina_regen_toggle = set::stamina_regen_changes.GetValue();
        magicka_regen_toggle = set::magicka_regen_changes.GetValue();
        attack_stamina_toggle = set::enable_attack_stamina.GetValue();
        magic_stamina_toggle = set::enable_cast_stamina.GetValue();
        base_for_stamina_regen = static_cast<float>(set::stamina_regen_base_calc.GetValue());
        base_for_magicka_regen = static_cast<float>(set::magicka_regen_base_calc.GetValue());
        min_cast_speed = static_cast<float>(set::min_cast_speed.GetValue());
        max_cast_speed = static_cast<float>(set::max_cast_speed.GetValue());
        toggle_cast_speed = set::enable_skill_based_cast_speed.GetValue();
        jump_stamina_cost = set::jump_stamina_cost.GetValue();
        vanilla_attribute_leveling = set::vanilla_attribute_leveling.GetValue();
        MiscUtil::SetGMST("iAVDhmsLevelUp", vanilla_attribute_leveling);

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
        curses = false;
        resist_change = false;
        interupt_cast = true;
        follower_damage = true;
        one_shot_protec = true;
        enable_damage_caps = true;
        level_up_lows = true;
        auto_attributes = true;
        mass_equipment = true;
        tall_grass_sneak = true;
        attacks_of_opp = true;
		use_exhaustion = true;

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
        set::enable_automatic_attributes.SetValue(auto_attributes);
        set::enable_mass_equip_changes.SetValue(mass_equipment);
        set::tall_grass_sneak.SetValue(tall_grass_sneak);
        set::attacks_of_opp.SetValue(attacks_of_opp);
		set::use_exhaustion.SetValue(use_exhaustion);
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
        jump_stamina_cost = true;
        vanilla_attribute_leveling = 10;

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
        set::jump_stamina_cost.SetValue(jump_stamina_cost);
        set::vanilla_attribute_leveling.SetValue(vanilla_attribute_leveling);
        MiscUtil::SetGMST("iAVDhmsLevelUp", vanilla_attribute_leveling);
    }

    Config::Settings::GetSingleton()->UpdateSettings(true);
}

// https://github.com/QTR-Modding/SaveManagerSKSE/blob/af53d32f57a3fc4a0c0e22828b66e4592338826e/src/UI.cpp#L3
void HelpMarker(const char *desc)
{
    ImGuiMCP::TextDisabled("(?)");
    if (ImGuiMCP::BeginItemTooltip())
    {
        ImGuiMCP::PushTextWrapPos(ImGuiMCP::GetFontSize() * 35.0f);
        ImGuiMCP::TextUnformatted(desc);
        ImGuiMCP::PopTextWrapPos();
        ImGuiMCP::EndTooltip();
    }
}

namespace Settings
{
void __stdcall RenderSettings()
{
    FontAwesome::PushSolid();

    // === Jump Height ===
    ImGuiMCP::NewLine();
    ImGuiMCP::SeparatorText(Titles::jump_height.c_str());

    ImGuiMCP::SetNextItemWidth(300.0f);
    if (ImGuiMCP::SliderFloat(Label::jump_height_mod.c_str(), &vars::jump_height_mod, 0.0, 1.0, "%.2f"))
    {
        Config::Settings::sneak_height_modifier.SetValue(vars::jump_height_mod);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::jump_height_mod.c_str());

    // === Curse Settings ===
    ImGuiMCP::NewLine();
    ImGuiMCP::SeparatorText(Titles::curses.c_str());

    if (ImGuiMCP::Checkbox(Label::curse_swap_toggle.c_str(), &vars::curse_swap_toggle))
    {
        Config::Settings::allow_curse_swapping.SetValue(vars::curse_swap_toggle);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::curse_swap_toggle.c_str());

    ImGuiMCP::SetNextItemWidth(200.0);
    if (ImGuiMCP::SliderFloat(Label::curse_chance.c_str(), &vars::curse_chance, 0.0, 100.0f, "%.2f%%"))
    {
        Config::Settings::curse_chance.SetValue(vars::curse_chance);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::curse_chance.c_str());

    ImGuiMCP::SetNextItemWidth(200.0);
    if (ImGuiMCP::SliderFloat(Label::curse_swap_cooldown.c_str(), &vars::curse_swap_cooldown, 3.0, 120.0, "%.2fsec"))
    {
        Config::Settings::curse_swap_cooldown.SetValue(vars::curse_swap_cooldown);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::curse_swap_cooldown.c_str());

    // === Damage Ranges ===
    ImGuiMCP::NewLine();
    ImGuiMCP::SeparatorText(Titles::ranges.c_str());

    ImGuiMCP::SetNextItemWidth(200.0f);
    if (ImGuiMCP::SliderInt(Label::upper_range_melee.c_str(), &vars::upper_range_melee, 0, 99, "%d%%"))
    {
        Config::Settings::weapon_upper_range.SetValue(vars::upper_range_melee);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::upper_range_melee.c_str());

    ImGuiMCP::SameLine();
    ImGuiMCP::SetNextItemWidth(200.0f);
    if (ImGuiMCP::SliderInt(Label::lower_range_melee.c_str(), &vars::lower_range_melee, 0, 99, "%d%%"))
    {
        Config::Settings::weapon_lower_range.SetValue(vars::lower_range_melee);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::lower_range_melee.c_str());

    ImGuiMCP::SetNextItemWidth(200.0f);
    if (ImGuiMCP::SliderInt(Label::upper_range_magic.c_str(), &vars::upper_range_magic, 0, 99, "%d%%"))
    {
        Config::Settings::magic_upper_range.SetValue(vars::upper_range_magic);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::upper_range_magic.c_str());

    ImGuiMCP::SameLine();
    ImGuiMCP::SetNextItemWidth(200.0f);
    if (ImGuiMCP::SliderInt(Label::lower_range_magic.c_str(), &vars::lower_range_magic, 0, 99, "%d%%"))
    {
        Config::Settings::magic_lower_range.SetValue(vars::lower_range_magic);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::lower_range_magic.c_str());

    // === Resistance Change ===
    ImGuiMCP::NewLine();
    ImGuiMCP::SeparatorText(Titles::resist.c_str());

    ImGuiMCP::SetNextItemWidth(300.0f);
    if (ImGuiMCP::SliderFloat(Label::resistance_change_value.c_str(), &vars::resistance_change_value, 0.0, 50.0f,
                           "%.2f%%"))
    {
        Config::Settings::resist_reduction_value.SetValue(vars::resistance_change_value);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::resistance_change_value.c_str());

    // === Save / Reset System ===
    ImGuiMCP::NewLine();
    ImGuiMCP::SeparatorText(Titles::system.c_str());

    if (ImGuiMCP::Button(Label::save_settings.c_str()))
    {
        Config::Settings::GetSingleton()->UpdateSettings(true);
    }

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Button(Label::restore_defaults.c_str()))
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
    ImGuiMCP::SeparatorText(Titles::toggles.c_str());
    ImGuiMCP::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), Titles::system_note.c_str());
    ImGuiMCP::NewLine();

    // === Damage Range & Ethereal Changes ===
    if (ImGuiMCP::Checkbox(Label::damage_ranges.c_str(), &vars::damage_ranges))
        Config::Settings::enable_damage_ranges.SetValue(vars::damage_ranges);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::damage_ranges.c_str());

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Checkbox(Label::ethereal_change.c_str(), &vars::ethereal_change))
        Config::Settings::enable_etheral_change.SetValue(vars::ethereal_change);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::ethereal_change.c_str());

    // === Sneak Jump & Mass Based Jump ===
    if (ImGuiMCP::Checkbox(Label::sneak_jump_limit.c_str(), &vars::sneak_jump_limit))
        Config::Settings::enable_sneak_jump_limit.SetValue(vars::sneak_jump_limit);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::sneak_jump_limit.c_str());

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Checkbox(Label::mass_based_jump.c_str(), &vars::mass_based_jump))
        Config::Settings::enable_mass_based_jump_height.SetValue(vars::mass_based_jump);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::mass_based_jump.c_str());

    // === Quest Item Nerf & Curses ===
    if (ImGuiMCP::Checkbox(Label::quest_item_nerf.c_str(), &vars::quest_item_nerf))
        Config::Settings::enable_quest_item_nerf.SetValue(vars::quest_item_nerf);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::quest_item_nerf.c_str());

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Checkbox(Label::curses.c_str(), &vars::curses))
    {
        Config::Settings::enable_diseases.SetValue(vars::curses);
        Utility::Curses::CleanseCurse(Cache::GetPlayerSingleton());
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::curses.c_str());

    // === Resistance & Spell/Attack Toggles ===
    if (ImGuiMCP::Checkbox(Label::resist_change.c_str(), &vars::resist_change))
        Config::Settings::enable_resist_changes.SetValue(vars::resist_change);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::resist_change.c_str());

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Checkbox(Label::interupt_cast.c_str(), &vars::interupt_cast))
        Config::Settings::interupt_cast_on_hit.SetValue(vars::interupt_cast);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::interupt_cast.c_str());

    // === Actor Fade Out & Follower Damage ===
    if (ImGuiMCP::Checkbox(Label::fade_out_actors.c_str(), &vars::fade_out_actors))
        Config::Settings::enable_fading_actors.SetValue(vars::fade_out_actors);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::fade_out_actors.c_str());
    ImGuiMCP::SameLine();

    if (ImGuiMCP::Checkbox(Label::follower_damage.c_str(), &vars::follower_damage))
        Config::Settings::enable_foll_change.SetValue(vars::follower_damage);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::follower_damage.c_str());

    // === One Shot Protection & Damage Caps ===
    if (ImGuiMCP::Checkbox(Label::one_shot_protec.c_str(), &vars::one_shot_protec))
    {
        Config::Settings::one_shot_protection.SetValue(vars::one_shot_protec);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::one_shot_protec.c_str());
    ImGuiMCP::SameLine();
    if (ImGuiMCP::Checkbox(Label::enable_damage_caps.c_str(), &vars::enable_damage_caps))
    {
        Config::Settings::enable_damage_caps.SetValue(vars::enable_damage_caps);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::enable_damage_caps.c_str());

    // === Level Up Low Levels & Auto Attributes===

    if (ImGuiMCP::Checkbox(Label::level_up_lows.c_str(), &vars::level_up_lows))
    {
        Config::Settings::level_up_low_levels.SetValue(vars::level_up_lows);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::level_up_lows.c_str());

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Checkbox(Label::auto_attributes.c_str(), &vars::auto_attributes))
        Config::Settings::enable_automatic_attributes.SetValue(vars::auto_attributes);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::auto_attributes.c_str());

    // === Mass Equipment Changes & Tall Grass Sneak ===
    if (ImGuiMCP::Checkbox(Label::mass_equipment.c_str(), &vars::mass_equipment))
        Config::Settings::enable_mass_equip_changes.SetValue(vars::mass_equipment);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::mass_equipment.c_str());

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Checkbox(Label::tall_grass_sneak.c_str(), &vars::tall_grass_sneak))
        Config::Settings::tall_grass_sneak.SetValue(vars::tall_grass_sneak);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::tall_grass_sneak.c_str());

    // === Attacks of Opportunity & Exhaustion===
    if (ImGuiMCP::Checkbox(Label::attacks_of_opp.c_str(), &vars::attacks_of_opp))
      Config::Settings::attacks_of_opp.SetValue(vars::attacks_of_opp);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::attacks_of_opp.c_str());

	ImGuiMCP::SameLine();
	if (ImGuiMCP::Checkbox(Label::use_exhaustion.c_str(), &vars::use_exhaustion))
		Config::Settings::use_exhaustion.SetValue(vars::use_exhaustion);
	ImGuiMCP::SameLine();
	HelpMarker(Tooltip::use_exhaustion.c_str());


    // === Save / Reset System ===
    ImGuiMCP::NewLine();
    ImGuiMCP::SeparatorText(Titles::system.c_str());

    if (ImGuiMCP::Button(Label::save_settings.c_str()))
        Config::Settings::GetSingleton()->UpdateSettings(true);

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Button(Label::restore_defaults.c_str()))
        RestoreDefaults(false, true);

    FontAwesome::Pop();
}

} // namespace Toggles

namespace Attributes
{
void __stdcall RenderAttributes()
{
    FontAwesome::PushSolid();
    ImGuiMCP::SeparatorText(Titles::attributes_header.c_str());
    ImGuiMCP::NewLine();

    ImGuiMCP::SeparatorText(Titles::stamina.c_str());

    ImGuiMCP::SetNextItemWidth(200.0f);
    if (ImGuiMCP::SliderFloat(Label::stamina_attack.c_str(), &vars::stamina_attack, 3.0, 50.0, "%.2f"))
    {
        Config::Settings::base_stamina_cost_attacks.SetValue(vars::stamina_attack);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::stamina_attack.c_str());

    ImGuiMCP::SameLine();
    ImGuiMCP::SetNextItemWidth(200.0f);
    if (ImGuiMCP::SliderFloat(Label::stamina_magic.c_str(), &vars::stamina_magic, 0.1, 10.0f, "%.2f"))
    {
        Config::Settings::magic_stamina_cost_divider.SetValue(vars::stamina_magic);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::stamina_magic.c_str());

    if (ImGuiMCP::Checkbox(Label::attack_stamina_toggle.c_str(), &vars::attack_stamina_toggle))
        Config::Settings::enable_attack_stamina.SetValue(vars::attack_stamina_toggle);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::attack_stamina_toggle.c_str());

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Checkbox(Label::magic_stamina_toggle.c_str(), &vars::magic_stamina_toggle))
        Config::Settings::enable_cast_stamina.SetValue(vars::magic_stamina_toggle);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::magic_stamina_toggle.c_str());

    if (ImGuiMCP::Checkbox(Label::sneak_stamina.c_str(), &vars::sneak_stamina))
        Config::Settings::enable_sneak_stamina.SetValue(vars::sneak_stamina);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::sneak_stamina.c_str());

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Checkbox(Label::jump_stamina_cost.c_str(), &vars::jump_stamina_cost))
        Config::Settings::jump_stamina_cost.SetValue(vars::jump_stamina_cost);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::jump_stamina_cost.c_str());

    ImGuiMCP::NewLine();
    ImGuiMCP::SeparatorText(Titles::regen.c_str());

    if (ImGuiMCP::Checkbox(Label::stamina_regen_toggle.c_str(), &vars::stamina_regen_toggle))
        Config::Settings::stamina_regen_changes.SetValue(vars::stamina_regen_toggle);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::stamina_regen_toggle.c_str());

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Checkbox(Label::magicka_regen_toggle.c_str(), &vars::magicka_regen_toggle))
        Config::Settings::magicka_regen_changes.SetValue(vars::magicka_regen_toggle);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::magicka_regen_toggle.c_str());

    ImGuiMCP::SetNextItemWidth(200.0f);
    if (ImGuiMCP::SliderFloat(Label::base_for_stamina_regen.c_str(), &vars::base_for_stamina_regen, 10.0f, 500.0f, "%.2f"))
        Config::Settings::stamina_regen_base_calc.SetValue(vars::base_for_stamina_regen);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::base_for_stamina_regen.c_str());

    ImGuiMCP::SameLine();
    ImGuiMCP::SetNextItemWidth(200.0f);
    if (ImGuiMCP::SliderFloat(Label::base_for_magicka_regen.c_str(), &vars::base_for_magicka_regen, 10.0f, 500.0f, "%.2f"))
        Config::Settings::magicka_regen_base_calc.SetValue(vars::base_for_magicka_regen);
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::base_for_magicka_regen.c_str());

    ImGuiMCP::NewLine();
    ImGuiMCP::Separator();
    ImGuiMCP::Text(Titles::magicka.c_str());

    ImGuiMCP::SetNextItemWidth(200.0f);
    if (ImGuiMCP::SliderFloat(Label::min_cast_speed.c_str(), &vars::min_cast_speed, 0.05, vars::max_cast_speed, " % .2f "))
    {
        Config::Settings::min_cast_speed.SetValue(vars::min_cast_speed);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::min_cast_speed.c_str());

    ImGuiMCP::SameLine();
    ImGuiMCP::SetNextItemWidth(200.f);
    if (ImGuiMCP::SliderFloat(Label::max_cast_speed.c_str(), &vars::max_cast_speed, vars::min_cast_speed, 7.0f, " % .2f "))
    {
        Config::Settings::max_cast_speed.SetValue(vars::max_cast_speed);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::max_cast_speed.c_str());

    if (ImGuiMCP::Checkbox(Label::toggle_cast_speed.c_str(), &vars::toggle_cast_speed))
    {
        Config::Settings::enable_skill_based_cast_speed.SetValue(vars::toggle_cast_speed);
    }
    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::toggle_cast_speed.c_str());


    ImGuiMCP::SetNextItemWidth(300.f);
    if (ImGuiMCP::SliderInt(Label::vanilla_attribute_leveling.c_str(), &vars::vanilla_attribute_leveling, 0, 10))
    {
        Config::Settings::vanilla_attribute_leveling.SetValue(vars::vanilla_attribute_leveling);
    }

    ImGuiMCP::SameLine();
    HelpMarker(Tooltip::vanilla_attribute_leveling.c_str());

    // === Save / Reset System ===
    ImGuiMCP::NewLine();
    ImGuiMCP::SeparatorText(Titles::system.c_str());

    if (ImGuiMCP::Button(Label::save_settings.c_str())) {
      Config::Settings::GetSingleton()->UpdateSettings(true);
      MiscUtil::SetGMST("iAVDhmsLevelUp", vars::vanilla_attribute_leveling);
    }
        

    ImGuiMCP::SameLine();
    if (ImGuiMCP::Button(Label::restore_defaults.c_str()))
        RestoreDefaults(false, false, true);

    FontAwesome::Pop();
}
} // namespace Attributes
} // namespace UI
