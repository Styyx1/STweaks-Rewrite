//
// Created by styyx on 11/02/2026.
//

#pragma once

namespace stweaks
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
            const auto opp = static_cast<int>(type);
            if (opp > 4)
                return 1.0;
            return OpportunityModifiers[opp];
        }
    };

}