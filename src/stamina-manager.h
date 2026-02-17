//
// Created by styyx on 11/02/2026.
//

#pragma once

namespace stweaks
{
    struct StaminaCost
    {
        static void ManageSneakStamina(RE::PlayerCharacter* a_player, float a_deltaTime);
        static void ManageJumpStamina(RE::Actor* a_actor);
        static float CalculateJumpCost(RE::Actor* a_actor);
        static bool ManageCastStamina(RE::ActorMagicCaster* a_caster, RE::MagicItem* a_spell, RE::MagicSystem::CannotCastReason *a_reason);
        static float CalculateWeaponWeightCost(RE::Actor *actor, const RE::ActorValue av_to_use, RE::TESForm* weapon_or_shield);
        static float ManageAttackStaminaCost(RE::Actor* a_actor, const RE::BGSAttackData* a_attack);
        static void ApplyExhaustion(RE::Actor* a_actor, float a_cost);

    private:
        static bool HasRangedWeaponOut(RE::Actor* a_actor);
        static float GetJumpMassModifier(const RE::Actor* a_actor);
        static float GetJumpBaseCost();
        static float CalculateCastStaminaCost(RE::ActorMagicCaster* a_caster, RE::MagicItem* a_spell);
        static RE::ActorValue GetActorValueToUseForAttackCostBash(const RE::Actor* a_actor);
        static RE::ActorValue GetActorValueToUseForAttackCostAttack(RE::Actor* a_actor);



    };
}