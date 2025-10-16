#pragma once
#include "Events.h"

namespace Hooks
{
    void Install();

    class CombatHit
    {
    public:
        static void InstallH0();

    private:
        static float WeaponTypeModifier(RE::TESObjectWEAP* a_weap, float f_in);
        static void RandomiseDamage(RE::Actor* a_this, RE::HitData* a_hitData);
        static void CHit(RE::Actor* a_this, RE::HitData* a_hitData);

        static inline REL::Hook _Hook0{ REL::ID(38627), 0x4a8, &CHit };
    };

    class MainUpdate
    {
    public:
		static void InstallH1();
        static inline int frameCount = 0;
        static inline std::chrono::steady_clock::time_point sprintStartTime;
        static inline bool isSprinting = false;

    private:
        static void PlayerUpdate(RE::PlayerCharacter* p, float a_delta);
        static bool HasRangedWeaponDrawn(RE::PlayerCharacter* player);

		static inline REL::HookVFT _Hook1{ RE::VTABLE_PlayerCharacter[0], 0xAD, &PlayerUpdate };
    };

    class AdjustActiveEffect
    {
    public:
		static void InstallH2();
        static void thunk(RE::ActiveEffect* a_this, float a_power, bool a_onlyHostile);

    private:
		static inline REL::Hook _Hook2{ REL::ID(34547), 0x656, &thunk };
    };

    class JumpHeight
    {
    public:
		static void InstallH3();
        static float JumpHeightGetScale(RE::TESObjectREFR* refr);

    private:

		static inline REL::Hook _Hook3{ REL::ID(37257), 0x17f, &JumpHeightGetScale };
    };

    class DealtMeleeDamage
    {
    public:
        static void InstallH4();
		
    private:
        static float MeleeDamage(RE::TESObjectWEAP* _weap, RE::ActorValueOwner* a, float DamageMult, char isbow);
        static bool ActorHasQuestObjectInHand(RE::Actor* actor);
		static inline REL::Hook _Hook4{ REL::ID(44001), 0x1a4, &MeleeDamage };

    };

    class OnEffectEndHook
    {
    private:
        static void OnEffectEnd(RE::ScriptEffect* a_this);
		static inline REL::HookVFT _Hook5{ RE::VTABLE_ScriptEffect[0], 0x15, &OnEffectEnd };
    };

    class NPCFade
    {
    public:
		static void InstallH6();
        static Utility::GameDifficulty g_cachedDifficulty;

    private:
        static void ActorUpdate(RE::Character* a_actor, float a_delta);
		static inline REL::HookVFT _Hook6{ RE::VTABLE_Character[0], 0xAD, &ActorUpdate };
    };

    class PreventCast
    {
    public:
		static void InstallH7();
    private:
        static bool CheckCast(RE::ActorMagicCaster* a_this, RE::MagicItem* a_spell, bool a_dualCast, float* a_effectStrength, RE::MagicSystem::CannotCastReason* a_reason, bool a_useBaseValueForCost);
        static void InterruptActor(RE::Actor* a_actor, RE::MagicSystem::CastingSource a_castingSource);

        static inline REL::HookVFT _Hook7{ RE::VTABLE_ActorMagicCaster[0], 0x0A, CheckCast };
    };

    class PlayerPotionUsed
    {
    public:
		static void InstallH8();
    private:
        static void PlayerUsePotion(uint64_t self, RE::AlchemyItem* alch, uint64_t extralist);
        static inline REL::Hook _Hook8{ REL::ID(40690), 0x15, PlayerUsePotion };
    };

    class HighGravityArrows {
    public:
        static void InstallH9();
    private:
	
        static float GetGravityArrow(RE::Projectile* a_this);

		static inline REL::HookVFT _Hook9{ RE::VTABLE_ArrowProjectile[0], 0xB5, GetGravityArrow };
    };

    static RE::ActorValue LookupActorValueByName(const char* av_name)
    {
        // SE: 0x3E1450, AE: 0x3FC5A0, VR: ---
        using func_t = decltype(&LookupActorValueByName);
        REL::Relocation<func_t> func{ REL::ID(27203) };
        return func(av_name);
    }

    // returns from 0 - 1.0 1 == 100%
    static float GetActorValuePercentage(RE::Actor* a_actor, RE::ActorValue a_av)
    {
        using func_t = decltype(&GetActorValuePercentage);
        REL::Relocation<func_t> target{ REL::ID(37337) };
        return target(a_actor, a_av);
    }

    static bool IsQuestObject(RE::ExtraDataList* a_list) {
        using func_t = decltype(&IsQuestObject);
        REL::Relocation<func_t> target{ REL::ID(12052) };
        return target(a_list);
    }

} // namespace Hooks
