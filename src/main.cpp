#include "hooks.h"
#include "settings.h"
#include "formloader.h"
#include "papyrus.h"
#include "serialisation.h"
#include "cache.h"

void Listener(SKSE::MessagingInterface::Message* message) noexcept
{
    switch (message->type) {
    case SKSE::MessagingInterface::kDataLoaded:
        Hooks::Install();
        Forms::FormLoader::GetSingleton()->LoadForms();
        Events::RegisterEvents();
        Config::Exceptions::LoadJson();
        break;
    case SKSE::MessagingInterface::kPostLoadGame:
        Utility::Curses::PopulateActiveCursesAfterLoad(Cache::GetPlayerSingleton());
        if (!Config::Settings::enable_diseases.GetValue()) {
            Utility::Curses::CleanseCurse(Cache::GetPlayerSingleton());
        }
        break;
    default:
        break;
    }
}

SKSE_PLUGIN_LOAD(const SKSE::LoadInterface* a_skse)
{
	SKSE::Init(a_skse, {.trampoline = true});
    Cache::CacheAddLibAddresses();

    const auto papyrus = SKSE::GetPapyrusInterface();
    papyrus->Register(Papyrus::BindAll);

    if (auto serialization = SKSE::GetSerializationInterface())
    {
        serialization->SetUniqueID(Serialisation::ID);
        serialization->SetSaveCallback(&Serialisation::SaveCallback);
        serialization->SetLoadCallback(&Serialisation::LoadCallback);
        serialization->SetRevertCallback(&Serialisation::RevertCallback);
    }

    SKSE::GetMessagingInterface()->RegisterListener(Listener);
    Config::Settings::GetSingleton()->LoadSettings();

	return true;
}
