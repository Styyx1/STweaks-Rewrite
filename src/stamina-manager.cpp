//
// Created by styyx on 11/02/2026.
//

#include "stamina-manager.h"
#include "settings.h"
#include "hooks.h"
#include "API/PerkEntryPointExtenderAPI.h"
#include "mod-data.h"

namespace stweaks
{
    bool StealthStamina::HasRangedWeaponOut(RE::Actor* a_actor)
    {
        bool result = false;
        if (const RE::TESObjectWEAP *weap = Utility::getWieldingWeapon(a_actor); weap)
        {
            if (weap->IsBow() && a_actor->IsWeaponDrawn() || weap->IsCrossbow() && a_actor->IsWeaponDrawn())
                result = true;
        }

        REX::DEBUG("check for ranged weapon, it is {}", result ? "true" : "false");
        return result;
    }

    void StealthStamina::ManageSneakStamina(RE::PlayerCharacter* a_player, float a_deltaTime )
    {
        if (!a_player)
        {
            return;
        }

        if (RE::PlayerCharacter::IsGodMode())
        {
            return;
        }

        bool isSneakSetting = Config::Settings::enable_sneak_stamina.GetValue();
        bool shouldDrain = a_player->IsSneaking() && (Utility::IsMoving(a_player) || HasRangedWeaponOut(a_player));

        if ( isSneakSetting && shouldDrain)
        {
            if (const float stam = a_player->GetActorValue(RE::ActorValue::kStamina); stam > 0.0f)
            {
                float stam_reduce = 5.0f * a_deltaTime;
                RE::TESObjectARMO* armor = ActorUtil::GetEquippedChestPiece(a_player);
                RE::HandleEntryPoint(pepe::SNEAK_STAMINA::perkEntry, a_player, stam_reduce, pepe::SNEAK_STAMINA::perkCategory, armor);

                a_player->DamageActorValue(RE::ActorValue::kStamina, -stam_reduce);
                if (a_player->GetActorValue(RE::ActorValue::kStamina) <= 3 && HasRangedWeaponOut(a_player))
                {
                    a_player->actorState2.weaponState = RE::WEAPON_STATE::kWantToSheathe;
                    a_player->DrawWeaponMagicHands(false);
                }
            }
        }

    }

    float StealthStamina::GetJumpMassModifier(const RE::Actor* a_actor)
    {
        float mass = a_actor->GetActorValue(RE::ActorValue::kMass);
        REX::INFO("mass: {}", mass);
        mass = std::clamp(mass, 0.1f, MAX_JUMP_MASS_CALCULATOR);
        const float mass_ratio = mass / BASE_MASS_AVERAGE;
        return std::pow(mass_ratio, 1.2f);
    }

    float StealthStamina::GetJumpBaseCost()
    {
        return 10.0f;
    }

    float StealthStamina::CalculateJumpCost(RE::Actor* a_actor)
    {
        if (!Config::Settings::jump_stamina_cost.GetValue())
            return 0.0f;

        const float base_cost = GetJumpBaseCost();
        const float mass_modifier = GetJumpMassModifier(a_actor);
        float cost = base_cost * mass_modifier;
        REX::INFO("cost: {}, base: {}, mass modifier: {}", cost, base_cost, mass_modifier);
        RE::TESObjectARMO* armor = ActorUtil::GetEquippedChestPiece(a_actor);
        RE::HandleEntryPoint(pepe::JUMP_STAMINA::perkEntry, a_actor, cost, pepe::JUMP_STAMINA::perkCategory, armor);
        return cost;
    }

    bool StealthStamina::ManageJumpStamina(RE::Actor* a_actor)
    {
        if (!Config::Settings::jump_stamina_cost.GetValue())
        {
            return true;
        }
        const float jump_cost = CalculateJumpCost(a_actor);
        REX::INFO("jump cost {}", jump_cost);

        if (const float stamina = a_actor->GetActorValue(RE::ActorValue::kStamina); stamina < jump_cost)
        {
            return false;
        }
        a_actor->DamageActorValue(RE::ActorValue::kStamina, -jump_cost);
        return true;
    }
}
