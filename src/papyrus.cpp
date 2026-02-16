#include "papyrus.h"
#include "hooks.h"
#include "mod-storage.h"
#include <string_view>

#include "damage-manager.h"

namespace Papyrus
{
bool BindAll(VM *a_vm)
{
    if (!a_vm)
    {
        REX::CRITICAL("couldn't get VM State");
        return false;
    }

    Functions::Bind(*a_vm);

    return true;
}

namespace Functions
{

void Bind(VM &a_vm)
{
    constexpr std::string_view script = "STweaksFunctions";

    a_vm.RegisterFunction("GetVersion", script, GetVersion, true);
    a_vm.RegisterFunction("CleanseCurseActor", script, CleanseCurseActor, true);
    if (std::to_underlying(APIVersion::kVersion) > 1)
    {
        a_vm.RegisterFunction("ChangeOpportunityModifier", script, ChangeOpportunityModifier, true);
        a_vm.RegisterFunction("GetOpportunityModifier", script, GetOpportunityModifier, true);
    }

    REX::INFO("Registered STweaks functions for version {}", std::to_underlying(APIVersion::kVersion));
}

std::int32_t GetVersion(STATIC_ARGS)
{
    return std::to_underlying(APIVersion::kVersion);
}

void CleanseCurseActor(STATIC_ARGS, RE::Actor *a_actor)
{

    if (!a_actor)
    {
        a_vm->TraceStack("Actor is None", a_stackID);
        return;
    }
    Utility::Curses::CleanseCurse(a_actor);
}
void ChangeOpportunityModifier(STATIC_ARGS, int opportunityType, float modifier)
{
  if (opportunityType < 0 ||
      opportunityType >= stweaks::OppModi::OpportunityModifiers.size())
    {
        REX::WARN("Invalid opportunity type in ChangeOpportunityModifier script function");
        return;
    }
    stweaks::OppModi::OpportunityModifiers[opportunityType] = modifier;
    REX::DEBUG("Changed Opportunity Type({}) Modifier to: {}", opportunityType, modifier);
}
float GetOpportunityModifier(STATIC_ARGS, int opportunityType)
{
  if (opportunityType < 0 ||
      opportunityType >= stweaks::OppModi::OpportunityModifiers.size())
    {
        REX::WARN("Invalid opportunity type in GetOpportunityModifier script "
                  "function");
        return 1.0f;
    }
    return stweaks::OppModi::OpportunityModifiers[opportunityType];
}
} // namespace Functions

} // namespace Papyrus
