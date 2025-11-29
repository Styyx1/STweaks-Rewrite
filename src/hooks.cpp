#include "hooks.h"

namespace Hooks
{

using namespace Config;

bool wasEnraged = false;

void MainUpdate::MainUpdateHook(float a_delta)
{
    auto *player = Cache::GetPlayerSingleton();

    if (!Settings::enable_sneak_stamina.GetValue())
    {
        if (Utility::HasSpell(player, Forms::FormLoader::sneak_stamina_spell))
        {
            player->RemoveSpell(Forms::FormLoader::sneak_stamina_spell);
        }
        return _Hook1(a_delta);
    }

    if (frameCount > 10)
    {
        frameCount = 0;
    }
    else
    {
        if (player->IsGodMode())
        {
            if (player->HasSpell(Forms::FormLoader::sneak_stamina_spell))
                player->RemoveSpell(Forms::FormLoader::sneak_stamina_spell);
        }
        else
        {
            switch (frameCount)
            {
            case 1:
                if (player->IsSneaking() && Utility::IsMoving(player) && Settings::enable_sneak_stamina.GetValue() ||
                    player->IsSneaking() && HasRangedWeaponDrawn(player) && Settings::enable_sneak_stamina.GetValue())
                {
                    if (!Utility::HasSpell(player, Forms::FormLoader::sneak_stamina_spell))
                    {
                        player->AddSpell(Forms::FormLoader::sneak_stamina_spell);
                    }
                    if (player->GetActorValue(RE::ActorValue::kStamina) <= 5 &&
                        player->GetActorValue(RE::ActorValue::kStamina) > 0 && HasRangedWeaponDrawn(player))
                    {
                        player->actorState2.weaponState = RE::WEAPON_STATE::kWantToSheathe;
                        player->DrawWeaponMagicHands(false);
                    }
                }
                else if (Utility::HasSpell(player, Forms::FormLoader::sneak_stamina_spell))
                {
                    player->RemoveSpell(Forms::FormLoader::sneak_stamina_spell);
                }

                break;
            default:
                break;
            }
        }
    }
    frameCount++;

    if (av_timer.ElapsedSeconds() >= 45)
    {
        av_timer.Reset();

        float perc_health = GetActorValuePercentage(player, RE::ActorValue::kHealth);
        float perc_stamina = GetActorValuePercentage(player, RE::ActorValue::kStamina);
        float perc_magicka = GetActorValuePercentage(player, RE::ActorValue::kMagicka);
        float perc_carry = GetCarryPercentage(player);

        auto *av_store = AVStorage::GetSingleton();
        if (perc_health > 0.4 && perc_health < 0.95)
            av_store->attribute_xp[RE::ActorValue::kHealth] += 1;
        if (perc_stamina > 0.4 && perc_stamina < 0.95)
            av_store->attribute_xp[RE::ActorValue::kStamina] += 1;
        if (perc_magicka > 0.4 && perc_magicka < 0.95)
            av_store->attribute_xp[RE::ActorValue::kMagicka] += 1;
        if (perc_carry > 0.4 && perc_carry < 0.95)
            av_store->attribute_xp[RE::ActorValue::kCarryWeight] += 1;

        auto it = av_store->attribute_xp.begin();
        for (it; it != av_store->attribute_xp.end(); ++it)
        {
            if (it->second >= 100)
            {
                player->SetBaseActorValue(it->first, player->GetBaseActorValue(it->first) + 1);
                it->second -= 100;
            }
        }
    }

    if (!av_timer.IsRunning() && !MiscUtil::IsAnyOfMenuOpen(Cache::GetUISingleton(), a_menuNames))
    {
        av_timer.Start();
    }
    if (av_timer.IsRunning() && MiscUtil::IsAnyOfMenuOpen(Cache::GetUISingleton(), a_menuNames))
    {
        av_timer.Stop();
    }

    if (!Config::Settings::enable_automatic_attributes.GetValue())
        av_timer.Stop();

    return _Hook1(a_delta);
}

bool MainUpdate::HasRangedWeaponDrawn(RE::PlayerCharacter *player)
{
    bool result = false;
    RE::TESObjectWEAP *weap = Utility::getWieldingWeapon(player);
    if (weap)
    {
        if (weap->IsBow() && player->IsWeaponDrawn() || weap->IsCrossbow() && player->IsWeaponDrawn())
            result = true;
    }

    REX::DEBUG("check for ranged weapon, it is {}", result ? "true" : "false");
    return result;
}

float MainUpdate::GetCarryPercentage(RE::PlayerCharacter *player)
{

    return player->GetActorValue(RE::ActorValue::kInventoryWeight) /
           player->GetActorValue(RE::ActorValue::kCarryWeight);
}

float JumpHeight::JumpHeightGetScale(RE::TESObjectREFR *refr)
{
    float scale = refr->GetScale();
    RE::Actor *actor = refr->As<RE::Actor>();

    if (!actor)
    {
        return scale;
    }

    float mass = 1.0f;
    if (Config::Settings::enable_mass_based_jump_height.GetValue())
    {
        mass = actor->GetActorValue(RE::ActorValue::kMass);
    }

    if (actor->IsSneaking() && Settings::enable_sneak_jump_limit.GetValue())
    {
        scale *= Settings::sneak_height_modifier.GetValue();
    }
    float ju_modifier = (float)sqrt(1.0 / mass);

    float curse_modi = 1.0f;
    if (Config::Settings::enable_diseases.GetValue())
    {
        if (Utility::ActiveEffectHasNewDiseaseKeyword(actor, Forms::FormConstants::jump_curse_key))
        {
            curse_modi = 0.5f;
        }
    }
    if (Config::Settings::jump_stamina_cost.GetValue())
    {
        float min_cost = 10.0f;
        float max_cost = 50.0f;
        float max_mass = 80.0f;

        float jump_cost = min_cost + (max_cost - min_cost) *
                                         std::min(actor->GetActorValue(RE::ActorValue::kMass), max_mass) / max_mass;

        float stamina = actor->GetActorValue(RE::ActorValue::kStamina);
        if (stamina < jump_cost)
            return 0.0;
        actor->DamageActorValue(RE::ActorValue::kStamina, jump_cost);
    }

    return scale *= ju_modifier * curse_modi;
}

void OnEffectEndHook::OnEffectEnd(RE::ScriptEffect *a_this)
{
    _Hook5(a_this);
    if (!Config::Settings::enable_diseases.GetValue())
        return;

    auto hitEv = Events::HitEventHandler::GetSingleton();
    if (a_this->target && a_this->GetBaseObject()->HasAnyKeywordByEditorID(Forms::FormConstants::diseases))
    {
        REX::DEBUG("curse ended");
        RE::Actor *aff_actor = skyrim_cast<RE::Actor *>(a_this->target);
        if (a_this->GetBaseObject()->HasKeywordString(Forms::FormConstants::diseases[0]))
        {
            float currentPenaltyH = hitEv->storedHealth_disease;
            if (currentPenaltyH > 0)
            {
                hitEv->storedHealth_disease = 0.0f;
                aff_actor->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kHealth,
                                         currentPenaltyH);
            }
        }
        if (a_this->GetBaseObject()->HasKeywordString(Forms::FormConstants::diseases[1]))
        {
            float currentPenaltyS = hitEv->storedStamina_disease;
            if (currentPenaltyS > 0)
            {
                hitEv->storedStamina_disease = 0.0f;
                aff_actor->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kStamina,
                                         currentPenaltyS);
            }
        }
        if (a_this->GetBaseObject()->HasKeywordString(Forms::FormConstants::diseases[2]))
        {
            float currentPenaltyM = hitEv->storedMagicka_disease;
            if (currentPenaltyM > 0)
            {
                hitEv->storedMagicka_disease = 0.0f;
                aff_actor->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMagicka,
                                         currentPenaltyM);
            }
        }
    }
}

