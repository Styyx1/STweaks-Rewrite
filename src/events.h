#pragma once
#include "Settings.h"
#include "Utility.h"

using EventResult = RE::BSEventNotifyControl;

namespace Events
{
    void RegisterEvents();

    using EventResult = RE::BSEventNotifyControl;

    class HitEventHandler : public REX::Singleton<HitEventHandler>,
        public RE::BSTEventSink<RE::TESHitEvent>
    {
        EventResult ProcessEvent(const RE::TESHitEvent* a_event, [[maybe_unused]] RE::BSTEventSource<RE::TESHitEvent>* a_eventSource) override;
        float CalculatePenaltyAndStoreIt(RE::Actor* a_actor, RE::ActorValue a_av, float& penalty_storage);
        bool doOnce = false;

    public:
        void RegisterHitEvent();
        float GetMaxActorValue(RE::Actor* a_actor, RE::ActorValue a_av);
        float storedHealth_disease = 0;
        float storedStamina_disease = 0;
        float storedMagicka_disease = 0;
        static inline std::unordered_map<RE::Actor*, Utility::Timer> disease_timers;

        static void Character__invalidate_cached(RE::Actor* a, RE::ActorValue av)
        {
            using func_t = decltype(Character__invalidate_cached);
            REL::Relocation<func_t> func{ REL::ID(38483) };
            return func(a, av);
        }
    private: 
        void ManageCurse(RE::Actor* defender);

    };

    class ApplyEffectEvent final : public REX::Singleton<ApplyEffectEvent>, public RE::BSTEventSink<RE::TESMagicEffectApplyEvent>
    {
        RE::BSEventNotifyControl ProcessEvent(const RE::TESMagicEffectApplyEvent* a_event, RE::BSTEventSource<RE::TESMagicEffectApplyEvent>* a_eventSource) override;

    public:
        static void RegisterApplyEffect();
    };

}