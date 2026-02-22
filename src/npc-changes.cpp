//
// Created by styyx on 20/02/2026.
//

#include "npc-changes.h"
#include "Settings.h"

namespace stweaks
{
    void NPCChanges::LevelUpNpcs(RE::Actor* a_this, float a_playerLevel)
    {
        if (Config::Settings::level_up_low_levels.GetValue())
        {
            //levels NPCs up to min 10 levels below yours so you don't encounter level 5 enemies at level 50
            auto npc_level = a_this->GetLevel();
            if (npc_level + 10 < a_playerLevel)
            {
                ActorUtil::SetNPCLevel(a_this, a_playerLevel - 10);
            }
        }
    }

    void NPCChanges::ChangeNPCResistances(RE::Actor* a_this, float a_playerLevel)
    {
        if (Config::Settings::enable_resist_changes.GetValue())
        {
            //reduces the magic resistance for npcs early game
            if (a_playerLevel <= LEVEL_CAP && a_this->GetLevel() >= a_playerLevel &&
                a_this->GetActorValue(RE::ActorValue::kResistMagic) >= 0)
            {
                a_this->SetActorValue(RE::ActorValue::kResistMagic,
                                      a_this->GetActorValue(RE::ActorValue::kResistMagic) -
                                          static_cast<float>(Config::Settings::resist_reduction_value.GetValue()));
            }
        }
    }
}
