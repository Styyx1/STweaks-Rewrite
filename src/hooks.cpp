#include "hooks.h"
#include "actor-fade.h"
#include "attributes.h"
#include "curses.h"
#include "damage-manager.h"
#include "npc-changes.h"
#include "spells.h"
#include "stamina-manager.h"
#include "stealth-changes.h"


#undef PlaySound
namespace stweaks
{
    using namespace Config;

#pragma region AttributeGrowth & SneakStamina

    // nullsub loop that runs while the game is paused as well, other than PlayerCharacter::Update for example
    // NOTE: a_delta is always 0 in this function!
    void MainUpdate::MainUpdateHook(float a_delta)
    {
        _mainUpdateHook(a_delta);
        Attributes::UpdateAttributes(Cache::GetPlayerSingleton());
    }
    //Player Update loop. does not run while paused
    //Used for sneak stamina because i need a valid delta time parameter
    void MainUpdate::PlayerUpdateHook(RE::PlayerCharacter* a_this, float a_delta)
    {
        _playerUpdateHook(a_this, a_delta);
        StaminaCost::ManageSneakStamina(a_this, a_delta);
    }

#pragma endregion
#pragma region Jump
    //GetScale function inside the Jump function. vanilla returns the ref scale.
    //it's the only thing deciding jump height here. I expand it
    //used for mass jump, sneak jump, jump stamina and one of the curses
    float JumpHook::JumpHeightGetScale(RE::TESObjectREFR *refr)
    {
        const float scale = _jumpHeightGetScale(refr);

        auto *actor = refr ? refr->As<RE::Actor>() : nullptr;

        if (!actor)
        {
            return scale;
        }

        //jump stamina cost. Opt out for god mode in function
        StaminaCost::ManageJumpStamina(actor);

        return scale * JumpHeight::GetTotalModifier(actor);
    }

    // function to prevent jumping.
    // prevents jumping by not returning a valid CharacterController if the actor (player) has too little stamina to jump

