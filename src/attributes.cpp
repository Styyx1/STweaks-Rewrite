//
// Created by styyx on 15/02/2026.
//

#include "attributes.h"
#include "mod-storage.h"
#include "cache.h"
#include "settings.h"
#include "stamina-manager.h"
#include "Utility.h"

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

    float JumpHeight::GetMassModifier(const RE::Actor* a_actor)
    {
        float modi = 1.0f;

        if (!a_actor)
            return modi;

        if (Config::Settings::enable_mass_based_jump_height.GetValue())
        {
            constexpr float MIN_MASS = 0.01f;
            const float mass = std::max(a_actor->GetActorValue(RE::ActorValue::kMass),MIN_MASS);
            modi = std::sqrt(1.2f/mass);
            modi = std::max(modi, 0.6f);
            REX::DEBUG("mass modifier is: {}", modi);
        }
        return modi;
    }

    float JumpHeight::GetCurseModifier(RE::Actor* a_actor)
    {
        float curse_modi = 1.0f;
        if (!a_actor)
            return curse_modi;

        if (Config::Settings::enable_curses.GetValue())
        {
            if (Utility::ActiveEffectHasNewDiseaseKeyword(a_actor, stweaks::keywords::kJump.data()))
            {
                curse_modi = 0.5f;
            }
        }
        return curse_modi;
    }

    float JumpHeight::GetSneakModifier(const RE::Actor* a_actor)
    {
        float modi = 1.0f;
        if (!a_actor)
            return modi;

        if (a_actor->IsSneaking() && Config::Settings::enable_sneak_jump_limit.GetValue())
        {
            modi = Config::Settings::sneak_height_modifier.GetValue();
        }

        return modi;
    }

    float JumpHeight::GetTotalModifier(RE::Actor* a_actor)
    {
        if (!a_actor)
            return 1.0f;

        if (Utility::IsGod(a_actor))
        {
            return 1.0f;
        }
        const float modi = GetSneakModifier(a_actor) * GetCurseModifier(a_actor) * GetMassModifier(a_actor);
        REX::DEBUG("modifier is: {}", modi);
        return modi;
    }

    bool JumpHeight::HasEnoughStamina(const RE::AIProcess* a_proc)
    {
        const auto player = Cache::GetPlayerSingleton();
        if (!player)
        {
            return false;
        }
        if (a_proc && a_proc->middleHigh)
        {
            if (player->GetActorValue(RE::ActorValue::kStamina) < stweaks::StaminaCost::CalculateJumpCost(player))
            {
                RE::FlashHUDMeter(RE::ActorValue::kStamina);
                return false;
            }
        }
        return true;
    }
}
