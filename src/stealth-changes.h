//
// Created by styyx on 22/02/2026.
//

#pragma once

namespace stweaks
{
    struct EnvironmentStealth
    {
        static bool ShouldAllowTallGrassPerk(RE::Actor* a_actor);
        static bool IsStandingInTallGrass(const RE::Actor* a_actor);
        static bool ShouldHaveTallGrassPerk(RE::Actor* a_actor);
    };
}