//
// Created by styyx on 20/02/2026.
//

#pragma once

namespace stweaks
{
    struct NPCChanges
    {
        static void LevelUpNpcs(RE::Actor* a_this, float a_playerLevel);
        static void ChangeNPCResistances(RE::Actor* a_this, float a_playerLevel);
    };
}