    RE::bhkCharacterController* JumpHook::GetCharacterController(RE::AIProcess* a_proc)
    {
        if (!JumpHeight::HasEnoughStamina(a_proc))
        {
            return nullptr;
        }
        return _getCharacterControllerHook(a_proc);
    }

#pragma endregion
#pragma region CurseEnd
    // Function to end curses and clean up what they did
    void OnEffectEndHook::OnEffectEnd(RE::ScriptEffect *a_this)
    {
        _onScriptEffectEnd(a_this);
        Curses::RemoveCurse(a_this);
    }
#pragma endregion
#pragma region NPCFade
    // Actors Fade if they're a certain distance away from you. I made a similar feature for Stressful Darkness, but there it scales with stress
    void NPCFade::ActorUpdate(RE::Character *a_actor, float a_delta)
    {
        ActorFade::ProcessActorFade(a_actor);
        _onActorUpdate(a_actor, a_delta);
    }
#pragma endregion
#pragma region PreventCast
    //Prevent casting without stamina and prevent casting if afflicted with the silence curse
    bool PreventCast::CheckCast(RE::ActorMagicCaster *a_this, RE::MagicItem *a_spell, bool a_dualCast,
                                float *a_effectStrength, RE::MagicSystem::CannotCastReason *a_reason,
                                bool a_useBaseValueForCost)
    {
        const auto actor = a_this->actor;
        if (!a_spell || !actor)
        {
            return _checkCast(a_this, a_spell, a_dualCast, a_effectStrength, a_reason, a_useBaseValueForCost);
        }

        if (Utility::IsGod(actor))
        {
            return _checkCast(a_this, a_spell, a_dualCast, a_effectStrength, a_reason, a_useBaseValueForCost);
        }

        if (!MagicUtil::IsCastableSpell(a_spell))
        {
            return _checkCast(a_this, a_spell, a_dualCast, a_effectStrength, a_reason, a_useBaseValueForCost);
        }

        if (Curses::BlockSpellWithCurse(actor, a_spell))
        {
            Utility::InterruptActor(actor, a_this->GetCastingSource());
            if (a_reason)
            {
                *a_reason = RE::MagicSystem::CannotCastReason::kMagicka;
            }
            return false;
        }
        if (!StaminaCost::ManageCastStamina(a_this, a_spell, a_reason))
        {
            return false;
        }

        return _checkCast(a_this, a_spell, a_dualCast, a_effectStrength, a_reason, a_useBaseValueForCost);
    }

#pragma endregion
#pragma region PlayerPotionUsed
    // track usage of potion to see if a curse needs to be removed
    void PlayerPotionUsed::PlayerUsePotion(uint64_t self, RE::AlchemyItem *alch, uint64_t extralist)
    {
        if (alch && alch->HasKeywordString(stweaks::keywords::kCure))
        {
            RE::PlayerCharacter *player = Cache::GetPlayerSingleton();
            Utility::Curses::CleanseCurse(player);
        }
        return _playerUsePotion(self, alch, extralist);
    }
    //make arrows heavier with a curse
    float HighGravityArrows::GetGravityArrow(RE::Projectile *a_this)
    {
        const float result = _arrowGetGravity(a_this);
        if (!Config::Settings::enable_curses.GetValue())
            return result;

        const auto shooterRef = a_this->shooter.get();
        if (const auto actorShooter = shooterRef ? shooterRef->As<RE::Actor>() : nullptr)
        {
            if (Utility::ActiveEffectHasNewDiseaseKeyword(actorShooter, stweaks::keywords::kBow.data()))
            {
                return result * 10.0f;
            }
        }
        return result;
    }
#pragma endregion
#pragma region StaminaAttackCost
    //attack stamina cost. Scales with weapon weight and PowerAttackStaminaCost perk entry
    //also applies stamina exhaustion that lowers weapon damage and speed when you attack with too low stamina
    float StaminaAttackCost::GetAttackCost(RE::ActorValueOwner *a_owner, RE::BGSAttackData *attack)
    {
        const float result = _getAttackCost(a_owner, attack);
        if (!Settings::enable_attack_stamina.GetValue())
        {
            return result;
        }
        const auto actor = skyrim_cast<RE::Actor *>(a_owner);

        if (Utility::IsGod(actor))
        {
            return _getAttackCost(a_owner, attack);
        }

        if (const auto res = StaminaCost::ManageAttackStaminaCost(actor, attack); res >= 0.0f)
        {
            StaminaCost::ApplyExhaustion(actor, res);
            return res;
        }
        return _getAttackCost(a_owner, attack);
    }

#pragma endregion
#pragma region LoadWithResistance & Adjust Level & Mass System  & StaminaRegenAdjuster
    // Load Actor hook
    // manages resistances to make magic early game a bit easier, sets their level to max 10 below yours, adds the mass system to npcs and adjusts npc's regen rate
    RE::NiAVObject *LoadWithResistance::LoadActor(RE::Actor *a_this, bool arg)
    {

        const auto actor = _loadActorHook(a_this, arg);

        Utility::AdjustMass(a_this);

        const auto player = Cache::GetPlayerSingleton();
        if (a_this == player)
            return actor;

        const uint16_t player_level = player ? player->GetLevel() : 5;
        NPCChanges::ChangeNPCResistances(a_this, player_level);
        NPCChanges::LevelUpNpcs(a_this, player_level);

        return actor;
    }
    RE::NiAVObject *LoadWithResistance::LoadPlayer(RE::Actor *a_this, bool arg)
    {
        const auto actor = _loadPlayerHook(a_this, arg);
        // Ties Mass Actor Value to inventory weight. only when the player is loaded. there's a function later on for inventory changes
            Utility::AdjustMass(a_this);
        REX::DEBUG("Player Mass on load: {}", a_this->GetActorValue(RE::ActorValue::kMass));
        return actor;
    }
    // Stamina and magica regen is based on the max stamina in vanilla
    // this means the higher your stamina, the faster your regen which makes late game stamina regen too fast for attack stamina to have any impact
    // this function uses a flat base basically making the first few levels faster to regen and it falls off pretty quick afterward
    // should keep stamina an important resource to manage
    // same goes for magicka.
    // it's not made for health cause health calcs aren't that simple, and i don't think it's as busted as the other 2
    float StaminaRegenAdjuster::GetStamBase(RE::Character *a_char, RE::ActorValue a_av)
    {
        const float original_value = _getBaseStamina(a_char, a_av);
        return RegenChanges::GetNewRegeneration(a_char, a_av, original_value);
    }
#pragma endregion
#pragma region Spell Damage & Weapon Damage