void NPCFade::ActorUpdate(RE::Character *a_actor, float a_delta)
{

    if (!Config::Settings::enable_fading_actors.GetValue())
    {
        if (a_actor->GetAlpha() < 1.0)
            a_actor->SetAlpha();

        _Hook6(a_actor, a_delta);
        return;
    }

    RE::PlayerCharacter *player = Cache::GetPlayerSingleton();
    float distance_full_fade = 2400.00;
    float distance_no_fade = 1200.00f;
    float light_level_threshold = 30.0f;
    float fade_range = distance_full_fade - distance_no_fade;
    float minAlpha = 0.05f;

    auto difficulty_level = player->difficulty;
    float difficultyMult = static_cast<float>(difficulty_level) / 5.0f;

    if (float distance = a_actor->GetDistance(player); distance >= distance_full_fade && a_actor->GetHighProcess() &&
                                                       a_actor->GetHighProcess()->lightLevel <= light_level_threshold)
    {
        float fade_progress = std::min(std::max((distance - distance_no_fade) / fade_range, 0.0f), 1.0f);
        float alpha = std::lerp(1.0f, minAlpha, fade_progress);
        a_actor->SetAlpha(alpha);
        a_actor->dialogueItemTarget;
    }
    else
    {
        a_actor->SetAlpha();
    }

    _Hook6(a_actor, a_delta);
}

