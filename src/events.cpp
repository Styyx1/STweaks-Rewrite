#include "Events.h"
#include "mod-storage.h"

namespace Events
{
    EventResult HitEventHandler::ProcessEvent(const RE::TESHitEvent* a_event, RE::BSTEventSource<RE::TESHitEvent>*)
    {
        using HitFlag = RE::TESHitEvent::Flag;
        if (!a_event || !a_event->target || !a_event->cause)
        {
            return EventResult::kContinue;
        }
        const auto defender = a_event->target ? a_event->target->As<RE::Actor>() : nullptr;
        if (!defender)
        {
            return EventResult::kContinue;
        }
        if (const auto aggressor = a_event->cause ? a_event->cause->As<RE::Actor>() : nullptr; !aggressor)
        {
            return EventResult::kContinue;
        }
        const auto spell_item = RE::TESForm::LookupByID<RE::SpellItem>(a_event->source);

        if (!a_event->flags.any(HitFlag::kHitBlocked)) {
            if (defender->IsCasting(nullptr) && Config::Settings::interupt_cast_on_hit.GetValue()) {
                if(!spell_item || spell_item->GetCastingType() != RE::MagicSystem::CastingType::kConcentration)
                defender->InterruptCast(true);
            }
        }

        if (!a_event->flags.any(HitFlag::kBashAttack))
        {
            if (defender->IsPlayerRef()) {
                if (const auto spellItem = RE::TESForm::LookupByID<RE::SpellItem>(a_event->source); spellItem && Forms::FormLoader::disease_mod_active) {
                    REX::DEBUG("Spell item is {}", spellItem->GetName());
                    auto chance = Config::Settings::curse_chance.GetValue();
                    if (spellItem->GetCastingType() == RE::MagicSystem::CastingType::kConcentration) {
                        chance *= 0.10f;
                    }
                    if (Utility::Curses::ShouldApplyCurse(chance)) {
                        Utility::Curses::ApplyRandomCurse(defender, Forms::FormLoader::curse_list);
                    }
                }
            }

            if (auto& tick = stweaks::ModStorage::GetSingleton()->disease_timers[defender]; !tick.IsRunning() || tick.ElapsedSeconds() > 3.0f)
            {
				ManageCurse(defender);
                tick.Reset();
                tick.Start();
            }
            else
            {
                REX::DEBUG("{} tried to apply disease too soon. {:.1f}s remaining", defender->GetName(), 3.0f - tick.ElapsedSeconds());
                return EventResult::kContinue;
            }
        }
        return EventResult::kContinue;
    }

    float HitEventHandler::CalculatePenaltyAndStoreIt(RE::Actor* a_actor, RE::ActorValue a_av, float& penalty_storage)
    {
        float maxPenAv = GetMaxActorValue(a_actor, a_av) + penalty_storage;
        float newPenaltyMag = std::roundf(maxPenAv * 0.01f);
        float maxAllowedPenalty = std::roundf(maxPenAv * 0.9f);
        REX::DEBUG("penalty_storage is {}", penalty_storage);
        REX::DEBUG("maxPenAv is {}", maxPenAv);
        REX::DEBUG("newPenaltyMag is {}", newPenaltyMag);

        if (penalty_storage + newPenaltyMag > maxAllowedPenalty) {
            newPenaltyMag = maxAllowedPenalty - penalty_storage;
            newPenaltyMag = std::max(0.0f, newPenaltyMag);
        }

        penalty_storage += newPenaltyMag;
        return newPenaltyMag;
    }

