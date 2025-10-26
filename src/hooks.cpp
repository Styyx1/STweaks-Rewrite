#include "hooks.h"

namespace Hooks {

	using namespace Config;

	void Install()
	{
		CombatHit::InstallH0();
		AdjustActiveEffect::InstallH2();
		MainUpdate::InstallH1();
		JumpHeight::InstallH3();
		DealtMeleeDamage::InstallH4();
		NPCFade::InstallH6();
		PreventCast::InstallH7();
		PlayerPotionUsed::InstallH8();
		HighGravityArrows::InstallH9();
        StaminaAttackCost::InstallH12();
        LoadWithResistance::InstallH13();

	}

    void CombatHit::InstallH0()
    {
        if (!Config::Settings::enable_damage_ranges.GetValue()) {
            _Hook0.Disable();
        }
    }

    void AdjustActiveEffect::InstallH2() 
    {
        if (!Config::Settings::enable_damage_ranges.GetValue()) {
            _Hook2.Disable();
        }
    }

    void MainUpdate::InstallH1()
    {
        if (!Settings::enable_sneak_stamina.GetValue()) {
            _Hook1.Disable();
        }
    }

    void JumpHeight::InstallH3()
    {
        if (!Settings::enable_mass_based_jump_height.GetValue() && !Settings::enable_sneak_jump_limit.GetValue()) {
            _Hook3.Disable();
        }
    }

    void DealtMeleeDamage::InstallH4()
    {
        if (!Settings::enable_foll_change.GetValue() && !Settings::enable_diseases.GetValue()) {
            _Hook4.Disable();
        }
    }

    void NPCFade::InstallH6()
    {
        if (!Settings::enable_fading_actors.GetValue()) {
            _Hook6.Disable();
        }
    }

    void PreventCast::InstallH7()
    {
        if (!Settings::enable_diseases.GetValue() && !Settings::enable_cast_stamina.GetValue() && !Settings::interupt_cast_on_hit.GetValue()) {
            _Hook7.Disable();
        }
    }

    void PlayerPotionUsed::InstallH8()
    {
        if (!Settings::enable_diseases.GetValue()) {
            _Hook8.Disable();
        }
    }

    void HighGravityArrows::InstallH9()
    {
        if (!Settings::enable_diseases.GetValue()) {
            _Hook9.Disable();
        }
    }

    void StaminaAttackCost::InstallH12()
    {
        if (!Settings::enable_attack_stamina.GetValue()) {
            _Hook12.Disable();
        }
    }

    void LoadWithResistance::InstallH13()
    {
        if (!Settings::enable_resist_changes.GetValue()) {
            _Hook13.Disable();
        }
    }


    bool wasEnraged = false;
    float CombatHit::WeaponTypeModifier(RE::TESObjectWEAP* a_weap, float f_in)
    {
        float f_out = f_in;

        if (a_weap->IsOneHandedDagger())
            f_out *= 2.0f;

        return f_out;
    }

    void CombatHit::RandomiseDamage(RE::Actor* a_this, RE::HitData* a_hitData)
    {
        float remaining = a_hitData->totalDamage;

        if (a_hitData->weapon && !a_hitData->weapon->IsHandToHandMelee())
        {
            REX::DEBUG("----------------------------------------------------");
            REX::DEBUG("started hooked damage calc for {} you got hit by: {}", a_this->GetName(), a_hitData->aggressor.get().get()->GetDisplayFullName());
            float rand_mult = Utility::GetRandomFloat(Utility::CalcPerc(Settings::weapon_lower_range.GetValue(), false), Utility::CalcPerc(Settings::weapon_upper_range.GetValue(), true));
            REX::DEBUG("multiplier is {}", rand_mult);
            remaining *= rand_mult;
            REX::DEBUG("new damage for melee is {}. Original damage was: {} \n", remaining, a_hitData->totalDamage);
            a_hitData->totalDamage = remaining;
        }
    }

    void CombatHit::CHit(RE::Actor* a_this, RE::HitData* a_hitData)
    {
        RandomiseDamage(a_this, a_hitData);
        _Hook0(a_this, a_hitData);
    }