bool PreventCast::CheckCast(RE::ActorMagicCaster *a_this, RE::MagicItem *a_spell, bool a_dualCast,
                            float *a_effectStrength, RE::MagicSystem::CannotCastReason *a_reason,
                            bool a_useBaseValueForCost)
{
    if (!a_spell)
        return _Hook7(a_this, a_spell, a_dualCast, a_effectStrength, a_reason, a_useBaseValueForCost);

    auto type = a_spell->GetSpellType();
    switch (type)
    {
        using st = RE::MagicSystem::SpellType;
    case st::kSpell:
    case st::kPower:
    case st::kVoicePower:
    case st::kLesserPower:
        break;
    default:
        return _Hook7(a_this, a_spell, a_dualCast, a_effectStrength, a_reason, a_useBaseValueForCost);
    }

    auto actor = a_this->actor;

    if (!actor)
        return _Hook7(a_this, a_spell, a_dualCast, a_effectStrength, a_reason, a_useBaseValueForCost);

    if (Config::Settings::enable_diseases.GetValue())
    {
        if (Utility::ActiveEffectHasNewDiseaseKeyword(actor, Forms::FormConstants::silence_key))
        {
            if (a_spell && a_spell->GetFormType() != RE::FormType::AlchemyItem &&
                a_spell->GetFormType() != RE::FormType::Enchantment &&
                !Forms::FormLoader::spell_allow_list->HasForm(a_spell))
            {
                InterruptActor(actor, a_this->GetCastingSource());
                return false;
            }
        }
    }
    if (Config::Settings::enable_cast_stamina.GetValue())
    {
        float cost = 5;
        float type_factor = Config::Settings::magic_stamina_cost_divider.GetValue();

        if (a_spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration)
            type_factor *= 2.0f;

        cost = a_this->GetCurrentSpellCost() / type_factor;
        if (actor->GetActorValue(RE::ActorValue::kStamina) < cost)
        {
            return false;
        }
        if (a_this->state == RE::MagicCaster::State::kCharging)
        {
            if (actor == Cache::GetPlayerSingleton() && Cache::GetPlayerSingleton()->IsGodMode())
            {
                return _Hook7(a_this, a_spell, a_dualCast, a_effectStrength, a_reason, a_useBaseValueForCost);
            }
            actor->DamageActorValue(RE::ActorValue::kStamina, cost);
        }
    }

    return _Hook7(a_this, a_spell, a_dualCast, a_effectStrength, a_reason, a_useBaseValueForCost);
}

void PreventCast::InterruptActor(RE::Actor *a_actor, RE::MagicSystem::CastingSource a_castingSource)
{
    switch (a_castingSource)
    {
    case RE::MagicSystem::CastingSource::kLeftHand:
        RE::SourceActionMap::DoAction(a_actor, RE::DEFAULT_OBJECT::kActionLeftInterrupt);
        break;
    case RE::MagicSystem::CastingSource::kRightHand:
        RE::SourceActionMap::DoAction(a_actor, RE::DEFAULT_OBJECT::kActionRightInterrupt);
        break;
    case RE::MagicSystem::CastingSource::kOther:
        RE::SourceActionMap::DoAction(a_actor, RE::DEFAULT_OBJECT::kActionVoiceInterrupt);
        break;
    default:
        break;
    }
}

void PlayerPotionUsed::PlayerUsePotion(uint64_t self, RE::AlchemyItem *alch, uint64_t extralist)
{
    if (alch->HasKeywordString(Forms::FormConstants::cure_keyword))
    {
        RE::PlayerCharacter *player = Cache::GetPlayerSingleton();
        Utility::Curses::CleanseCurse(player);
    }
    return _Hook8(self, alch, extralist);
}

float HighGravityArrows::GetGravityArrow(RE::Projectile *a_this)
{
    if (!Config::Settings::enable_diseases.GetValue())
        return _Hook9(a_this);

    auto shooterRef = a_this->shooter.get().get();
    auto actorShooter = shooterRef ? shooterRef->As<RE::Actor>() : nullptr;
    if (actorShooter)
    {
        if (Utility::ActiveEffectHasNewDiseaseKeyword(actorShooter, Forms::FormConstants::bow_curse_key))
        {
            return _Hook9(a_this) * 10.0f;
        }
    }
    return _Hook9(a_this);
}

