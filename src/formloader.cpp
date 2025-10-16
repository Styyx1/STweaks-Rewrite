#include "formloader.h"

void Forms::FormLoader::LoadForms()
{
    auto dh = RE::TESDataHandler::GetSingleton();
    if (Config::Settings::enable_sneak_stamina.GetValue()) {
        if (auto main_file = dh->LookupModByName(Forms::FormConstants::mod_name); main_file && main_file->compileIndex != 0xFF) {
            sneak_stamina_spell = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::sneak_stamina_spell_ID, Forms::FormConstants::mod_name);
            REX::INFO("sneak spell is: {}", sneak_stamina_spell->GetName());
        }
        else
            REX::FAIL(std::format("{} not found, please enable it.", Forms::FormConstants::mod_name));
    }


    if (Config::Settings::enable_diseases.GetValue()) {
        if (auto file = dh->LookupModByName(Forms::FormConstants::diseases_name); file && file->compileIndex != 0xFF) {
            health_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::health_curse_ID, Forms::FormConstants::diseases_name);
            stamina_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::stamina_curse_ID, Forms::FormConstants::diseases_name);
            magicka_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::magicka_curse_ID, Forms::FormConstants::diseases_name);
            silence_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::silence_curse_ID, Forms::FormConstants::diseases_name);
            melee_damage_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::melee_weakness_curse_ID, Forms::FormConstants::diseases_name);
            bow_damage_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::bow_weakness_curse_ID, Forms::FormConstants::diseases_name);
            jump_curse = dh->LookupForm<RE::SpellItem>(Forms::FormConstants::jump_curse_ID, Forms::FormConstants::diseases_name);

            curse_list = {
                health_curse, stamina_curse, magicka_curse, silence_curse,melee_damage_curse, bow_damage_curse, jump_curse
            };

            disease_mod_active = true;
        }
        else {
            REX::FAIL(std::format("{} not found, please either enable the esp or disable the diseases in the toml file of this mod.", Forms::FormConstants::diseases_name));
        }
    }
}
