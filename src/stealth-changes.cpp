//
// Created by styyx on 22/02/2026.
//

#include "stealth-changes.h"

#include "Settings.h"

namespace stweaks
{
    bool EnvironmentStealth::ShouldAllowTallGrassPerk(RE::Actor* a_actor)
    {
        if (!Config::Settings::tall_grass_sneak.GetValue())
            return false;

        if (!a_actor)
            return false;

        if (!a_actor->IsPlayerRef())
            return false;

        if (!a_actor->IsSneaking())
            return false;

        return true;
    }

    bool EnvironmentStealth::IsStandingInTallGrass(const RE::Actor* a_actor)
    {
        if (!a_actor || !a_actor->IsPlayerRef())
            return false;

        const auto cell = a_actor->GetParentCell();
        if (!cell)
            return false;

        if (!cell->IsExteriorCell())
            return false;

        const auto tes = RE::TES::GetSingleton();
        if (!tes)
            return false;

        const auto landscapeTexture = tes->GetLandTexture(a_actor->GetPosition());
        if (!landscapeTexture)
            return false;

        if (Config::Exceptions::IsTallGrass(landscapeTexture))
        {
            return true;
        }
        return false;
    }

    bool EnvironmentStealth::ShouldHaveTallGrassPerk(RE::Actor* a_actor)
    {
        return ShouldAllowTallGrassPerk(a_actor) && IsStandingInTallGrass(a_actor);
    }
}