    //damage calcs for Spells.
    //manages OneShot Protection, damage caps and damage randomisation
    void SpellCap::ApplyPerkEntrySpellMag(RE::BGSPerkEntry::EntryPoint a_entry, RE::Actor *caster,
                                                 RE::SpellItem *spell, RE::Actor *target, float &damage)
    {
        const float original_damage = damage;
        _applySpellMagPerkEntry(a_entry, caster, spell, target, damage);

        if (!DamageCalculations::IsSpellValidForCalcs(spell))
            return;

        DamageCalculations::ApplyDamageRanges(damage, Config::Settings::magic_lower_range.GetValue(), Config::Settings::magic_upper_range.GetValue());
        DamageCalculations::ApplyDamageCap(caster, target, original_damage, damage);
    }
    //same as above but for weapons
    void DamageOut::ApplyPerkEntryAttack(RE::BGSPerkEntry::EntryPoint a_entry, RE::Actor *attacker,
                                         RE::TESObjectWEAP *weapon, RE::TESObjectREFR *target, float &damage)
    {
        _applyPerkEntryAttack(a_entry, attacker, weapon, target, damage);

        RE::Actor *const &target_actor = target ? target->As<RE::Actor>() : nullptr;
        if (!DamageCalculations::IsValidTargetAndAttacker(attacker, target_actor))
            return;

        // handle curse modifier for attack damage
        damage = DamageCalculations::GetCurseModifiedDamage(attacker, damage);

        // safeguard to actually make sure the first hit that ends the ethereal effect doesn't deal damage.
        // If vanilla does that, it's pretty inconsistent
        damage = DamageCalculations::GetEtherealModifiedDamage(attacker, damage);

        // adjust damage with follower. the more followers, the less damage those and you deal.
        damage = DamageCalculations::GetFollowerModifiedDamage(attacker, damage);

        // apply certain ranges to damage to not always deal the same amount with every hit
        damage = DamageCalculations::GetDamageRangeModifiedDamage(damage, Settings::weapon_lower_range.GetValue(), Settings::weapon_upper_range.GetValue());

        //quest items are without weight. so they can be great weapons without contributing to your inventory weight (mainly useful for low weight setups like requiem)
        // this function scales their damage to almost 0, so you don't get a free weight weapon. Exceptions can be set in a json file
        damage = DamageCalculations::GetQuestItemModifiedDamage(attacker, weapon, damage);

        // attacks of opportunity. they count as crit and deal increased damage
        damage = DamageCalculations::GetOpportunityModifiedDamage(attacker, target_actor, damage);

        REX::INFO("done with apply perk entry");
    }

