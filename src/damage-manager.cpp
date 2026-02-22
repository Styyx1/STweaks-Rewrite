//
// Created by styyx on 11/02/2026.
//

#include "damage-manager.h"
#include "settings.h"
#include "Utility.h"

#undef PlaySound

namespace stweaks
{
    bool DamageCalculations::IsSpellValidForCalcs(RE::SpellItem* a_spell)
    {
        if (!a_spell)
            return false;

        if (!MagicUtil::IsCastableSpell(a_spell))
            return false;

        if (!a_spell->IsHostile())
            return false;

        if (const auto av_effect = a_spell->GetAVEffect(); !av_effect || !av_effect->IsDetrimental())
            return false;

        return true;
    }

    void DamageCalculations::ApplyDamageRanges(float& a_damage, float a_lowerRange, float a_upperRange)
    {
        if (!Config::Settings::enable_damage_ranges.GetValue())
            return;
        const float lowerRange = Utility::CalcPerc(a_lowerRange, false);
        const float upperRange = Utility::CalcPerc(a_upperRange, true);
        const float damage_mult = RandomiserUtil::GetRandomFloat(lowerRange, upperRange);

        a_damage *= damage_mult;
    }

    void DamageCalculations::ApplyDamageCap(RE::Actor* a_attacker, RE::Actor* a_target, float original_damage,
        float& a_damageOut, const bool a_powerAttack)
    {
        if (!a_attacker || !a_target || a_target->IsDead())
            return;
        if (a_attacker->IsSneaking() && a_target->RequestDetectionLevel(a_attacker) <= 0)
            return;
        const float max_health = ActorUtil::GetMaxHealth(a_target);
        const float current_health = a_target->GetActorValue(RE::ActorValue::kHealth);

        if (a_attacker->GetLevel() > a_target->GetLevel() + 10)
            return;

        if (Config::Settings::one_shot_protection.GetValue())
        {
            if (a_damageOut > max_health && current_health >= max_health * 0.99f)
            {
                a_damageOut = max_health * Utility::GetPercentageDecrease(Config::Settings::max_damage_one_hit.GetValue());
            }
        }

        if (Config::Settings::enable_damage_caps.GetValue())
        {
            if (const float damage_cap = original_damage * 5.f; a_damageOut > damage_cap)
            {
                const float modifier = a_powerAttack ? 2.f : 1.f;
                a_damageOut = damage_cap * modifier;
            }
        }
    }

    bool DamageCalculations::IsValidTargetAndAttacker(const RE::Actor* a_attacker, const RE::Actor* a_target)
    {
        if (!a_attacker || !a_target || a_target->IsDead())
            return false;
        return true;
    }

    float DamageCalculations::GetCurseModifiedDamage(RE::Actor* a_attacker, float damage_in)
    {
        if (!a_attacker)
            return damage_in;

        if (Config::Settings::enable_curses.GetValue())
        {
            //manages the damage reduction from the curse. 60% chance to deal 50% damage and 10% chance to deal no damage at all
            if (Utility::ActiveEffectHasNewDiseaseKeyword(a_attacker, keywords::kAttack.data()))
            {
                if (const float chance_for_effect = Utility::GetRandomFloat(0.0, 100.0); chance_for_effect < 10.0f)
                {
                    damage_in *= 0.0;
                    REX::DEBUG(" 10% weakness curse is active, you deal {} damage", damage_in);
                }
                else if (chance_for_effect < 60.0f)
                {
                    damage_in *= 0.5f;
                    REX::DEBUG("50% weakness curse is active, you deal {} damage", damage_in);
                }
            }
        }
        return damage_in;
    }

    float DamageCalculations::GetEtherealModifiedDamage(RE::Actor* a_attacker, float damage_in)
    {
        if (!a_attacker)
            return damage_in;

        if (Utility::ActorHasEffectWithArchetype(a_attacker, RE::EffectArchetypes::ArchetypeID::kEtherealize) &&
            Config::Settings::enable_etheral_change.GetValue())
        {
            damage_in = 0.0f;
        }
        return damage_in;
    }

    float DamageCalculations::GetFollowerModifiedDamage(const RE::Actor* a_attacker, float damage_in)
    {
        const auto player = Cache::GetPlayerSingleton();

        if (!player || !a_attacker)
            return damage_in;

        const bool is_team = a_attacker->IsPlayerTeammate() && !a_attacker->IsCommandedActor();
        if (const bool is_player = a_attacker == player; is_team || is_player )
        {
            if (player->teammateCount > 0 && Config::Settings::enable_foll_change.GetValue())
            {
                constexpr float adj_mod1 = 5.0f;
                constexpr float adj_divider = 20.0f;
                const uint32_t foll_count = player->teammateCount;
                const float speech_level = player->GetActorValue(RE::ActorValue::kSpeech);

                const float modifier = std::clamp(1.0f - (foll_count) / (adj_mod1 + speech_level / adj_divider), 0.10f, 0.90f);

                damage_in *= modifier;
             }
        }
        return damage_in;
    }

    float DamageCalculations::GetDamageRangeModifiedDamage(float damage_in, const float a_lowerRange, const float a_upperRange)
    {
        if (!Config::Settings::enable_damage_ranges.GetValue())
            return damage_in;
        const float lowerRange = Utility::CalcPerc(a_lowerRange, false);
        const float upperRange = Utility::CalcPerc(a_upperRange, true);
        const float damage_mult = RandomiserUtil::GetRandomFloat(lowerRange, upperRange);
        damage_in *= damage_mult;

        return damage_in;
    }

