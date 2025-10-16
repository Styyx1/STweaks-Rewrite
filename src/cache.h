#pragma once

namespace Cache
{
	inline uintptr_t IsAttackingAddress;
	inline uintptr_t IsBlockingAddress;
	inline uintptr_t HasSpellAddress;
	inline uintptr_t PlayerSingletonAddress;

	// https://github.com/colinswrath/BladeAndBlunt/blob/main/include/Cache.h
	inline void CacheAddLibAddresses()
	{
		//1.6 = 38590
		//1.5.97 = 37637
		IsAttackingAddress = REL::ID(38590).address();

		//1.6 = 37952
		//1.5.97 = 36927
		IsBlockingAddress = REL::ID(37952).address();
		// 1.6 = 38782
		// 1.5.97 = 37828
		HasSpellAddress = REL::ID(38782).address();

		// 1.6 = 403521
		// 1.5 = 517014
		PlayerSingletonAddress = REL::ID(403521).address();
	}

	inline RE::PlayerCharacter* GetPlayerSingleton()
	{
		REL::Relocation<RE::NiPointer<RE::PlayerCharacter>*> singleton{ PlayerSingletonAddress };
		return singleton->get();
	}
}
