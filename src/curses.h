//
// Created by styyx on 11/02/2026.
//

#pragma once
#include "../lib/commonlibsse/lib/commonlib-shared/include/REX/REX/Singleton.h"

namespace stweaks
{
    struct Curses : REX::TSingleton<Curses>
    {
        static void RemoveCurse(RE::ActiveEffect* a_curseEffect);
        static bool BlockSpellWithCurse(RE::Actor* a_actor, const RE::MagicItem* a_spellItem);
    };
}
