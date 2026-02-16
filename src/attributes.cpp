//
// Created by styyx on 15/02/2026.
//

#include "attributes.h"
#include "mod-storage.h"
#include "cache.h"
#include "settings.h"

namespace stweaks
{
    float Attributes::GetCarryPercentage(RE::PlayerCharacter* a_player)
    {
        return a_player->GetActorValue(RE::ActorValue::kInventoryWeight) /
           a_player->GetActorValue(RE::ActorValue::kCarryWeight);
    }

    void Attributes::UpdateAttributes(RE::PlayerCharacter* player)
    {
        if (av_timer.ElapsedSeconds() >= 45)
        {
            av_timer.Reset();
            stweaks::Attributes::ManageAttributeGrowthPlayer(player);
        }

        if (!av_timer.IsRunning() && !MiscUtil::IsAnyOfMenuOpen(Cache::GetUISingleton(), a_menuNames))
        {
            av_timer.Start();
        }
        if (av_timer.IsRunning() && MiscUtil::IsAnyOfMenuOpen(Cache::GetUISingleton(), a_menuNames))
        {
            av_timer.Stop();
        }

        if (!Config::Settings::enable_automatic_attributes.GetValue())
            av_timer.Stop();
    }

    void Attributes::ManageAttributeGrowthPlayer(RE::PlayerCharacter* a_this)
    {
        const float perc_health = ActorUtil::GetActorValuePercentage(a_this, RE::ActorValue::kHealth);
        const float perc_stamina = ActorUtil::GetActorValuePercentage(a_this, RE::ActorValue::kStamina);
        const float perc_magicka = ActorUtil::GetActorValuePercentage(a_this, RE::ActorValue::kMagicka);
        const float perc_carry = GetCarryPercentage(a_this);

        auto* av_store = AVStorage::GetSingleton();
        if (perc_health > 0.4 && perc_health < 0.95)
            av_store->attribute_xp[RE::ActorValue::kHealth] += 1;
        if (perc_stamina > 0.4 && perc_stamina < 0.95)
            av_store->attribute_xp[RE::ActorValue::kStamina] += 1;
        if (perc_magicka > 0.4 && perc_magicka < 0.95)
            av_store->attribute_xp[RE::ActorValue::kMagicka] += 1;
        if (perc_carry > 0.4 && perc_carry < 0.95)
            av_store->attribute_xp[RE::ActorValue::kCarryWeight] += 1;

        auto it = av_store->attribute_xp.begin();
        for (it; it != av_store->attribute_xp.end(); ++it)
        {
            if (it->second >= 100)
            {
                a_this->SetBaseActorValue(it->first, a_this->GetBaseActorValue(it->first) + 1);
                it->second -= 100;
            }
        }
    }
}