    void HitEventHandler::RegisterHitEvent()
    {
        RE::ScriptEventSourceHolder* eventHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventHolder->AddEventSink(HitEventHandler::GetSingleton());
        REX::INFO("Registered for {}", typeid(RE::TESHitEvent).name());
    }
    float HitEventHandler::GetMaxActorValue(RE::Actor* a_actor, RE::ActorValue a_av)
    {
        return a_actor->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kTemporary, a_av) + a_actor->GetPermanentActorValue(a_av);
    }
    void HitEventHandler::ManageCurse(RE::Actor* defender)
    {
        auto store = stweaks::ModStorage::GetSingleton();
        if (Utility::ActiveEffectHasNewDiseaseKeyword(defender, stweaks::keywords::disease_keywords[0].data()))
        {
            // storedHealth_disease = std::min(std::max(storedHealth_disease + 1.0f, 0.0f), 99.0f);
            float decrease_value = CalculatePenaltyAndStoreIt(defender, RE::ActorValue::kHealth, store->storedHealth_disease);
            REX::DEBUG("stored health disease is {}", store->storedHealth_disease);
            defender->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kHealth, -decrease_value);
        }
        if (Utility::ActiveEffectHasNewDiseaseKeyword(defender, stweaks::keywords::disease_keywords[1].data()))
        {
            // storedStamina_disease = std::min(std::max(storedStamina_disease + 1.0f, 0.0f), 99.0f);
            float decrease_value_stam = CalculatePenaltyAndStoreIt(defender, RE::ActorValue::kStamina, store->storedStamina_disease);
            REX::DEBUG("stored stamina disease is {}", store->storedStamina_disease);
            defender->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kStamina, -decrease_value_stam);
        }
        if (Utility::ActiveEffectHasNewDiseaseKeyword(defender, stweaks::keywords::disease_keywords[2].data()))
        {

            // storedMagicka_disease = std::min(std::max(storedMagicka_disease + 1.0f, 0.0f), 99.0f);
            float decrease_value_mag = CalculatePenaltyAndStoreIt(defender, RE::ActorValue::kMagicka, store->storedMagicka_disease);
            REX::DEBUG("stored magicka disease is {}", store->storedMagicka_disease);
            defender->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMagicka, -decrease_value_mag);
        }
    }
    void RegisterEvents()
    {
        if (Config::Settings::enable_curses.GetValue())
        {
            auto hitEventHandler = HitEventHandler::GetSingleton();
            hitEventHandler->RegisterHitEvent();
            auto effectHandler = ApplyEffectEvent::GetSingleton();
            effectHandler->RegisterApplyEffect();
            EquipManager::RegisterEquipEvent();
        }
    }
    RE::BSEventNotifyControl ApplyEffectEvent::ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource)
    {
        if (!a_event)
            return EventResult::kContinue;
        if (!a_event->magicEffect)
            return EventResult::kContinue;
        auto effect = RE::TESForm::LookupByID<RE::EffectSetting>(a_event->magicEffect);
        if (!effect)
            return EventResult::kContinue;
        if (!effect->HasKeywordByEditorID(stweaks::keywords::kCure))
            return EventResult::kContinue;
        auto target = a_event->target.get();
        auto targetActor = target ? target->As<RE::Actor>() : nullptr;
        if (!targetActor)
            return EventResult::kContinue;
        Utility::Curses::CleanseCurse(targetActor);
        REX::DEBUG("Effect '{}' triggered. Target is '{}'.", effect->GetName(), targetActor->GetName());

        return EventResult::kContinue;
    }
    void ApplyEffectEvent::RegisterApplyEffect()
    {
        auto* eventSink = ApplyEffectEvent::GetSingleton();
        auto* eventSourceHolder = RE::ScriptEventSourceHolder::GetSingleton();
        eventSourceHolder->AddEventSink<RE::TESMagicEffectApplyEvent>(eventSink);

        REX::INFO("registered Apply Effect Event");
    }

    
    void EquipManager::RegisterEquipEvent()
    {
        if (const auto src = RE::ScriptEventSourceHolder::GetSingleton(); src) {
            src->AddEventSink<RE::TESEquipEvent>(GetSingleton());
            REX::INFO("Registered for EquipEvent");
        }
    }

    RE::BSEventNotifyControl EquipManager::ProcessEvent(const RE::TESEquipEvent* event, RE::BSTEventSource<RE::TESEquipEvent>*)
    {
        if(!event || !event->actor)
            return RE::BSEventNotifyControl::kContinue;

        const auto act = event->actor.get() ? event->actor.get()->As<RE::Actor>() : nullptr;
        if(!act)
            return RE::BSEventNotifyControl::kContinue;

        Utility::AdjustMass(act);
        REX::INFO("equip event fired for: {}, item was: {}", act->GetName(), event->equipped ? "equipped" : "unequipped");
        
        return RE::BSEventNotifyControl::kContinue;
    }

}