    void MainUpdate::PlayerUpdate(RE::PlayerCharacter* player, float a_delta)
    {
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
                    if (player->IsSneaking() && Utility::IsMoving(player) && Settings::enable_sneak_stamina.GetValue() || player->IsSneaking() && HasRangedWeaponDrawn(player) && Settings::enable_sneak_stamina.GetValue())
                    {
                        REX::INFO("Player is moving or has bow out");
                        if (!Utility::HasSpell(player, Forms::FormLoader::sneak_stamina_spell))
                        {
                            REX::INFO("try to add Spell to Player");
                            player->AddSpell(Forms::FormLoader::sneak_stamina_spell);
                            //Utility::ApplySpell(player, player, Forms::FormConstants::sneak_stamina_spell);
                            REX::INFO("Added Spell to Player");
                        }
                        if (player->GetActorValue(RE::ActorValue::kStamina) <= 5 && player->GetActorValue(RE::ActorValue::kStamina) > 0)
                        {
                            player->actorState2.weaponState = RE::WEAPON_STATE::kWantToSheathe;
                            player->DrawWeaponMagicHands(false);
                        }
                    }
                    else if (Utility::HasSpell(player, Forms::FormLoader::sneak_stamina_spell))
                    {
                        player->RemoveSpell(Forms::FormLoader::sneak_stamina_spell);
                        REX::INFO("Removed stamina spell from player");
                    }

                    break;
                default:
                    break;
                }
            }
        }
        frameCount++;
        return _Hook1(player, a_delta);
    }

    bool MainUpdate::HasRangedWeaponDrawn(RE::PlayerCharacter* player)
    {
        bool result = false;
        RE::TESObjectWEAP* weap = Utility::getWieldingWeapon(player);
        if (weap)
        {
            if (weap->IsBow() && player->IsWeaponDrawn() || weap->IsCrossbow() && player->IsWeaponDrawn())
                result = true;
        }

        REX::DEBUG("check for ranged weapon, it is {}", result ? "true" : "false");
        return result;
    }

    void AdjustActiveEffect::thunk(RE::ActiveEffect* a_this, float a_power, bool a_onlyHostile)
    {
        const auto attacker = a_this->GetCasterActor();
        const auto target = a_this->GetTargetActor();
        const auto effect = a_this->GetBaseObject();
        const auto spell = a_this->spell;

        if (attacker && target && spell && effect && effect->IsHostile())
        {
            if (const auto projectile = effect->data.projectileBase; projectile)
            {
                if (auto damage_ranges = Utility::GetRandomFloat(Utility::CalcPerc(Settings::magic_lower_range.GetValue(), false), Utility::CalcPerc(Settings::magic_upper_range.GetValue(), true)))
                {
                    a_this->magnitude *= damage_ranges;
                }
            }
        }
        _Hook2(a_this, a_power, a_onlyHostile);
    }

    float JumpHeight::JumpHeightGetScale(RE::TESObjectREFR* refr)
    {
        float scale = refr->GetScale();
        RE::Actor* actor = refr->As<RE::Actor>();

        if (!actor)
        {
            return scale;
        }
        float mass = actor->GetActorValue(RE::ActorValue::kMass);

        if (actor->IsSneaking())
        {
            scale *= Settings::sneak_height_modifier.GetValue();
        }
        float ju_modifier = (float)sqrt(1.0 / mass);

        float curse_modi = 1.0f;
        if (Utility::ActiveEffectHasNewDiseaseKeyword(actor, Forms::FormConstants::jump_curse_key)) {
            curse_modi = 0.5f;
        }

        return scale *= ju_modifier * curse_modi;
    }

    float DealtMeleeDamage::MeleeDamage(RE::TESObjectWEAP* _weap, RE::ActorValueOwner* a, float DamageMult, char isbow)
    {
        RE::PlayerCharacter* player = Cache::GetPlayerSingleton();
        RE::Actor* actor = skyrim_cast<RE::Actor*>(a);
        float dam = DamageMult;
        REX::DEBUG("damage mult argument is {}", DamageMult);

        if (actor == player)
        {
            if (Settings::enable_diseases.GetValue()) {
                const char* curse_word = "curse_weapons";
                if (Utility::ActiveEffectHasNewDiseaseKeyword(actor, curse_word)) {
                    float chance_for_effect = Utility::GetRandomFloat(0.0, 100.0);
                    if (chance_for_effect < 10.0f) {
                        dam *= 0.0;
                        REX::DEBUG(" 10% weakness curse is active, you deal {} damage", dam);
                    }
                    else if (chance_for_effect < 60.0f) {
                        dam *= 0.5f;
                        REX::DEBUG("50% weakness curse is active, you deal {} damage", dam);
                    }
                }
            }
            if (Settings::enable_quest_item_nerf.GetValue()) {
                if (ActorHasQuestObjectInHand(player)) {
                    if (_weap && !Config::Exceptions::IsQuestWeaponException(_weap)) {
                        dam *= 0.005f;
                    }
                }
            }
        }

        if (Utility::ActorHasEffectWithArchetype(actor, RE::EffectArchetypes::ArchetypeID::kEtherealize) && Settings::enable_etheral_change.GetValue())
        {
            dam *= 0.0f;
            REX::DEBUG("[{}] is ethereal, damage is set to 0", actor->GetName());
        }

        if (actor && actor->IsPlayerTeammate() && !actor->IsCommandedActor() || actor && actor == player)
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
                dam *= modifier;
                REX::DEBUG("\n final modifier is {} \n", modifier);
            }
        }
        return _Hook4(_weap, a, dam, isbow);
    }

    bool DealtMeleeDamage::ActorHasQuestObjectInHand(RE::Actor* actor) {
        if (actor) {
            auto* rightHandItem = actor->GetEquippedEntryData(false);
            if (rightHandItem) {
                if (rightHandItem->IsQuestObject()) {
                    return true;
                }
            }

            auto* leftHandItem = actor->GetEquippedEntryData(true);
            if (leftHandItem) {
                if (leftHandItem->IsQuestObject()) {
                    return true;
                }
            }
        }

        return false;
    }

    void OnEffectEndHook::OnEffectEnd(RE::ScriptEffect* a_this)
    {
        _Hook5(a_this);
        auto hitEv = Events::HitEventHandler::GetSingleton();

        if (a_this->target && a_this->GetBaseObject()->HasAnyKeywordByEditorID(Forms::FormConstants::diseases))
        {
            REX::DEBUG("curse ended");
            RE::Actor* aff_actor = skyrim_cast<RE::Actor*>(a_this->target);
            if (a_this->GetBaseObject()->HasKeywordString(Forms::FormConstants::diseases[0]))
            {
                float currentPenaltyH = hitEv->storedHealth_disease;
                if (currentPenaltyH > 0)
                {
                    hitEv->storedHealth_disease = 0.0f;
                    aff_actor->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kHealth, currentPenaltyH);
                }
            }
            if (a_this->GetBaseObject()->HasKeywordString(Forms::FormConstants::diseases[1]))
            {
                float currentPenaltyS = hitEv->storedStamina_disease;
                if (currentPenaltyS > 0)
                {
                    hitEv->storedStamina_disease = 0.0f;
                    aff_actor->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kStamina, currentPenaltyS);
                }
            }
            if (a_this->GetBaseObject()->HasKeywordString(Forms::FormConstants::diseases[2]))
            {
                float currentPenaltyM = hitEv->storedMagicka_disease;
                if (currentPenaltyM > 0)
                {
                    hitEv->storedMagicka_disease = 0.0f;
                    aff_actor->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMagicka, currentPenaltyM);
                }
            }
        }
    }

    void NPCFade::ActorUpdate(RE::Character* a_actor, float a_delta)
    {
        RE::PlayerCharacter* player = Cache::GetPlayerSingleton();
        float distance_full_fade = 2400.00;
        float distance_no_fade = 1200.00f;
        float light_level_threshold = 30.0f;
        float fade_range = distance_full_fade - distance_no_fade;
        float minAlpha = 0.05f;

        auto difficulty_level = player->difficulty;
        float difficultyMult = static_cast<float>(difficulty_level) / 5.0f;  // 0.0 - 1.0

        if (float distance = a_actor->GetDistance(player); distance >= distance_full_fade && a_actor->GetHighProcess() && a_actor->GetHighProcess()->lightLevel <= light_level_threshold)
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

    bool PreventCast::CheckCast(RE::ActorMagicCaster* a_this, RE::MagicItem* a_spell, bool a_dualCast, float* a_effectStrength, RE::MagicSystem::CannotCastReason* a_reason, bool a_useBaseValueForCost)
    {
        auto actor = a_this->actor;
        const char* curse_word = "curse_silence";
        if (Utility::ActiveEffectHasNewDiseaseKeyword(actor, Forms::FormConstants::silence_key)) {
            if (a_spell && a_spell->GetFormType() != RE::FormType::AlchemyItem && a_spell->GetFormType() != RE::FormType::Enchantment && !Forms::FormLoader::spell_allow_list->HasForm(a_spell)) {
                InterruptActor(actor, a_this->GetCastingSource());
                return false;
            }
        }
        if (Config::Settings::enable_cast_stamina.GetValue()) {
            float cost = 5;
            float type_factor = Config::Settings::magic_stamina_cost_divider.GetValue();

            if (a_spell && a_spell->GetCastingType() == RE::MagicSystem::CastingType::kConcentration)
                type_factor *= 2.0f;

            cost = a_this->GetCurrentSpellCost() / type_factor;
            if (actor->GetActorValue(RE::ActorValue::kStamina) < cost) {
                return false;
            }
            if (a_this->state == RE::MagicCaster::State::kCharging) {
                if (actor == Cache::GetPlayerSingleton() && Cache::GetPlayerSingleton()->IsGodMode()) {
                    return _Hook7(a_this, a_spell, a_dualCast, a_effectStrength, a_reason, a_useBaseValueForCost);
                }
                actor->DamageActorValue(RE::ActorValue::kStamina, cost);
            }   
        }

        return _Hook7(a_this, a_spell, a_dualCast, a_effectStrength, a_reason, a_useBaseValueForCost);
    }

    void PreventCast::InterruptActor(RE::Actor* a_actor, RE::MagicSystem::CastingSource a_castingSource)
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

    void PlayerPotionUsed::PlayerUsePotion(uint64_t self, RE::AlchemyItem* alch, uint64_t extralist)
    {
        if (alch->HasKeywordString(Forms::FormConstants::cure_keyword))
        {
            RE::PlayerCharacter* player = Cache::GetPlayerSingleton();
            Utility::Curses::CleanseCurse(player);
        }
        return _Hook8(self, alch, extralist);
    }

    float HighGravityArrows::GetGravityArrow(RE::Projectile* a_this)
    {
        auto shooterRef = a_this->shooter.get().get();
        auto actorShooter = shooterRef ? shooterRef->As<RE::Actor>() : nullptr;
        if (actorShooter) {
            if (Utility::ActiveEffectHasNewDiseaseKeyword(actorShooter, Forms::FormConstants::bow_curse_key)) {
                return _Hook9(a_this) * 10.0f;
            }
        }
        return _Hook9(a_this);
    }

    

    float StaminaAttackCost::GetAttackCost(RE::ActorValueOwner* a_owner, RE::BGSAttackData* attack)
    {
        auto actor = skyrim_cast<RE::Actor*>(a_owner);
        if (attack->data.flags.any(RE::AttackData::AttackFlag::kBashAttack)) {
            auto weapon = Utility::getWieldingWeapon(actor);
            auto leftH = Utility::GetWeapon(actor, true);
            auto shield = Utility::GetShield(actor);
            if (!leftH || !shield || leftH->IsHandToHandMelee() || weapon == Utility::GetUnarmedWeapon()) {
                return 1.0f;
            }
            float weight = 1.0f;
            if(leftH)
                weight = leftH->GetWeight();
            if (shield)
                weight = shield->GetWeight();
            auto av = RE::ActorValue::kNone;

            if (leftH) {
                if (leftH->IsArmor()) {
                    if (leftH->As<RE::TESObjectARMO>()->IsLightArmor()) 
                    {
                        av = RE::ActorValue::kLightArmor;
                    }
                    else if (leftH->As<RE::TESObjectARMO>()->IsHeavyArmor())
                    {
                        av = RE::ActorValue::kHeavyArmor;
                    }
                }
                if (leftH->IsWeapon()) {
                    av = leftH->As<RE::TESObjectWEAP>()->weaponData.skill.get();
                }
            }
                
            return GetWeightMult(actor, weight, av);
        }
        else {
            auto weap = Utility::getWieldingWeapon(actor);
            float weight = 1.0f;
            RE::ActorValue av = RE::ActorValue::kOneHanded;
            if (weap && !weap->IsHandToHandMelee() && weap != Utility::GetUnarmedWeapon()) {
                av = weap->As<RE::TESObjectWEAP>()->weaponData.skill.get();
                weight = weap->GetWeight();
            }
                
            REX::INFO("attacker {} has weapon weight of {} and uses av {}", actor->GetName(), weight, RE::ActorValueToString(av));

            return GetWeightMult(actor, weight, av);
        }
    }



    float StaminaAttackCost::GetWeightMult(RE::Actor* actor, float weight, RE::ActorValue av_to_use)
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

    RE::NiAVObject* LoadWithResistance::LoadActor(RE::Actor* a_this, bool arg)
    {
        auto actor = _Hook13(a_this, arg);
        auto player = Cache::GetPlayerSingleton();
        if (a_this == player)
            return actor;
        else {
            auto player_level = player->GetLevel();
            if (player_level <= LEVEL_CAP && a_this->GetLevel() >= player_level && a_this->GetActorValue(RE::ActorValue::kResistMagic) >= 0) {
                a_this->SetActorValue(RE::ActorValue::kResistMagic, a_this->GetActorValue(RE::ActorValue::kResistMagic) - Config::Settings::resist_reduction_value.GetValue());
            }
            
        }
        return actor;
    }

}

