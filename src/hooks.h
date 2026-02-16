#pragma once
#include "Events.h"

namespace Hooks
{

class MainUpdate
{
    static void MainUpdateHook(float a_delta);
    static void PlayerUpdateHook(RE::PlayerCharacter* a_this, float a_delta);

    static inline REL::Hook _mainUpdateHook{REL::ID(36564), 0xc26, &MainUpdateHook};
    static inline REL::HookVFT _playerUpdateHook{RE::VTABLE_PlayerCharacter[0], 0xAD, &PlayerUpdateHook};
};

class JumpHeight
{
  public:
    static float JumpHeightGetScale(RE::TESObjectREFR *refr);
    static RE::bhkCharacterController* GetCharacterController(RE::AIProcess* a_proc);

  private:
    static inline REL::Hook _jumpHeightGetScale{REL::ID(37257), 0x17f, &JumpHeightGetScale};
    static inline REL::Hook _getCharacterControllerHook{REL::ID(37257), 0x35, &GetCharacterController};
};

class OnEffectEndHook
{
  private:
    static void OnEffectEnd(RE::ScriptEffect *a_this);
    static inline REL::HookVFT _Hook5{RE::VTABLE_ScriptEffect[0], 0x15, &OnEffectEnd};
};

class NPCFade
{
  public:
    static Utility::GameDifficulty g_cachedDifficulty;

  private:
    static void ActorUpdate(RE::Character *a_actor, float a_delta);
    static inline REL::HookVFT _Hook6{RE::VTABLE_Character[0], 0xAD, &ActorUpdate};
};

class PreventCast
{
  private:
    static bool CheckCast(RE::ActorMagicCaster *a_this, RE::MagicItem *a_spell, bool a_dualCast,
                          float *a_effectStrength, RE::MagicSystem::CannotCastReason *a_reason,
                          bool a_useBaseValueForCost);
    static void InterruptActor(RE::Actor *a_actor, RE::MagicSystem::CastingSource a_castingSource);

    static inline REL::HookVFT _Hook7{RE::VTABLE_ActorMagicCaster[0], 0x0A, CheckCast};
};

class PlayerPotionUsed
{
  private:
    static void PlayerUsePotion(uint64_t self, RE::AlchemyItem *alch, uint64_t extralist);
    static inline REL::Hook _Hook8{REL::ID(40690), 0x15, PlayerUsePotion};
};

class HighGravityArrows
{

  private:
    static float GetGravityArrow(RE::Projectile *a_this);

    static inline REL::HookVFT _Hook9{RE::VTABLE_ArrowProjectile[0], 0xB5, GetGravityArrow};
};

class StaminaAttackCost
{

  private:
    static constexpr float MIN_COST = 5.0f;
    static constexpr float MAX_COST = 50.0f;
    static constexpr float AVERAGE_WEAPON_WEIGHT = 12.5f;
    static constexpr float WEIGHT_SCALING = 1.15f;
    static constexpr float SKILL_SCALING = 1.10f;

    static float GetAttackCost(RE::ActorValueOwner *a_owner, RE::BGSAttackData *attack);
    static float GetWeightMult(const RE::Actor *actor, float weight, RE::ActorValue av_to_use);

    static inline REL::Hook _Hook12{REL::ID(38603), 0x171, &GetAttackCost};
};

class LoadWithResistance
{

  private:
    static constexpr uint16_t LEVEL_CAP = 5;
    static RE::NiAVObject *LoadActor(RE::Actor *a_this, bool arg);
    static RE::NiAVObject *LoadPlayer(RE::Actor *a_this, bool arg);
    static inline REL::HookVFT _Hook13{RE::VTABLE_Character[0], 0x06A, &LoadActor};
    static inline REL::HookVFT _Hook14{RE::VTABLE_PlayerCharacter[0], 0x06A, &LoadPlayer};
};

class StaminaRegenAdjuster
{
  public:
  private:
    static float GetStamBase(RE::Character *a_char, RE::ActorValue a_av);
    static inline REL::Hook _Hook21{REL::ID(38459), 0x26, &GetStamBase};
};

class SpellCap
{
    static void ApplyPerkEntrySpellMag(RE::BGSPerkEntry::EntryPoint a_entry, RE::Actor *caster, RE::SpellItem *spell,
                                       RE::Actor *target, float &damage);
    static inline REL::Hook _Hook23{REL::ID(34053), 0x60, ApplyPerkEntrySpellMag};
};

class DamageOut
{
    static void ApplyPerkEntryAttack(RE::BGSPerkEntry::EntryPoint a_entry, RE::Actor *attacker,
                                     RE::TESObjectWEAP *weapon, RE::TESObjectREFR *target, float &damage);

    static void ManageCombatHit(RE::Actor* a_this, RE::HitData* a_hitData);

    static inline REL::Hook _applyPerkEntryAttack{REL::ID(44016), 0x96, ApplyPerkEntryAttack};
    static inline REL::Hook _manageCombatHit{REL::ID(38627), 0x4a8, ManageCombatHit};

    static float GetOpportunityModifier(RE::Actor *victim, RE::Actor *attacker, bool notification);
};

class CastingSpeed
{
    static void CasterUpdate(RE::ActorMagicCaster *a_this, float a_delta);

    static inline float GetCastingSpeedMult(float skillLevel)
    {
        skillLevel = std::clamp(skillLevel, 0.0f, 100.0f);
        const auto min_speed = static_cast<float>(Config::Settings::min_cast_speed.GetValue());
        const auto max_speed = static_cast<float>(Config::Settings::max_cast_speed.GetValue());
        return std::lerp(min_speed, max_speed, skillLevel / 100.0f);
    }

    static inline REL::HookVFT _casterUpdate{RE::VTABLE_ActorMagicCaster[0], 0x1d, CasterUpdate};
};

class EquipHandler
{
    static void OnItemEquipped(RE::Actor *a_this, bool a_playAnim);
    static void OnItemEquippedPlayer(RE::PlayerCharacter *a_this, bool a_playAnim);
 

    static inline REL::HookVFT _Hook26{RE::VTABLE_Character[0], 0x0b2, OnItemEquipped};
    static inline REL::HookVFT _Hook27{RE::VTABLE_PlayerCharacter[0], 0x0b2, OnItemEquippedPlayer};

};

class Detection
{

    static void DoCalculateDetection(RE::Actor *a_this, RE::Actor *target, std::int32_t &score, bool &spotted,
                                     bool &hasLOS, std::int32_t &reason, RE::NiPoint3 &lastPos, std::int32_t &soundLvl,
                                     float &unk8, float &unk9);
    static inline REL::Hook _Hook28{REL::ID(42742), 0x67b, DoCalculateDetection};
    static inline bool IsStandingInTallGrass(RE::Actor *target);
};


static RE::ActorValue LookupActorValueByName(const char *av_name)
{
    using func_t = decltype(&LookupActorValueByName);
    REL::Relocation<func_t> func{REL::ID(27203)};
    return func(av_name);
}

// returns from 0 - 1.0 1 == 100%
static float GetActorValuePercentage(RE::Actor *a_actor, RE::ActorValue a_av)
{
    using func_t = decltype(&GetActorValuePercentage);
    REL::Relocation<func_t> target{REL::ID(37337)};
    return target(a_actor, a_av);
}

static bool IsQuestObject(RE::ExtraDataList *a_list)
{
    using func_t = decltype(&IsQuestObject);
    REL::Relocation<func_t> target{REL::ID(12052)};
    return target(a_list);
}

} // namespace Hooks