float StaminaAttackCost::GetAttackCost(RE::ActorValueOwner *a_owner, RE::BGSAttackData *attack)
{
    if (!Config::Settings::enable_attack_stamina.GetValue())
    {
        return _Hook12(a_owner, attack);
    }
    auto actor = skyrim_cast<RE::Actor *>(a_owner);
    if (attack->data.flags.any(RE::AttackData::AttackFlag::kBashAttack))
    {
        auto weapon = Utility::getWieldingWeapon(actor);
        auto leftH = Utility::GetWeapon(actor, true);
        auto shield = Utility::GetShield(actor);
        if (!leftH || !shield || leftH->IsHandToHandMelee() || weapon == Utility::GetUnarmedWeapon())
        {
            return 1.0f;
        }
        float weight = 1.0f;
        if (leftH)
            weight = leftH->GetWeight();
        if (shield)
            weight = shield->GetWeight();
        auto av = RE::ActorValue::kNone;

        if (leftH)
        {
            if (leftH->IsArmor())
            {
                if (leftH->As<RE::TESObjectARMO>()->IsLightArmor())
                {
                    av = RE::ActorValue::kLightArmor;
                }
                else if (leftH->As<RE::TESObjectARMO>()->IsHeavyArmor())
                {
                    av = RE::ActorValue::kHeavyArmor;
                }
            }
            if (leftH->IsWeapon())
            {
                av = leftH->As<RE::TESObjectWEAP>()->weaponData.skill.get();
            }
        }
        return GetWeightMult(actor, weight, av);
    }
    else
    {
        auto weap = Utility::getWieldingWeapon(actor);
        float weight = 1.0f;
        RE::ActorValue av = RE::ActorValue::kOneHanded;
        if (weap && !weap->IsHandToHandMelee() && weap != Utility::GetUnarmedWeapon())
        {
            av = weap->As<RE::TESObjectWEAP>()->weaponData.skill.get();
            weight = weap->GetWeight();
        }
        float ret = GetWeightMult(actor, weight, av);
        RE::BGSEntryPoint::HandleEntryPoint(RE::BGSPerkEntry::EntryPoint::kModPowerAttackStamina, actor, weap, &ret);
        if(actor->GetActorValue(RE::ActorValue::kStamina) < ret + 0.1 && !ActorUtil::HasEffectWithKeywordActive(actor,"StweaksExhaustion"))
			MagicUtil::ApplySpell(actor, actor, Forms::FormLoader::exhaustion_spell);
        return ret;
    }
}
float StaminaAttackCost::GetWeightMult(RE::Actor *actor, float weight, RE::ActorValue av_to_use)
{
    auto lvl = actor->GetActorValue(av_to_use);
    auto equip_weight = weight;

    if (equip_weight < 0.5f)
        equip_weight = 0.5f;

    float weightRatio = equip_weight / AVERAGE_WEAPON_WEIGHT;
    float weightMultiplier = std::pow(weightRatio, WEIGHT_SCALING);

    float skillFrac = std::clamp(lvl / 100.0f, 0.0f, 1.0f);
    float skillReduction = 0.6 * std::pow(skillFrac, SKILL_SCALING);
    float skillFactor = std::clamp(1.0f - skillReduction, 0.0f, 1.0f);

    float cost = Config::Settings::base_stamina_cost_attacks.GetValue() * weightMultiplier * skillFactor;

    cost = std::clamp(cost, MIN_COST, MAX_COST);
    return cost;
}

