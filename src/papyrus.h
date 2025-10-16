#pragma once

#include "utility.h"

#define STATIC_ARGS [[maybe_unused]] VM *a_vm, [[maybe_unused]] StackID a_stackID, RE::StaticFunctionTag *

namespace Papyrus
{
	using VM = RE::BSScript::Internal::VirtualMachine;
	using StackID = RE::VMStackID;

	bool BindAll(VM* a_vm);

	namespace Functions
	{
		enum
		{
			kVersion = 1
		};

		std::int32_t GetVersion(STATIC_ARGS);
		void CleanseCurseActor(STATIC_ARGS, RE::Actor* a_actor);

		void Bind(VM& a_vm);
	}
}