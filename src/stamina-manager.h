//
// Created by styyx on 11/02/2026.
//

#pragma once

namespace stweaks
{
    struct StealthStamina
    {
        static void ManageSneakStamina(RE::PlayerCharacter* a_player, float a_deltaTime);
        static bool ManageJumpStamina(RE::Actor* a_actor);
        static float CalculateJumpCost(RE::Actor* a_actor);
    private:
        static bool HasRangedWeaponOut(RE::Actor* a_actor);
        static float GetJumpMassModifier(const RE::Actor* a_actor);
        static float GetJumpBaseCost();


    };
}