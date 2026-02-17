#pragma once
#include "cache.h"
#include "Settings.h"
#include "formloader.h"

class Utility
{
public:

    enum class GameDifficulty : std::int32_t {
        Novice = 0,
        Apprentice = 1,
        Adept = 2,
        Expert = 3,
        Master = 4,
        Legendary = 5
    };

    static float CalcPerc(int a_input, bool a_high)
    {
        float result;
        if (a_high)
        {
            return result = (100.00f + a_input) / 100.00f;
        }
        else
            return result = (100.00f - a_input) / 100.00f;
    }

    static RE::Setting* get_gmst(const char* a_setting)
    {
        return RE::GameSettingCollection::GetSingleton()->GetSetting(a_setting);
    }

    static void AddItem(RE::Actor* a, RE::TESBoundObject* item, RE::ExtraDataList* extraList, int count, RE::TESObjectREFR* fromRefr)
    {
        using func_t = decltype(AddItem);
        REL::Relocation<func_t> func{ REL::ID(37525) };
        return func(a, item, extraList, count, fromRefr);
    }

    static void AddItemPlayer(RE::TESBoundObject* item, int count)
    {
        return AddItem(RE::PlayerCharacter::GetSingleton(), item, nullptr, count, nullptr);
    }

    static int RemoveItemPlayer(RE::TESBoundObject* item, int count)
    {
        using func_t = decltype(RemoveItemPlayer);
        REL::Relocation<func_t> func{ REL::ID(16919) };
        return func(item, count);
    }

    static int get_item_count(RE::Actor* a, RE::TESBoundObject* item)
    {
        if (auto changes = a->GetInventoryChanges())
        {
            using func_t = int(RE::InventoryChanges*, RE::TESBoundObject*);
            REL::Relocation<func_t> func{ REL::ID(16047) };
            return func(changes, item);
        }
        return 0;
    }

    static void LogBool(bool bLog)
    {
		REX::DEBUG("bool is set to {}", bLog ? "true" : "false");
    }
    static void LogItemCountMiscItem(RE::TESObjectMISC* item, int count)
    {
        REX::DEBUG("player has {} of {} in the inventory", count, item->GetName());
    }
    static void LogGlobal(RE::TESGlobal* global)
    {
        REX::DEBUG("lookup successfull global {} with the value {} found", global->GetFormEditorID(), global->value);
    }

    // Credit: D7ry for getWieldingWeapon in ValhallaCombat
    // https://github.com/D7ry/valhallaCombat/blob/48fb4c3b9bb6bbaa691ce41dbd33f096b74c07e3/src/include/Utils.cpp#L10
    static RE::TESObjectWEAP* getWieldingWeapon(RE::Actor* a_actor)
    {
        auto weapon = a_actor->GetAttackingWeapon();
        if (weapon)
        {
            return weapon->object->As<RE::TESObjectWEAP>();
        }
        auto rhs = a_actor->GetEquippedObject(false);
        if (rhs && rhs->IsWeapon())
        {
            return rhs->As<RE::TESObjectWEAP>();
        }
        auto lhs = a_actor->GetEquippedObject(true);
        if (lhs && lhs->IsWeapon())
        {
            return lhs->As<RE::TESObjectWEAP>();
        }

        return nullptr;
    }

