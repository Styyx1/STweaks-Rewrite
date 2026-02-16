#include "formloader.h"

void Forms::FormLoader::PopulateCurseList()
{
    curse_list.clear();    
    if (health_curse)
        curse_list.push_back(health_curse);
    if (stamina_curse)
        curse_list.push_back(stamina_curse);
    if (magicka_curse)
        curse_list.push_back(magicka_curse);
    if (silence_curse)
        curse_list.push_back(silence_curse);
    if (melee_damage_curse)
        curse_list.push_back(melee_damage_curse);
    if (bow_damage_curse)
        curse_list.push_back(bow_damage_curse);
    if (jump_curse)
        curse_list.push_back(jump_curse);
}

void Forms::FormLoader::AddFormListEntriesToCurseVector()
{
    if (!curses_formlist || curses_formlist->forms.empty())
    {
        return;
    }

    for (const auto& form : curses_formlist->forms)
    {
        if (!form) {
            continue;
        }
        if (auto spell = form->As<RE::SpellItem>(); spell)
        {
            curse_list.push_back(spell);
            REX::DEBUG("Added {} to curse vector", spell->GetName());
        }
	}
}

void Forms::FormLoader::LoadForms()
{
    const auto dh = RE::TESDataHandler::GetSingleton();
    
    if (!MiscUtil::IsModLoaded(stweaks::mod_name))
    {
        REX::FAIL("{} not found, The plugin is needed for the mod to work.", stweaks::mod_name);
    }
    sneak_stamina_spell = dh->LookupForm<RE::SpellItem>(stweaks::sneak_stamina_spell_ID, stweaks::mod_name);
    tall_grass_perk = dh->LookupForm<RE::BGSPerk>(stweaks::tall_grass_perk_ID, stweaks::mod_name);
    exhaustion_spell = dh->LookupForm<RE::SpellItem>(stweaks::exhaustion_spell_ID, stweaks::mod_name);
    
    if (!sneak_stamina_spell|| !tall_grass_perk|| !exhaustion_spell)
    {
        REX::FAIL("Data from {} is invalid, please make sure to not modify the base file.", stweaks::mod_name);
    }
    
    if (!MiscUtil::IsModLoaded(stweaks::diseases_name))
    {
        REX::CRITICAL("{} not found, please either enable the esp or disable the diseases in the toml file of this mod. Curses are auto-disabled now!");
        Config::Settings::enable_diseases.SetValue(false);
    }
    else
    {
        health_curse = dh->LookupForm<RE::SpellItem>(stweaks::health_curse_ID, stweaks::diseases_name);
        stamina_curse = dh->LookupForm<RE::SpellItem>(stweaks::stamina_curse_ID, stweaks::diseases_name);
        magicka_curse = dh->LookupForm<RE::SpellItem>(stweaks::magicka_curse_ID, stweaks::diseases_name);
        silence_curse = dh->LookupForm<RE::SpellItem>(stweaks::silence_curse_ID, stweaks::diseases_name);
        melee_damage_curse = dh->LookupForm<RE::SpellItem>(stweaks::melee_weakness_curse_ID, stweaks::diseases_name);
        bow_damage_curse = dh->LookupForm<RE::SpellItem>(stweaks::bow_weakness_curse_ID, stweaks::diseases_name);
        jump_curse = dh->LookupForm<RE::SpellItem>(stweaks::jump_curse_ID, stweaks::diseases_name);
        spell_allow_list = dh->LookupForm<RE::BGSListForm>(stweaks::spell_allow_list_ID, stweaks::diseases_name);
        curses_formlist = dh->LookupForm<RE::BGSListForm>(stweaks::curses_formlist_ID, stweaks::diseases_name);

        PopulateCurseList();
        AddFormListEntriesToCurseVector();

        disease_mod_active = true;
    }
}
