#include "formloader.h"

void Forms::FormLoader::PopulateCurseList()
{
    curse_list.push_back(health_curse);
    curse_list.push_back(stamina_curse);
    curse_list.push_back(magicka_curse);
    curse_list.push_back(silence_curse);
    curse_list.push_back(melee_damage_curse);
    curse_list.push_back(bow_damage_curse);
    curse_list.push_back(jump_curse);
}

void Forms::FormLoader::AddFormListEntriesToCurseVector()
{
    
    for (auto& form : curses_formlist->forms)
    {
        if (auto spell = form->As<RE::SpellItem>(); spell)
        {
            curse_list.push_back(spell);
            REX::DEBUG("Added {} to spell allow vector", spell->GetName());
        }
	}
}

void Forms::FormLoader::LoadForms()
{
    auto dh = RE::TESDataHandler::GetSingleton();
    if (Config::Settings::enable_sneak_stamina.GetValue())
    {
        if (auto main_file = dh->LookupModByName(Forms::FormConstants::mod_name);
            main_file && main_file->compileIndex != 0xFF)
        {
            sneak_stamina_spell = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::sneak_stamina_spell_ID,
                                                                Forms::FormConstants::mod_name);
            REX::DEBUG("sneak spell is: {}", sneak_stamina_spell->GetName());
            tall_grass_perk =
                dh->LookupForm<RE::BGSPerk>(Forms::FormConstants::tall_grass_perk_ID, Forms::FormConstants::mod_name);
            REX::DEBUG("Tall Grass Perk is: {}", tall_grass_perk->GetName());
			exhaustion_spell = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::exhaustion_spell_ID, Forms::FormConstants::mod_name);
			REX::DEBUG("Exhaustion Spell is: {}", exhaustion_spell->GetName());
        }
        else
        {

            std::string mod_name_base = Forms::FormConstants::mod_name;
            REX::FAIL("{} not found, please enable it.", mod_name_base);
        }
    }

    if (Config::Settings::enable_diseases.GetValue())
    {
        if (auto file = dh->LookupModByName(Forms::FormConstants::diseases_name); file && file->compileIndex != 0xFF)
        {
            health_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::health_curse_ID,
                                                         Forms::FormConstants::diseases_name);
            stamina_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::stamina_curse_ID,
                                                          Forms::FormConstants::diseases_name);
            magicka_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::magicka_curse_ID,
                                                          Forms::FormConstants::diseases_name);
            silence_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::silence_curse_ID,
                                                          Forms::FormConstants::diseases_name);
            melee_damage_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::melee_weakness_curse_ID,
                                                               Forms::FormConstants::diseases_name);
            bow_damage_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::bow_weakness_curse_ID,
                                                             Forms::FormConstants::diseases_name);
            jump_curse =
                dh->LookupForm<RE::SpellItem>(Forms::FormConstants::jump_curse_ID, Forms::FormConstants::diseases_name);
            spell_allow_list = dh->LookupForm<RE::BGSListForm>(Forms::FormConstants::spell_allow_list_ID,
                                                               Forms::FormConstants::diseases_name);

			curses_formlist = dh->LookupForm<RE::BGSListForm>(Forms::FormConstants::curses_formlist_ID, Forms::FormConstants::diseases_name);


            PopulateCurseList();
			AddFormListEntriesToCurseVector();

            disease_mod_active = true;
        }
        else
        {
            std::string mod_name = Forms::FormConstants::diseases_name;
            REX::FAIL(std::format(
                "{} not found, please either enable the esp or disable the diseases in the toml file of this mod.",
                mod_name));
        }
    }
}
