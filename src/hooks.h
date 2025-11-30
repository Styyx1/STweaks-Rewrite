#pragma once
#include "Events.h"

namespace Hooks
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
    return OpportunityModifiers[static_cast<int>(type)];
  }
};

struct AVStorage : public REX::Singleton<AVStorage>
{
    std::unordered_map<RE::ActorValue, uint16_t> attribute_xp;
    inline uint16_t get_attribute_xp(RE::ActorValue av)
    {
        if (attribute_xp.empty())
            return 0;
        return attribute_xp[av];
    }
};

class MainUpdate
{
  public:
    static inline int frameCount = 0;
    static inline std::chrono::steady_clock::time_point sprintStartTime;
    static inline bool isSprinting = false;

  private:
    static void MainUpdateHook(float a_delta);
    static bool HasRangedWeaponDrawn(RE::PlayerCharacter *player);
    static float GetCarryPercentage(RE::PlayerCharacter *player);
	static void ManageSneakStamina(RE::PlayerCharacter* player);
    static void ManageAttributeGrowth(RE::PlayerCharacter* player);
    static inline TimerUtil::Timer av_timer;
    static inline float pl_mass;
    static inline constexpr std::array restricted_menu_names{
        RE::BarterMenu::MENU_NAME,    RE::BookMenu::MENU_NAME,     RE::Console::MENU_NAME,
        RE::ContainerMenu::MENU_NAME, RE::CraftingMenu::MENU_NAME, RE::DialogueMenu::MENU_NAME,
        RE::FavoritesMenu::MENU_NAME, RE::GiftMenu::MENU_NAME,     RE::InventoryMenu::MENU_NAME,
        RE::JournalMenu::MENU_NAME,   RE::LevelUpMenu::MENU_NAME,  RE::LockpickingMenu::MENU_NAME,
        RE::MagicMenu::MENU_NAME,     RE::MapMenu::MENU_NAME,      RE::RaceSexMenu::MENU_NAME,
        RE::SleepWaitMenu::MENU_NAME, RE::StatsMenu::MENU_NAME,    RE::TrainingMenu::MENU_NAME,
        RE::TutorialMenu::MENU_NAME,  RE::TweenMenu::MENU_NAME,
    };
    static inline std::vector<std::string> a_menuNames{restricted_menu_names.begin(), restricted_menu_names.end()};
    static inline bool IsAnyOfMenuOpen(const std::vector<std::string> &a_menuNames)
    {
        auto a_ui = RE::UI::GetSingleton();
        if (!a_ui)
            return true;
        for (const std::string_view menuName : a_menuNames)
        {
            if (a_ui->IsMenuOpen(menuName))
            {
                return true;
            }
        }
        return false;
    }
    static inline REL::Hook _Hook1{REL::ID(36564), 0xc26, &MainUpdateHook};
};

class JumpHeight
{
  public:
    static float JumpHeightGetScale(RE::TESObjectREFR *refr);

  private:
    static inline REL::Hook _Hook3{REL::ID(37257), 0x17f, &JumpHeightGetScale};
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
    static float GetWeightMult(RE::Actor *actor, float weight, RE::ActorValue av_to_use);

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
    static inline REL::Hook _Hook24{REL::ID(44016), 0x96, ApplyPerkEntryAttack};

    static float GetOpportunityModifier(RE::Actor *victim, RE::Actor *attacker, bool notification);
};

class CastingSpeed
{
    static void CasterUpdate(RE::ActorMagicCaster *a_this, float a_delta);

    static inline float GetCastingSpeedMult(float skillLevel)
    {
        skillLevel = std::clamp(skillLevel, 0.0f, 100.0f);
        float min_speed = Config::Settings::min_cast_speed.GetValue();
        float max_speed = Config::Settings::max_cast_speed.GetValue();
        return std::lerp(min_speed, max_speed, skillLevel / 100.0f);
    }

    static inline REL::HookVFT _Hook25{RE::VTABLE_ActorMagicCaster[0], 0x1d, CasterUpdate};
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
