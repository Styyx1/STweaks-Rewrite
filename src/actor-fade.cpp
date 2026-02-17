//
// Created by styyx on 16/02/2026.
//

#include "actor-fade.h"
#include "cache.h"
#include "mod-data.h"
#include "Settings.h"

namespace stweaks
{
    void ActorFade::ProcessActorFade(RE::Actor* a_this)
    {

        if (!a_this)
        {
            return;
        }
        if (!Config::Settings::enable_fading_actors.GetValue())
        {
            if (a_this->GetAlpha() < 1.0)
                a_this->SetAlpha();
            return;
        }

        RE::PlayerCharacter *player = Cache::GetPlayerSingleton();
        if (!player)
        {
            return;
        }

        constexpr float fade_range = FADE_DISTANCE_FULL_FADE - FADE_DISTANCE_NO_FADE;

        if (const float distance = a_this->GetDistance(player); distance >= FADE_DISTANCE_FULL_FADE && a_this->GetHighProcess() &&
                                                           a_this->GetHighProcess()->lightLevel <= FADE_LIGHT_LEVEL_THRESHOLD)
        {
            const float fade_progress = std::min(std::max((distance - FADE_DISTANCE_NO_FADE) / fade_range, 0.0f), 1.0f);
            const float alpha = std::lerp(1.0f, FADE_MIN_ALPHA, fade_progress);
            a_this->SetAlpha(alpha);
        }
        else
        {
            a_this->SetAlpha();
        }
    }
}