    float DamageCalculations::GetQuestItemModifiedDamage(const RE::Actor* a_attacker, RE::TESObjectWEAP* a_weapon, float damage_in)
    {
        if (!a_attacker)
            return damage_in;

        if (!Config::Settings::enable_quest_item_nerf.GetValue())
            return damage_in;

        if (!ActorUtil::ActorHasQuestObjectInHand(a_attacker))
            return damage_in;

        if (!a_weapon)
            return damage_in;

        if (Config::Exceptions::IsQuestWeaponException(a_weapon))
            return damage_in;

        damage_in *= 0.005f;

        return damage_in;
    }

    float DamageCalculations::GetOpportunityModifiedDamage(RE::Actor* a_attacker, RE::Actor* a_target,
        float damage_in)
    {
        if (!a_attacker || !a_target)
            return damage_in;

        if (!Config::Settings::attacks_of_opp.GetValue())
            return damage_in;

        const float opp_mod = GetOpportunityModifier(a_target, a_attacker, false);
        damage_in *= opp_mod;
        if (opp_mod > 1.0f && a_target->IsPlayerRef())
        {
            RE::PlaySound("UISneakAttack");
            const RE::CriticalHit::Event event{a_attacker, ActorUtil::getWieldingWeapon(a_attacker), false};
            RE::CriticalHit::GetEventSource()->SendEvent(&event);
        }
        return damage_in;
    }

    float DamageCalculations::GetOpportunityModifier(RE::Actor* victim, RE::Actor* attacker, const bool notification)
    {
        float mod = 1.0f;

        if (!victim || !attacker) {
            return mod;
        }

        if (!ActorUtil::IsInOpportunityState(victim, attacker))
            return mod;
        static std::string message{};
        if (ActorUtil::ActorHasEffectOfTypeActive(victim, RE::EffectArchetypes::ArchetypeID::kParalysis) ||
            ActorUtil::ActorHasEffectOfTypeActive(victim, RE::EffectArchetypes::ArchetypeID::kCalm))
        {
            mod = OppModi::GetModifier(OppModi::OpportunityType::Paralysis);
            if (notification) {
                message = std::format("attack of opportunity occured for {} times damage", mod);
                RE::SendHUDMessage::ShowHUDMessage(message.c_str());
            }

        }
        else if (ActorUtil::IsPowerAttacking(victim) || victim->IsStaggering())
        {
            mod = OppModi::GetModifier(OppModi::OpportunityType::Attack);
            if (notification) {
                message = std::format("attack of opportunity occured for {} times damage", mod);
                RE::SendHUDMessage::ShowHUDMessage(message.c_str());
            }

        }
        else if (victim->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kIsSitting ||
                 victim->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kIsSleeping)
        {
            mod = OppModi::GetModifier(OppModi::OpportunityType::Sleep);
            if (notification) {
                message = std::format("attack of opportunity occured for {} times damage", mod);
                RE::SendHUDMessage::ShowHUDMessage(message.c_str());
            }

        }
        else if (victim->GetHeadingAngle(attacker->GetPosition(), false) <= -135 ||
                 victim->GetHeadingAngle(attacker->GetPosition(), false) >= 135)
        {
            mod = OppModi::GetModifier(OppModi::OpportunityType::Backstab);
            if (notification) {
                message = std::format("attack of opportunity occured for {} times damage", mod);
                RE::SendHUDMessage::ShowHUDMessage(message.c_str());
            }
        }
        return mod;
    }

    bool DamageCalculations::ShouldProtectOneHit(const RE::Actor* a_attacker, RE::Actor* a_target, bool a_isSneakAttack, bool a_usesBow)
    {
        if (!Config::Settings::one_shot_protection.GetValue())
            return false;

        if (!a_attacker || !a_target)
            return false;

        if (a_attacker->GetLevel() > a_target->GetLevel() + 10)
            return false;

        if (a_target->GetActorValue(RE::ActorValue::kHealth) < ActorUtil::GetMaxHealth(a_target) * 0.98f)
            return false;

        if (a_isSneakAttack && !a_usesBow)
            return false;

        return true;
    }

    float DamageCalculations::GetCappedDamagePhysical(float damage_in,  float damage_cap, bool a_powerAttack, bool a_sneakAttack)
    {
        if (!Config::Settings::enable_damage_caps.GetValue())
            return damage_in;

        if (a_sneakAttack)
            return damage_in;

        if (a_powerAttack)
            damage_cap *= 2;

        return std::min(damage_in, damage_cap);
    }

    float DamageCalculations::GetOneShotProtectedDamagePhysical(const RE::Actor* a_attacker, RE::Actor* a_target,
                                                                const float damage_in, const bool a_sneakAttack, const bool a_usesBow)
    {
        if (!a_attacker || !a_target)
            return damage_in;

        if (!ShouldProtectOneHit(a_attacker, a_target, a_sneakAttack, a_usesBow))
            return damage_in;

        const float max_health = ActorUtil::GetMaxHealth(a_target);
        if (damage_in < max_health)
            return damage_in;

        return  max_health * Utility::GetPercentageDecrease(Config::Settings::max_damage_one_hit.GetValue());
    }
}