    static float GetRandomFloat(float a_min, float a_max)
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_real_distribution<float> distrib(a_min, a_max);
        return std::roundf((distrib(gen) * 100)) / 100;
    }

    static bool IsDualWielding(RE::Actor* a_actor)
    {
        auto weapon = a_actor->GetAttackingWeapon();
        auto rhs = a_actor->GetEquippedObject(false);
        auto lhs = a_actor->GetEquippedObject(true);
        if (weapon && rhs && lhs && lhs->IsWeapon() && rhs->IsWeapon())
        {
            return true;
        }
        else
            return false;
    }

    static float GetMassFromInventory(RE::Actor* actor)
    {
        //https://www.desmos.com/calculator/qxttj42mfl?lang=en
        //this helped me come up with the calcs
        if (!actor)
            return 1.0f;
        float weight = actor->GetEquippedWeight();
        constexpr float kMinWeight = 0.0f;
        float kMaxBaseMass = MiscUtil::GetGameSetting("fStaggerMassOffsetBase")->GetFloat();
        constexpr float kBaseMass = 1.0f;

        constexpr float kGrowth = 0.03f; // 0.01 is too low and 0.05 is too high

        weight = std::max(weight, kMinWeight);

        float mass = kMaxBaseMass - (kMaxBaseMass - kBaseMass) * std::exp(-kGrowth * weight);

        return std::clamp(mass, kBaseMass, kMaxBaseMass);
    }

    static void AdjustMass(RE::Actor* a_actor) {
        auto curr_mass = GetMassFromInventory(a_actor);
        auto base_mass = a_actor->GetBaseActorValue(RE::ActorValue::kMass);
        if(curr_mass < base_mass) {
            curr_mass = base_mass;
		}       
        a_actor->SetActorValue(RE::ActorValue::kMass, base_mass);
        float modi = a_actor->GetActorValueModifier(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass) - base_mass;
        a_actor->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass, -(modi + base_mass));
        a_actor->ModActorValue(RE::ACTOR_VALUE_MODIFIER::kPermanent, RE::ActorValue::kMass, curr_mass - base_mass);
    }

    // https://github.com/powerof3/PapyrusExtenderSSE/blob/640b79d554da4a5392a05107560685621825568e/include/Papyrus/Functions/ObjectReference.h#L662
    static std::vector<RE::Actor*> GetNearbyActors(RE::TESObjectREFR* a_ref, float a_radius, bool a_ignorePlayer)
    {
        {
            std::vector<RE::Actor*> result;
            if (const auto processLists = RE::ProcessLists::GetSingleton(); processLists)
            {
                if (a_ignorePlayer && processLists->numberHighActors == 0)
                {
                    REX::DEBUG("no process list");
                    return result;
                }

                const auto squaredRadius = a_radius * a_radius;
                const auto originPos = a_ref->GetPosition();

                result.reserve(processLists->numberHighActors);

                const auto get_actor_within_radius = [&](RE::Actor* a_actor)
                    {
                        if (a_actor && a_actor != a_ref && originPos.GetSquaredDistance(a_actor->GetPosition()) <= squaredRadius)
                        {
                            result.emplace_back(a_actor);
                        }
                    };
                for (auto& actorHandle : processLists->highActorHandles)
                {
                    const auto actor = actorHandle.get();
                    get_actor_within_radius(actor.get());
                }

                if (!a_ignorePlayer)
                {
                    get_actor_within_radius(RE::PlayerCharacter::GetSingleton());
                }

                if (!result.empty())
                {
                    REX::DEBUG("vector is not empty");
                    return result;
                }
            }
            return result;
        }
    }

    static bool ActorHasActiveEffect(RE::Actor* a_actor, RE::EffectSetting* a_effect)
    {
        auto activeEffects = a_actor->GetActiveEffectList();
        RE::EffectSetting* setting = nullptr;
        if (!activeEffects->empty())
        {
            for (RE::ActiveEffect* effect : *activeEffects)
            {
                if (effect; !effect->flags.any(RE::ActiveEffect::Flag::kInactive))
                {
                    setting = effect ? effect->GetBaseObject() : nullptr;
                    if (setting)
                    {
                        if (setting == a_effect)
                        {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    static bool ActiveEffectHasNewDiseaseKeyword(RE::Actor* a_actor, std::string a_keyword)
    {
        if (!a_actor || a_actor->IsDead() || !a_actor->Is3DLoaded())
            return false;
        auto activeEffects = a_actor->GetActiveEffectList();
        RE::EffectSetting* setting = nullptr;
        if (!activeEffects->empty())
        {
            for (RE::ActiveEffect* effect : *activeEffects)
            {
                if (effect; !effect->flags.any(RE::ActiveEffect::Flag::kInactive))
                {
                    setting = effect ? effect->GetBaseObject() : nullptr;
                    if (setting)
                    {
                        if (setting->HasKeywordString(a_keyword))
                        {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    static bool IsQuestItem(const RE::TESObjectREFR* a_ref)
    {
        if (const auto xAliases = a_ref->extraList.GetByType<RE::ExtraAliasInstanceArray>(); xAliases) {
            RE::BSReadLockGuard locker(xAliases->lock);

            return std::ranges::any_of(xAliases->aliases, [](const auto& aliasData) {
                const auto alias = aliasData ? aliasData->alias : nullptr;
                return alias && alias->IsQuestObject();
                });
        }

        return a_ref->HasQuestObject();
    }

    static bool ActorHasEffectWithArchetype(RE::Actor* a_actor, RE::EffectArchetype a_archetype)
    {
        auto activeEffects = a_actor->GetActiveEffectList();
        RE::EffectSetting* setting = nullptr;
        if (!activeEffects->empty())
        {
            for (RE::ActiveEffect* effect : *activeEffects)
            {
                if (effect; !effect->flags.any(RE::ActiveEffect::Flag::kInactive))
                {
                    setting = effect ? effect->GetBaseObject() : nullptr;
                    if (setting)
                    {
                        if (setting->GetArchetype() == a_archetype)
                        {
                            return true;
                        }
                    }
                }
            }
        }
        return false;
    }

    // Credit: KernalsEgg for ApplySpell and IsPermanent

    static bool IsPermanent(RE::MagicItem* item)
    {
        switch (item->GetSpellType())
        {
        case RE::MagicSystem::SpellType::kDisease:
        case RE::MagicSystem::SpellType::kAbility:
        case RE::MagicSystem::SpellType::kAddiction:
        {
            return true;
        }
        default:
        {
            return false;
        }
        }
    }

    static void ApplySpell(RE::Actor* caster, RE::Actor* target, RE::SpellItem* spell)
    {
        if (IsPermanent(spell))
        {
            target->AddSpell(spell);
        }
        else
        {
            caster->GetMagicCaster(RE::MagicSystem::CastingSource::kInstant)->CastSpellImmediate(spell, false, target, 1.0F, false, 0.0F, nullptr);
        }
    }

    // https://github.com/colinswrath/BladeAndBlunt/blob/main/include/Conditions.h
    static bool HasSpell(RE::Actor* actor, RE::SpellItem* spell)
    {
        using func_t = decltype(&Utility::HasSpell);

        REL::Relocation<func_t> func{ Cache::HasSpellAddress };

        return func(actor, spell);
    }
    inline static REL::Relocation<decltype(HasSpell)> _HasSpell;

    static bool IsMoving(RE::Actor* a_actor)
    {
        return (static_cast<bool>(a_actor->actorState1.movingForward) || static_cast<bool>(a_actor->actorState1.movingBack) || static_cast<bool>(a_actor->actorState1.movingLeft) || static_cast<bool>(a_actor->actorState1.movingRight));
    }

    static bool IsAttacking(RE::Actor* actor)
    {
        using func_t = decltype(&IsAttacking);
        REL::Relocation<func_t> func{ Cache::IsAttackingAddress };
        return func(actor);
    }

    inline static REL::Relocation<decltype(IsAttacking)> _IsAttacking;

    static bool IsBlocking(RE::Actor* actor)
    {
        using func_t = decltype(&IsBlocking);
        REL::Relocation<func_t> func{ Cache::IsBlockingAddress };
        return func(actor);
    }
    inline static REL::Relocation<decltype(IsBlocking)> _IsBlocking;

    static bool GetMount(RE::Actor* a_actor, RE::ActorPtr* a_mountOut)
    {
        using func_t = decltype(&GetMount);
        REL::Relocation<func_t> func{ REL::ID(38702) };
        return func(a_actor, a_mountOut);
    }



    static inline bool IsPowerAttacking(RE::Actor* actor)
    {
        if (auto high = actor->GetHighProcess())
        {
            if (const auto& attackData = high->attackData)
            {
                auto flags = attackData->data.flags;

                if (flags && flags.any(RE::AttackData::AttackFlag::kPowerAttack))
                {
                    return true;
                }
            }
        }
        return false;
    }

    static bool IsGod(const RE::Actor* actor)
    {
        return actor && actor->IsPlayerRef() && RE::PlayerCharacter::IsGodMode();
    }

    // Credits: https://github.com/shad0wshayd3-TES5/BakaBloodMagic/blob/10aa95c56244aff3f1c78c5584968e8a9f827341/src/BloodMagic/Utils.h#L142
    static void InterruptActor(RE::Actor *a_actor, RE::MagicSystem::CastingSource a_castingSource)
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


    class Timer
    {
    public:
        void Start()
        {
            if (!running) {
                startTime = std::chrono::steady_clock::now();
                running = true;
            }
        }

        void Stop()
        {
            running = false;
        }

        void Reset()
        {
            startTime = std::chrono::steady_clock::now();
        }

        double ElapsedSeconds() const
        {
            if (!running) {
                return 0.0;
            }
            auto now = std::chrono::steady_clock::now();
            return std::chrono::duration<double>(now - startTime).count();
        }

        bool IsRunning() const
        {
            return running;
        }

    private:
        std::chrono::steady_clock::time_point startTime{};
        bool running{ false };
    };

    struct Curses
    {
        static inline std::unordered_map<RE::Actor*, RE::SpellItem*> active_curses;
        static inline std::unordered_map<RE::Actor*, Timer> curse_swap_timers;

        static RE::SpellItem* GetRandomSpell(const std::vector<RE::SpellItem*>& spells)
        {
            if (spells.empty()) {
                return nullptr;
            }

            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_int_distribution<std::size_t> dist(0, spells.size() - 1);

            return spells[dist(gen)];
        }

        static bool ShouldApplyCurse(float chancePercent)
        {
            if (chancePercent <= 0.0f)
                return false;
            if (chancePercent >= 100.0f)
                return true;

            static std::random_device rd;
            static std::mt19937 gen(rd());
            std::uniform_real_distribution<float> dist(0.0f, 100.0f);
            REX::DEBUG("chance percent is {}", chancePercent);
            return dist(gen) < chancePercent;
        }

        static void ApplyRandomCurse(RE::Actor* a_actor, const std::vector<RE::SpellItem*>& curses)
        {
            double CURSE_SWAP_COOLDOWN = Config::Settings::curse_swap_cooldown.GetValue(); // 60 seconds
            REX::DEBUG("curse vector size is: {}", curses.size());
            if (!a_actor || curses.empty())
                return;

            // If actor already has any curse, swap instead of reapplying
            for (auto curse : curses) {
                if (curse && a_actor->HasSpell(curse)) {
                    auto& timer = curse_swap_timers[a_actor];
                    if (!timer.IsRunning() || timer.ElapsedSeconds() >= CURSE_SWAP_COOLDOWN) {
                        RE::SpellItem* newCurse = GetRandomSpell(curses);
                        if (newCurse && newCurse != curse && Config::Settings::allow_curse_swapping.GetValue()) {
                            SwapCurse(a_actor, newCurse);
                            timer.Reset();
                            timer.Start();
                            return;
                        }
                        else {
                            REX::DEBUG("{} already has this curse: {}", a_actor->GetName(), curse->GetName());
                        }
                        return;
                    }
                    else {
                        REX::DEBUG("{} tried to swap a curse too soon. {:.1f}s remaining", a_actor->GetName(), CURSE_SWAP_COOLDOWN - timer.ElapsedSeconds());
                    }
                    return;
                }
            }

            RE::SpellItem* curse_to_add = GetRandomSpell(curses);
            if (curse_to_add) {
                ApplySpell(a_actor, a_actor, curse_to_add);
                Curses::active_curses[a_actor] = curse_to_add;
                curse_swap_timers[a_actor].Start();
                REX::DEBUG("Applied curse {} to {}", curse_to_add->GetName(), a_actor->GetName());
            }
            else {
                REX::WARN("Failed to apply curse: selected spell was null");
            }

        }

        static void CleanseCurse(RE::Actor* a_actor)
        {
            auto it = active_curses.find(a_actor);
            if (it != active_curses.end() && it->second) {
                a_actor->RemoveSpell(it->second);
                active_curses.erase(it);
                REX::DEBUG("{}'s curse has been cleansed", a_actor->GetName());
            }
        }
        static void SwapCurse(RE::Actor* actor, RE::SpellItem* newCurse)
        {
            if (!actor || !newCurse)
                return;

            // If there's already a curse, dispel it  effectEnd will handle restoration
            for (auto& curse : Forms::FormLoader::curse_list) {
                if (curse && actor->HasSpell(curse)) {
                    CleanseCurse(actor);
                    break;
                }
            }
            // Apply the new curse
            Utility::ApplySpell(actor, actor, newCurse);
            active_curses[actor] = newCurse;
            REX::DEBUG("{} has been cursed with {}", actor->GetName(), newCurse->GetName());
        }

        inline static void PopulateActiveCursesAfterLoad(RE::Actor* a_actor)
        {
            class Visitor : public RE::Actor::ForEachSpellVisitor
            {
            public:
                RE::BSContainer::ForEachResult Visit(RE::SpellItem* a_spell) override
                {
                    for (auto& curse : Forms::FormLoader::curse_list) {
                        if (a_spell == curse) {
                            foundCurse = curse;
                            return RE::BSContainer::ForEachResult::kStop; // found one, done
                        }
                    }
                    return RE::BSContainer::ForEachResult::kContinue;
                }

                RE::SpellItem* foundCurse{ nullptr };
            } visitor;

            a_actor->VisitSpells(visitor);

            if (visitor.foundCurse) {
                active_curses[a_actor] = visitor.foundCurse;
                REX::DEBUG("Restored active curse {} on {}", visitor.foundCurse->GetName(), a_actor->GetName());
            }
            else {
                Utility::Curses::CleanseCurse(a_actor);
            }
        }
    };

    struct Actor
    {
        static inline bool jumpHeightPlayer(RE::PlayerCharacter* player, float height_mod)
        {
            if (!player->IsInMidair())
            {
                player->GetCharController()->jumpHeight *= height_mod;
                return true;
            }
            return false;
        }
        static inline bool applyDebuffs(RE::PlayerCharacter* player, RE::SpellItem* a_debuff, bool a_check)
        {
            if (a_check)
            {
                ApplySpell(player, player, a_debuff);
                return true;
            }
            else
                return false;
        }
    };

    inline static RE::TESObjectWEAP* GetUnarmedWeapon()
    {
        auto** singleton{ reinterpret_cast<RE::TESObjectWEAP**>(Cache::getUnarmedWeaponAddress) };
        return *singleton;
    }

    inline static RE::TESObjectWEAP* GetWeapon(RE::Actor* a, bool left)
    {
        if (auto _weap = a->GetEquippedObject(left)) {
            if (auto weap = _weap->As<RE::TESObjectWEAP>(); weap && weap->IsWeapon()) {
                return weap;
            }
        }

        return GetUnarmedWeapon();
    }

    static inline RE::TESObjectARMO* GetShield(RE::Actor* a)
    {
        if (auto _shield = a->GetEquippedObject(true)) {
            if (auto shield = _shield->As<RE::TESObjectARMO>()) {
                return shield->GetSlotMask() == RE::BGSBipedObjectForm::BipedObjectSlot::kShield ? shield : nullptr;
            }
        }

        return nullptr;
    }

    static inline float GetDefenseToolWeight(RE::Actor* a)
    {
        if (auto shield = GetShield(a)) {
            return shield->GetWeight();
        }
        else {
            return GetWeapon(a, false)->GetWeight();
        }
    }

    static inline float GetOffensiveToolWeight(RE::Actor* a, bool left, bool bash) {
        return bash ? GetDefenseToolWeight(a) : GetWeapon(a, left)->GetWeight();
    }

    static inline void InterruptCaster(RE::Actor* a_actor)
    {
        RE::SourceActionMap::DoAction(a_actor, RE::DEFAULT_OBJECT::kActionVoiceInterrupt);
        RE::SourceActionMap::DoAction(a_actor, RE::DEFAULT_OBJECT::kActionRightInterrupt);
        RE::SourceActionMap::DoAction(a_actor, RE::DEFAULT_OBJECT::kActionLeftInterrupt);

    }

    struct CastInterruptTimer {
        Timer attackTimer;     
        Timer cooldownTimer;    
        bool attackActive{ false };
        bool cooldownActive{ false };

        void OnHit() {
            if (!cooldownActive) {
                attackActive = true;
                attackTimer.Reset();
                attackTimer.Start();
            }
        }

        void Update() {
            if (attackActive && attackTimer.ElapsedSeconds() >= 1.5) {
                attackActive = false;
                cooldownActive = true;
                cooldownTimer.Reset();
                cooldownTimer.Start();
            }

            if (cooldownActive && cooldownTimer.ElapsedSeconds() >= 12.0) {
                cooldownActive = false;
                cooldownTimer.Stop();
            }
        }

        bool IsAttackActive() const { return attackActive; }
    };

};
