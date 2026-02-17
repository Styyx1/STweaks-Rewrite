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
    bool StaminaCost::HasRangedWeaponOut(RE::Actor* a_actor)
    {
        bool result = false;
        if (const RE::TESObjectWEAP *weap = Utility::getWieldingWeapon(a_actor); weap)
        {
            if (weap->IsBow() && a_actor->IsWeaponDrawn() || weap->IsCrossbow() && a_actor->IsWeaponDrawn())
                result = true;
        }

        //REX::DEBUG("check for ranged weapon, it is {}", result ? "true" : "false");
        return result;
    }

    void StaminaCost::ManageSneakStamina(RE::PlayerCharacter* a_player, float a_deltaTime )
    {
        if (!a_player)
        {
            return;
        }

        if (RE::PlayerCharacter::IsGodMode())
        {
            return;
        }

        bool is_sneak_setting = Config::Settings::enable_sneak_stamina.GetValue();
        const bool is_sneaking = a_player->IsSneaking();
        const bool is_moving = Utility::IsMoving(a_player);
        float is_bow_out = HasRangedWeaponOut(a_player) ? 1.0f : 0.0f;

        RE::HandleEntryPoint(pepe::STEALTH_ARCHER::perkEntry, a_player, is_bow_out, pepe::STEALTH_ARCHER::perkCategory, ActorUtil::getWieldingWeapon(a_player));
        const bool should_drain_from_movement = is_sneaking && is_moving;
        const bool should_drain_with_bow = is_sneaking && is_bow_out != 0.0f;

        if (const bool should_drain = should_drain_from_movement || should_drain_with_bow; is_sneak_setting && should_drain)
        {
            if (const float stam = a_player->GetActorValue(RE::ActorValue::kStamina); stam > 0.0f)
            {
                float stam_reduce = 5.0f * a_deltaTime;
                RE::TESObjectARMO* armor = ActorUtil::GetEquippedChestPiece(a_player);
                RE::HandleEntryPoint(pepe::SNEAK_STAMINA::perkEntry, a_player, stam_reduce, pepe::SNEAK_STAMINA::perkCategory, armor);

                a_player->DamageActorValue(RE::ActorValue::kStamina, -stam_reduce);
                if (a_player->GetActorValue(RE::ActorValue::kStamina) <= stam_reduce && is_bow_out != 0.0f)
                {
                    a_player->actorState2.weaponState = RE::WEAPON_STATE::kWantToSheathe;
                    a_player->DrawWeaponMagicHands(false);
                }
            }
        }
    }

    float StaminaCost::GetJumpMassModifier(const RE::Actor* a_actor)
    {
        float mass = a_actor->GetActorValue(RE::ActorValue::kMass);
        mass = std::clamp(mass, 0.1f, MAX_JUMP_MASS_CALCULATOR);
        const float mass_ratio = mass / BASE_MASS_AVERAGE;
        return std::pow(mass_ratio, 1.2f);
    }

    float StaminaCost::GetJumpBaseCost()
    {
        return 10.0f;
    }

    float StaminaCost::CalculateJumpCost(RE::Actor* a_actor)
    {
        if (!Config::Settings::jump_stamina_cost.GetValue())
            return 0.0f;

        const float base_cost = GetJumpBaseCost();
        const float mass_modifier = GetJumpMassModifier(a_actor);
        float cost = base_cost * mass_modifier;
        RE::TESObjectARMO* armor = ActorUtil::GetEquippedChestPiece(a_actor);
        RE::HandleEntryPoint(pepe::JUMP_STAMINA::perkEntry, a_actor, cost, pepe::JUMP_STAMINA::perkCategory, armor);
        return cost;
    }

    bool StaminaCost::ManageCastStamina(RE::ActorMagicCaster* a_caster, RE::MagicItem* a_spell,
        RE::MagicSystem::CannotCastReason* a_reason)
    {
        const auto actor = a_caster->GetCasterAsActor();
        if (!actor)
        {
            return true;
        }
        const float cost = CalculateCastStaminaCost(a_caster, a_spell);

        if (cost <= 0.0f)
        {
            return true;
        }

        if (actor->GetActorValue(RE::ActorValue::kStamina) < cost)
        {
            if (a_reason)
                *a_reason = RE::MagicSystem::CannotCastReason::kCustomReasonNoStart;
            Utility::InterruptActor(actor,a_caster->GetCastingSource());
            RE::FlashHUDMeter(RE::ActorValue::kStamina);
            return false;
        }
        if (a_caster->state == RE::MagicCaster::State::kCharging)
        {
            actor->DamageActorValue(RE::ActorValue::kStamina, cost);
        }
        return true;
    }

    float StaminaCost::CalculateCastStaminaCost(RE::ActorMagicCaster* a_caster, RE::MagicItem* a_spell)
    {
        float stamina_cost = 0.0f;
        if (!Config::Settings::enable_cast_stamina.GetValue())
        {
            return stamina_cost;
        }
        const auto actor = a_caster->GetCasterAsActor();
        if (Utility::IsGod(actor))
        {
            return stamina_cost;
        }

        auto type_factor = static_cast<float>(Config::Settings::magic_stamina_cost_divider.GetValue());

        if (a_spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration)
            type_factor *= 2.0f;

        stamina_cost = a_caster->GetCurrentSpellCost() / type_factor;
        RE::HandleEntryPoint(pepe::CASTING_STAMINA::perkEntry, actor, stamina_cost,pepe::CASTING_STAMINA::perkCategory, a_spell);

        return stamina_cost;
    }

    RE::ActorValue StaminaCost::GetActorValueToUseForAttackCostBash(const RE::Actor* a_actor)
    {
        const RE::TESForm* left_hand_item = a_actor->GetEquippedObject(true);

        if (const auto weapon = left_hand_item ? left_hand_item->As<RE::TESObjectWEAP>() : nullptr; weapon)
        {
            if (!weapon->IsHandToHandMelee())
                return weapon->weaponData.skill.get();

            return RE::ActorValue::kNone;
        }

        if (const auto shield = left_hand_item ? left_hand_item->As<RE::TESObjectARMO>() : nullptr)
        {
            if (shield->IsLightArmor())
                return RE::ActorValue::kLightArmor;

            if (shield->IsHeavyArmor())
                return RE::ActorValue::kHeavyArmor;
        }
        if (!left_hand_item)
        {
            const RE::TESForm* right = a_actor->GetEquippedObject(false);
            if (const auto weapon = right ? right->As<RE::TESObjectWEAP>() : nullptr)
            {
                if (!weapon->IsHandToHandMelee())
                    return weapon->weaponData.skill.get();
            }
        }
        return RE::ActorValue::kNone;
    }

    RE::ActorValue StaminaCost::GetActorValueToUseForAttackCostAttack(RE::Actor* a_actor)
    {
        if (const auto weapon = ActorUtil::getWieldingWeapon(a_actor))
        {
            if (!weapon->IsHandToHandMelee())
            {
                return weapon->weaponData.skill.get();
            }
        }
        return RE::ActorValue::kNone;
    }

    float StaminaCost::CalculateWeaponWeightCost(RE::Actor* actor, const RE::ActorValue av_to_use, RE::TESForm* weapon_or_shield)
    {
        auto lvl = 1.0f;
        if (av_to_use != RE::ActorValue::kNone)
        {
            lvl = actor->GetActorValue(av_to_use);
        }

        const float weight = weapon_or_shield ? weapon_or_shield->GetWeight() : 0.5f;

        const float weight_ratio = weight / AttackStamina::AVERAGE_WEAPON_WEIGHT;
        float weight_mult = std::pow(weight_ratio, AttackStamina::WEIGHT_SCALING);

        const float skill_percentage = std::clamp(lvl / 100.0f, 0.0f, 1.0f);
        const float skill_reduction = 0.6 * std::pow(skill_percentage, AttackStamina::SKILL_SCALING);
        const float skill_factor = std::clamp(1.0f - skill_reduction, 0.0f, 1.0f);

        float cost = static_cast<float>(Config::Settings::base_stamina_cost_attacks.GetValue()) * weight_mult * skill_factor;
        RE::HandleEntryPoint(pepe::ATTACK_STAMINA::perkEntry, actor, cost, pepe::ATTACK_STAMINA::perkCategory, weapon_or_shield);

        return cost;
    }

    float StaminaCost::ManageAttackStaminaCost(RE::Actor* a_actor, const RE::BGSAttackData* a_attack)
    {
        constexpr float ret = -1.0f;
        if (!a_attack)
        {
            return ret;
        }
        if (a_attack->data.flags.any(RE::AttackData::AttackFlag::kBashAttack))
        {
            const auto av = GetActorValueToUseForAttackCostBash(a_actor);
            RE::TESForm* item_to_use = nullptr;
            if (RE::TESForm* left_hand = a_actor->GetEquippedObject(true); !left_hand)
            {
                item_to_use = a_actor->GetEquippedObject(false);
            }
            else
            {
                item_to_use = left_hand;
            }
            return 0.8f * CalculateWeaponWeightCost(a_actor, av, item_to_use);
        }
        if (a_attack->data.flags.none(RE::AttackData::AttackFlag::kPowerAttack))
        {
            const auto av = GetActorValueToUseForAttackCostAttack(a_actor);
            return CalculateWeaponWeightCost(a_actor, av, ActorUtil::getWieldingWeapon(a_actor));
        }

        return ret;
    }

    void StaminaCost::ApplyExhaustion(RE::Actor* a_actor, float a_cost)
    {
        bool useEx = Config::Settings::use_exhaustion.GetValue();
        if (a_actor->GetActorValue(RE::ActorValue::kStamina) < a_cost + 0.1) {
            if (!ActorUtil::HasEffectWithKeywordActive(a_actor, "StweaksExhaustion") && useEx) {
                MagicUtil::ApplySpell(a_actor, a_actor, Forms::FormLoader::exhaustion_spell);
            }
            RE::SourceActionMap::DoAction(a_actor, RE::DEFAULT_OBJECT::kActionRightInterrupt);
            RE::SourceActionMap::DoAction(a_actor, RE::DEFAULT_OBJECT::kActionLeftInterrupt);
        }
    }

    void StaminaCost::ManageJumpStamina(RE::Actor* a_actor)
    {
        if (!Config::Settings::jump_stamina_cost.GetValue())
        {
            return;
        }
        const float jump_cost = CalculateJumpCost(a_actor);

        if (Utility::IsGod(a_actor))
        {
            return;
        }

        if (const float stamina = a_actor->GetActorValue(RE::ActorValue::kStamina); stamina < jump_cost)
        {
            return;
        }
        a_actor->DamageActorValue(RE::ActorValue::kStamina, -jump_cost);
    }
}