RE::NiAVObject *LoadWithResistance::LoadActor(RE::Actor *a_this, bool arg)
{

    auto actor = _Hook13(a_this, arg);
    if (Config::Settings::enable_mass_equip_changes.GetValue())
    {
        auto curr_mass = ActorUtil::GetMassFromInventory(a_this);
        a_this->SetActorValue(RE::ActorValue::kMass, a_this->GetBaseActorValue(RE::ActorValue::kMass));
        float modi = a_this->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass);
        a_this->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass, -modi);
        a_this->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass, curr_mass);
    }

    auto player = Cache::GetPlayerSingleton();
    if (a_this == player)
        return actor;

    else
    {
        auto player_level = player->GetLevel();
        if (Config::Settings::enable_resist_changes.GetValue())
        {
            if (player_level <= LEVEL_CAP && a_this->GetLevel() >= player_level &&
                a_this->GetActorValue(RE::ActorValue::kResistMagic) >= 0)
            {
                a_this->SetActorValue(RE::ActorValue::kResistMagic,
                                      a_this->GetActorValue(RE::ActorValue::kResistMagic) -
                                          Config::Settings::resist_reduction_value.GetValue());
            }
        }
        if (Config::Settings::level_up_low_levels.GetValue())
        {

            auto npc_level = a_this->GetLevel();
            if (npc_level + 10 < player_level)
            {
                ActorUtil::SetNPCLevel(a_this, player_level - 10);
            }
        }
    }
    return actor;
}
RE::NiAVObject *LoadWithResistance::LoadPlayer(RE::Actor *a_this, bool arg)
{
    auto actor = _Hook14(a_this, arg);
    if (Config::Settings::enable_mass_equip_changes.GetValue())
    {
        auto curr_mass = ActorUtil::GetMassFromInventory(a_this);
        a_this->SetActorValue(RE::ActorValue::kMass, a_this->GetBaseActorValue(RE::ActorValue::kMass));
        float modi = a_this->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass);
        a_this->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass, -modi);
        a_this->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass, curr_mass);
    }
    return actor;
}
float StaminaRegenAdjuster::GetStamBase(RE::Character *a_char, RE::ActorValue a_av)
{

    if (!a_char)
        return _Hook21(a_char, a_av);

    if (a_av == RE::ActorValue::kStamina && Config::Settings::stamina_regen_changes.GetValue())
    {
        float ret = 0.f;
        float base = Config::Settings::stamina_regen_base_calc.GetValue() / 100.f;
        float rate = a_char->GetActorValue(RE::ActorValue::kStaminaRate);
        float rateMult = a_char->GetActorValue(RE::ActorValue::kStaminaRateMult);
        float combatRate = a_char->IsInCombat() ? Utility::get_gmst("fCombatStaminaRegenRateMult")->GetFloat() : 1.0f;
        ret = base * rate * rateMult * combatRate * 0.01f;
        return ret;
    }

    if (a_av == RE::ActorValue::kMagicka && Config::Settings::magicka_regen_changes.GetValue())
    {
        float ret = 0.f;
        float base = Config::Settings::magicka_regen_base_calc.GetValue() / 100.f;
        float rate = a_char->GetActorValue(RE::ActorValue::kMagickaRate);
        float rateMult = a_char->GetActorValue(RE::ActorValue::kMagickaRateMult);
        float combatRate = a_char->IsInCombat() ? Utility::get_gmst("fCombatMagickaRegenRateMult")->GetFloat() : 1.0f;
        ret = base * rate * rateMult * combatRate * 0.01f;
        return ret;
    }

    return _Hook21(a_char, a_av);
}

