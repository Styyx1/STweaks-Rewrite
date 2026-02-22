//
// Created by styyx on 11/02/2026.
//

#include "spells.h"
#include "settings.h"

namespace stweaks
{
    bool CastSpeedFunctions::ShouldAllowCastSpeedChange(RE::ActorMagicCaster* a_caster)
    {
        if (!a_caster)
            return false;
        if (!Config::Settings::enable_skill_based_cast_speed.GetValue())
            return false;
        if (const auto state = a_caster->state.any(RE::ActorMagicCaster::State::kUnk01) ||  a_caster->state.any(RE::ActorMagicCaster::State::kUnk02); !state)
            return false;

        return true;
    }
}

