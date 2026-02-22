//
// Created by styyx on 11/02/2026.
//

#pragma once

namespace stweaks
{
    struct OppModi {
        enum class OpportunityType : int {
            Sleep = 0,
            Attack = 1,
            Paralysis = 2,
            Backstab = 3,
            None = 4
          };

        inline static std::array<float, 5> OpportunityModifiers = {
            4.0f, // Sleep
            1.2f, // Attack
            2.5f, // Paralysis
            1.3f, // Backstab
            1.0f  // None
        };

        inline static float GetModifier(OpportunityType type) {
            const auto opp = static_cast<int>(type);
            if (opp > 4)
                return 1.0;
            return OpportunityModifiers[opp];
        }
    };

    struct DamageCalculations
    {
        static bool IsSpellValidForCalcs(RE::SpellItem* a_spell);
        static void ApplyDamageRanges(float& a_damage, float a_lowerRange, float a_upperRange);
        static void ApplyDamageCap(RE::Actor* a_attacker, RE::Actor* a_target, float original_damage, float &a_damageOut, bool a_powerAttack = false);
        static bool IsValidTargetAndAttacker(const RE::Actor* a_attacker, const RE::Actor* a_target);
        static float GetCurseModifiedDamage(RE::Actor* a_attacker, float damage_in);
        static float GetEtherealModifiedDamage(RE::Actor* a_attacker, float damage_in);
        static float GetFollowerModifiedDamage(const RE::Actor* a_attacker, float damage_in);
        static float GetDamageRangeModifiedDamage(float damage_in, const float a_lowerRange, const float a_upperRange);
        static float GetQuestItemModifiedDamage(const RE::Actor* a_attacker, RE::TESObjectWEAP* a_weapon, float damage_in);
        static float GetOpportunityModifiedDamage(RE::Actor* a_attacker, RE::Actor* a_target, float damage_in);
        static float GetOpportunityModifier(RE::Actor *victim, RE::Actor *attacker, bool notification);
        static bool ShouldProtectOneHit(const RE::Actor* a_attacker, RE::Actor* a_target, bool a_isSneakAttack, bool a_usesBow);
        static float GetCappedDamagePhysical(float damage_in,  float damage_cap, bool a_powerAttack = false, bool a_sneakAttack = false);
        static float GetOneShotProtectedDamagePhysical(const RE::Actor* a_attacker, RE::Actor* a_target, float damage_in, bool a_sneakAttack, bool a_usesBow);
    };

}