void Hooks::SpellCap::ApplyPerkEntrySpellMag(RE::BGSPerkEntry::EntryPoint a_entry, RE::Actor *caster,
                                             RE::SpellItem *spell, RE::Actor *target, float &damage)
{
    float original_damage = damage;
    _Hook23(a_entry, caster, spell, target, damage);

    auto spell_type = spell->GetSpellType();

    switch (spell_type)
    {
        using st = RE::MagicSystem::SpellType;
    case st::kSpell:
    case st::kPower:
    case st::kVoicePower:
    case st::kLesserPower:
        break;
    default:
        return;
    }

    bool isHostile = spell->IsHostile();
    if (!isHostile)
        return;

    auto av_effect = spell->GetAVEffect();
    if (!av_effect)
        return;

    bool detri = av_effect->IsDetrimental();
    if (!detri)
        return;

    if (auto damage_ranges = Utility::GetRandomFloat(Utility::CalcPerc(Settings::magic_lower_range.GetValue(), false),
                                                     Utility::CalcPerc(Settings::magic_upper_range.GetValue(), true)))
    {
        damage *= damage_ranges;
    }

    bool isGoodAssassin = caster->IsSneaking() && target->RequestDetectionLevel(caster) <= 0;
    if (isGoodAssassin)
    {
        return;
    }

    auto health = ActorUtil::GetMaxHealth(target);
    auto curr_health = target->GetActorValue(RE::ActorValue::kHealth);

    auto own_level = target->GetLevel();
    auto aggressor_level = caster->GetLevel();

    if (aggressor_level <= own_level + 10)
    {
        if (Config::Settings::one_shot_protection.GetValue()) {
            if (damage > health && curr_health >= health * 0.99)
                damage = health * 0.75;
        }
        
        if (Config::Settings::enable_damage_caps.GetValue()) {
            float cap = original_damage * 5;
            if (damage > cap)
            {
                damage = cap;
            }
        }
        
    }
}
void DamageOut::ApplyPerkEntryAttack(RE::BGSPerkEntry::EntryPoint a_entry, RE::Actor *attacker,
                                     RE::TESObjectWEAP *weapon, RE::TESObjectREFR *target, float &damage)
{
    const float orig_dam{damage};

    _Hook24(a_entry, attacker, weapon, target, damage);

    if (!attacker)
        return;

    if (!target || target->IsDead())
        return;

    RE::Actor *const &actor = target->As<RE::Actor>();

    bool isGoodAssassin = attacker->IsSneaking() && actor ? actor->RequestDetectionLevel(attacker) <= 0 : false;

    if (weapon && !weapon->IsHandToHandMelee())
    {
    }

    RE::PlayerCharacter *player = Cache::GetPlayerSingleton();

    if (attacker == player)
    {
        if (Settings::enable_diseases.GetValue())
        {
            const char *curse_word = "curse_weapons";
            if (Utility::ActiveEffectHasNewDiseaseKeyword(attacker, curse_word))
            {
                float chance_for_effect = Utility::GetRandomFloat(0.0, 100.0);
                if (chance_for_effect < 10.0f)
                {
                    damage *= 0.0;
                    REX::DEBUG(" 10% weakness curse is active, you deal {} damage", damage);
                }
                else if (chance_for_effect < 60.0f)
                {
                    damage *= 0.5f;
                    REX::DEBUG("50% weakness curse is active, you deal {} damage", damage);
                }
            }
        }
    }

    if (Utility::ActorHasEffectWithArchetype(attacker, RE::EffectArchetypes::ArchetypeID::kEtherealize) &&
        Settings::enable_etheral_change.GetValue())
    {
        damage *= 0.0f;
        REX::DEBUG("{} is ethereal, damage is set to 0", attacker->GetName());
    }

    if (attacker && attacker->IsPlayerTeammate() && !attacker->IsCommandedActor() || attacker && attacker == player)
    {
        if (player->teammateCount > 0 && Settings::enable_foll_change.GetValue())
        {
            REX::DEBUG("melee damage active for actor: {}", actor->GetDisplayFullName());
            float adj_mod1 = 5.0f;
            float adj_divider = 20.0f;
            uint32_t foll_count = player->teammateCount;
            float speech_level = player->GetActorValue(RE::ActorValue::kSpeech);
            REX::DEBUG("pre calculation. foll count = {}, speech_level = {}", foll_count, speech_level);
            float modifier = std::clamp(1.0f - (foll_count) / (adj_mod1 + speech_level / adj_divider), 0.10f, 0.90f);

            if (modifier < 0.10f)
            {
                modifier = 0.10f;
            }
            if (modifier > 0.9f)
            {
                modifier = 0.9f;
            }
            damage *= modifier;
            REX::DEBUG("\n final modifier is {} \n", modifier);
        }
    }

    if (Config::Settings::enable_damage_ranges.GetValue())
    {
        float rand_mult = Utility::GetRandomFloat(Utility::CalcPerc(Settings::weapon_lower_range.GetValue(), false),
                                                  Utility::CalcPerc(Settings::weapon_upper_range.GetValue(), true));
        damage *= rand_mult;
    }

    if (Settings::enable_quest_item_nerf.GetValue())
    {
        if (ActorUtil::ActorHasQuestObjectInHand(attacker))
        {
            if (weapon && !Config::Exceptions::IsQuestWeaponException(weapon))
            {
                damage *= 0.005f;
            }
        }
    }

    if (actor)
    {
        auto max_health = ActorUtil::GetMaxHealth(actor);
        auto curr = actor->GetActorValue(RE::ActorValue::kHealth);

        auto own_level = actor->GetLevel();
        auto aggressor_level = attacker->GetLevel();
        
        bool one_shot_prot = Config::Settings::one_shot_protection.GetValue();

        if (aggressor_level <= own_level + 10)
        {
            if (curr >= max_health * 0.99f && one_shot_prot)
            {
                if (!isGoodAssassin)
                {
                    if (damage >= max_health)
                    {
                        damage = max_health * 0.75;
                    }
                }
                else if (weapon->IsRanged())
                {
                    if (damage >= max_health)
                    {
                        damage = max_health * 0.75;
                    }
                }
            }
        }
    }
    if (Config::Settings::attacks_of_opp.GetValue())
    {

        float opp_mod = GetOpportunityModifier(actor, attacker, false);
        damage *= opp_mod;
        bool isPl = actor && actor->IsPlayerRef();
        if (Config::Settings::show_opp_notif.GetValue() && opp_mod > 1.0f  && isPl)
        {
            const auto crit_message = std::format("Critical Strike for {:.1f} x Damage", opp_mod);
            RE::DebugNotification(crit_message.c_str(),
                                  Config::Settings::play_opp_sound.GetValue() ? "UISneakAttack" : nullptr);

            const RE::CriticalHit::Event event{attacker, ActorUtil::getWieldingWeapon(attacker), false};
            RE::CriticalHit::GetEventSource()->SendEvent(&event);
        }
    }

    auto power_attack = attacker ? attacker->IsPowerAttacking() : false;
    uint16_t dmg_cap = weapon && !weapon->IsHandToHandMelee() ? weapon->attackDamage * 5 : 200;
    bool cap_dmg = Config::Settings::enable_damage_caps.GetValue();
    if (cap_dmg) {
        if (power_attack)
        {
            dmg_cap *= 2;
        }
        if (damage > dmg_cap && !attacker->IsSneaking())
        {
            damage = dmg_cap;
        }
    }
    
}
float DamageOut::GetOpportunityModifier(RE::Actor *victim, RE::Actor *attacker, bool notification)
{
    float mod = 1.0f;
    if (!ActorUtil::IsInOpportunityState(victim, attacker))
        return mod;

    if (ActorUtil::ActorHasEffectOfTypeActive(victim, RE::EffectArchetypes::ArchetypeID::kParalysis) ||
        ActorUtil::ActorHasEffectOfTypeActive(victim, RE::EffectArchetypes::ArchetypeID::kCalm))
    {
        mod = OppModi::GetModifier(OppModi::OpportunityType::Paralysis);
        if (notification)
            RE::DebugNotification(std::format("attack of opportunity occured for {} times damage", mod).c_str());
    }
    else if (ActorUtil::IsPowerAttacking(victim) || victim->IsStaggering())
    {
        mod = OppModi::GetModifier(OppModi::OpportunityType::Attack);
        if (notification)
            RE::DebugNotification(std::format("attack of opportunity occured for {} times damage", mod).c_str());
    }
    else if (victim->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kIsSitting ||
             victim->actorState1.sitSleepState == RE::SIT_SLEEP_STATE::kIsSleeping)
    {
        mod = OppModi::GetModifier(OppModi::OpportunityType::Sleep);
        if (notification)
            RE::DebugNotification(std::format("attack of opportunity occured for {} times damage", mod).c_str());
    }
    else if (victim->GetHeadingAngle(attacker->GetPosition(), false) <= -135 ||
             victim->GetHeadingAngle(attacker->GetPosition(), false) >= 135)
    {
        mod = OppModi::GetModifier(OppModi::OpportunityType::Backstab);
        if (notification)
            RE::DebugNotification(std::format("attack of opportunity occured for {} times damage", mod).c_str());
    }
    return mod;
}
// https://www.nexusmods.com/skyrimspecialedition/mods/73514 partially taken
// from this mod
void CastingSpeed::CasterUpdate(RE::ActorMagicCaster *a_this, float a_delta)
{
    auto state = a_this->state.any(RE::ActorMagicCaster::State::kUnk01) ||
                 a_this->state.any(RE::ActorMagicCaster::State::kUnk02);
    if (state && Config::Settings::enable_skill_based_cast_speed.GetValue())
    {
        if (const auto actor = a_this->GetCasterAsActor();
            actor && a_this->currentSpell && a_this->currentSpell->As<RE::SpellItem>())
        {

            auto spell = a_this->currentSpell->As<RE::SpellItem>();
            if (MagicUtil::IsSpellPlayable(spell) && !MagicUtil::IsPermanent(spell)) {
                if (spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration || spell->GetSpellType() == RE::MagicSystem::SpellType::kVoicePower)
                {
                    return _Hook25(a_this, a_delta);
                }
                float time_origin = a_this->currentSpell->GetChargeTime();
                float new_time = GetCastingSpeedMult(actor->GetActorValue(spell->GetAssociatedSkill()));
                return _Hook25(a_this, a_delta * new_time);
            }
            return _Hook25(a_this, a_delta);
            

           
        }
    }
    return _Hook25(a_this, a_delta);
}
void EquipHandler::OnItemEquipped(RE::Actor *a_this, bool a_playAnim)
{

    _Hook26(a_this, a_playAnim);
    if (Config::Settings::enable_mass_equip_changes.GetValue())
    {
        auto curr_mass = ActorUtil::GetMassFromInventory(a_this);
        a_this->SetActorValue(RE::ActorValue::kMass, a_this->GetBaseActorValue(RE::ActorValue::kMass));
        float modi = a_this->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass);
        a_this->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass, -modi);
        a_this->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass, curr_mass);
    }
    else
    {
        a_this->SetActorValue(RE::ActorValue::kMass, a_this->GetBaseActorValue(RE::ActorValue::kMass));
    }
}
void EquipHandler::OnItemEquippedPlayer(RE::PlayerCharacter *a_this, bool a_playAnim)
{
    _Hook26(a_this, a_playAnim);
    if (Config::Settings::enable_mass_equip_changes.GetValue())
    {
        auto curr_mass = ActorUtil::GetMassFromInventory(a_this);
        a_this->SetActorValue(RE::ActorValue::kMass, a_this->GetBaseActorValue(RE::ActorValue::kMass));
        float modi = a_this->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass);
        a_this->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass, -modi);
        a_this->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass, curr_mass);
    }
    else
    {
        a_this->SetActorValue(RE::ActorValue::kMass, a_this->GetBaseActorValue(RE::ActorValue::kMass));
    }
}