    void DamageOut::ManageCombatHit(RE::Actor* a_this, RE::HitData* a_hitData)
    {
        const auto attacker = a_hitData->aggressor.get();
        if (!attacker || !a_this)
        {
            return _manageCombatHit(a_this, a_hitData);
        }
        const bool is_sneak = attacker->IsSneaking() && a_this->RequestDetectionLevel(attacker.get()) <= 0;
        const bool uses_bow = a_hitData->weapon && a_hitData->weapon->IsRanged();

        a_hitData->totalDamage = DamageCalculations::GetOneShotProtectedDamagePhysical(attacker.get(), a_this, a_hitData->totalDamage, is_sneak, uses_bow);

        const auto power_attack = a_hitData->flags.any(RE::HitData::Flag::kPowerAttack);
        const auto sneak_attack = a_hitData->flags.any(RE::HitData::Flag::kSneakAttack);

        if (!a_hitData->weapon)
            return _manageCombatHit(a_this, a_hitData);

        const float dmg_cap = !a_hitData->weapon->IsHandToHandMelee() ? static_cast<float>(a_hitData->weapon->attackDamage) * 5.f : 200.f;

        a_hitData->totalDamage = DamageCalculations::GetCappedDamagePhysical(a_hitData->totalDamage, dmg_cap, power_attack, sneak_attack);

        return _manageCombatHit(a_this, a_hitData);
    }

    // https://www.nexusmods.com/skyrimspecialedition/mods/73514 partially taken
    // from this mod
    // cast speed changes with skill level
    void CastingSpeed::CasterUpdate(RE::ActorMagicCaster *a_this, float a_delta)
    {
        if (!CastSpeedFunctions::ShouldAllowCastSpeedChange(a_this))
            return _casterUpdate(a_this, a_delta);

        const auto actor = a_this->GetCasterAsActor();
        if (!actor)
            return _casterUpdate(a_this, a_delta);

        const auto spell = a_this->currentSpell ? a_this->currentSpell->As<RE::SpellItem>() : nullptr;
        if (!spell)
            return _casterUpdate(a_this, a_delta);

        if (!MagicUtil::IsSpellPlayable(spell) || MagicUtil::IsPermanent(spell))
            return _casterUpdate(a_this, a_delta);

        const bool excluded_types = spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration ||
            spell->GetSpellType() == RE::MagicSystem::SpellType::kVoicePower;
        if (excluded_types)
        {
            return _casterUpdate(a_this, a_delta);
        }

        const float new_time = GetCastingSpeedMult(actor->GetActorValue(spell->GetAssociatedSkill()));
        return _casterUpdate(a_this, a_delta * new_time);
    }
#pragma endregion
#pragma region EquipMassChanges & DetectionInTallGrass
    // mass change with equipment weight for npcs
    void EquipHandler::OnItemEquipped(RE::Actor *a_this, bool a_playAnim)
    {
        _onItemEquippedActor(a_this, a_playAnim);
        Utility::AdjustMass(a_this);
        REX::DEBUG("NPC mass after equip change: {}", a_this->GetActorValue(RE::ActorValue::kMass));
    }
    //same as above but for the player
    void EquipHandler::OnItemEquippedPlayer(RE::PlayerCharacter *a_this, bool a_playAnim)
    {
        _onItemEquippedActor(a_this, a_playAnim);
        Utility::AdjustMass(a_this);
        REX::DEBUG("Player mass after equip change: {}", a_this->GetActorValue(RE::ActorValue::kMass));
    }

    // detection changes when staning on areas flagged as tall grass. needs landscape textures from a json file
    // json file provided by Ylikollikas
    void Detection::DoCalculateDetection(RE::Actor *a_this, RE::Actor *target, std::int32_t &score, bool &spotted,
                                         bool &hasLOS, std::int32_t &reason, RE::NiPoint3 &lastPos, std::int32_t &soundLvl,
                                         float &unk8, float &unk9)
    {
        _doCalculateDetection(a_this, target, score, spotted, hasLOS, reason, lastPos, soundLvl, unk8, unk9);

        if (!target)
            return;

        RE::BGSPerk *perk = Forms::FormLoader::tall_grass_perk;
        if (EnvironmentStealth::ShouldHaveTallGrassPerk(target))
            ActorUtil::SafeApplyPerk(target, perk);
        else
            target->RemovePerk(perk);
    }

#pragma endregion
} // namespace Hooks
