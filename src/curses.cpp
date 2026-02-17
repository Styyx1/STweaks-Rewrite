//
// Created by styyx on 11/02/2026.
//

#include "curses.h"

#include "formloader.h"
#include "mod-storage.h"
#include "mod-data.h"
#include "Settings.h"
#include "Utility.h"

namespace stweaks
{
    void Curses::RemoveCurse(RE::ActiveEffect* a_curseEffect)
    {
        const auto store = stweaks::ModStorage::GetSingleton();
        const auto base_effect = a_curseEffect->GetBaseObject();
        if (RE::Actor *aff_actor = a_curseEffect->target ? a_curseEffect->target->GetTargetAsActor() : nullptr; aff_actor && base_effect)
        {
            if (base_effect->HasKeywordString(stweaks::keywords::disease_keywords[0]))
            {
                if (const float currentPenaltyH = store->storedHealth_disease; currentPenaltyH > 0)
                {
                    store->storedHealth_disease = 0.0f;
                    aff_actor->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kHealth,
                                             currentPenaltyH);
                }
            }
            if (base_effect->HasKeywordString(stweaks::keywords::disease_keywords[1]))
            {
                if (const float currentPenaltyS = store->storedStamina_disease; currentPenaltyS > 0)
                {
                    store->storedStamina_disease = 0.0f;
                    aff_actor->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kStamina,
                                             currentPenaltyS);
                }
            }
            if (base_effect->HasKeywordString(stweaks::keywords::disease_keywords[2]))
            {
                if (const float currentPenaltyM = store->storedMagicka_disease; currentPenaltyM > 0)
                {
                    store->storedMagicka_disease = 0.0f;
                    aff_actor->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMagicka,
                                             currentPenaltyM);
                }
            }
        }
    }

    bool Curses::BlockSpellWithCurse(RE::Actor* a_actor, const RE::MagicItem* a_spellItem)
    {
        if (!a_actor || !a_spellItem)
            return false; // don't block

        if (!Config::Settings::enable_curses.GetValue())
            return false;

        if (!Utility::ActiveEffectHasNewDiseaseKeyword(a_actor,keywords::kSilence.data()))
            return false;

        if (!Forms::FormLoader::spell_allow_list)
            return true; // silence active but no allow list = block everything

        if (Forms::FormLoader::spell_allow_list->forms.empty())
            return true;

        if (Forms::FormLoader::spell_allow_list->HasForm(a_spellItem))
            return false; // explicitly allowed

        return true; // block
    }
}