static void ArgumentDump(RE::Actor *a_this, RE::Actor *target, std::int32_t &score, bool &spotted, bool &hasLOS,
                         std::int32_t &reason, std::int32_t &soundLvl, float &unk8, float &unk9)
{
    auto this_name = a_this ? a_this->GetName() : "null";
    auto targ_name = target->GetName();
    REX::INFO("argument dump: a_this: {}, target: {}, "
              "score: {}, spotted: {}, hasLOS: {}, reason: "
              "{}, soundLvl: {}, unk8: {}, unk9: {}",
              this_name, targ_name, score, spotted, hasLOS, reason, soundLvl, unk8, unk9);
};

void Detection::DoCalculateDetection(RE::Actor *a_this, RE::Actor *target, std::int32_t &score, bool &spotted,
                                     bool &hasLOS, std::int32_t &reason, RE::NiPoint3 &lastPos, std::int32_t &soundLvl,
                                     float &unk8, float &unk9)
{
    _Hook28(a_this, target, score, spotted, hasLOS, reason, lastPos, soundLvl, unk8, unk9);
    RE::BGSPerk *perk = Forms::FormLoader::tall_grass_perk;
    bool hasPerk = target->HasPerk(perk);
    bool isEnabled = Config::Settings::tall_grass_sneak.GetValue();
    if (target->IsSneaking() && IsStandingInTallGrass(target) && isEnabled)
    {
        if (!hasPerk)
        {
            target->AddPerk(perk);
            REX::INFO("added {} to {}", perk->GetName(), target->GetName());
        }
        return;
    }
    if (hasPerk)
        target->RemovePerk(perk);
}

inline bool Detection::IsStandingInTallGrass(RE::Actor *target)
{
    if (!target || !target->IsPlayerRef())
        return false;
    const auto cell = target->GetParentCell();
    if (!cell)
        return false;
    if (!cell->IsExteriorCell())
        return false;
    const auto tes = RE::TES::GetSingleton();
    if (!tes)
        return false;
    auto landscapeTexture = tes->GetLandTexture(target->GetPosition());
    if (!landscapeTexture)
        return false;
    // const auto edid = editorID::get_editorID(landscapeTexture);
    //  REX::INFO("current landscape texture is: {}", edid);
    if (Config::Exceptions::IsTallGrass(landscapeTexture))
    {
        return true;
    }
    return false;
}

inline bool IsSleepRelated(RE::SIT_SLEEP_STATE state)
{
    switch (state)
    {
    case RE::SIT_SLEEP_STATE::kWantToSleep:
    case RE::SIT_SLEEP_STATE::kIsSleeping:
    case RE::SIT_SLEEP_STATE::kWaitingForSleepAnim:
    case RE::SIT_SLEEP_STATE::kWantToWake:
    case RE::SIT_SLEEP_STATE::kWantToStand:
        return true;
    default:
        return false;
    }
}

} // namespace Hooks
