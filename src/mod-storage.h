//
// Created by styyx on 12/02/2026.
//

#pragma once

namespace stweaks
{
    struct ModStorage : REX::TSingleton<ModStorage>
    {
        float storedHealth_disease = 0;
        float storedStamina_disease = 0;
        float storedMagicka_disease = 0;
        static inline std::unordered_map<RE::Actor*, TimerUtil::Timer> disease_timers;
    };

    struct AVStorage : public REX::TSingleton<AVStorage>
    {
        std::unordered_map<RE::ActorValue, uint16_t> attribute_xp;
        inline uint16_t get_attribute_xp(RE::ActorValue av)
        {
            if (attribute_xp.empty())
                return 0;
            return attribute_xp[av];
        }
    };

}
