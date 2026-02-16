#pragma once

#include "settings.h"
#include "mod-data.h"

namespace Forms
{
	struct FormLoader
	{
		static inline bool disease_mod_active = false;
		static inline RE::SpellItem *sneak_stamina_spell{nullptr};
		static inline RE::SpellItem *health_curse{nullptr};
		static inline RE::SpellItem *stamina_curse{nullptr};
		static inline RE::SpellItem *magicka_curse{nullptr};
		static inline RE::SpellItem *silence_curse{nullptr};
		static inline RE::SpellItem *melee_damage_curse{nullptr};
		static inline RE::SpellItem *bow_damage_curse{nullptr};
		static inline RE::SpellItem *jump_curse{nullptr};
		static inline RE::BGSListForm *spell_allow_list{nullptr};
		static inline RE::BGSPerk *tall_grass_perk{nullptr};
		static inline RE::SpellItem* exhaustion_spell{ nullptr };
		static inline RE::BGSListForm* curses_formlist{ nullptr };

		using SpellVector = std::vector<RE::SpellItem*>;
		//can contain duplicates
		//this allows users to weight towards specific spells with the formlist

		static inline SpellVector curse_list;

		//doesn't specifically expect duplicates
		static inline SpellVector spell_allow_vector;

		static bool ContainsSpell(const SpellVector& list, RE::SpellItem* spell)
		{
			return std::ranges::find(list, spell) != list.end();
		}

		static void PopulateCurseList();
		static void AddFormListEntriesToCurseVector();
		static void LoadForms();
	};
} // namespace Forms
