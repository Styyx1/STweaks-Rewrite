#pragma once
#include "Events.h"
#include "mod-storage.h"

// Credits: https://github.com/colinswrath/BladeAndBlunt/blob/main/include/Serialization.h
namespace Serialisation
{
static constexpr std::uint32_t SerializationVersion = 2;
static constexpr std::uint32_t ID = 'STWK';
static constexpr std::uint32_t SerializationType = 'STWD';

inline void SaveCallback(SKSE::SerializationInterface *a_skse)
{

    if (!a_skse->OpenRecord(SerializationType, SerializationVersion))
    {
        REX::ERROR("Failed to open STweaks record");
        return;
    }
    else
    {
        auto store = stweaks::ModStorage::GetSingleton();
        auto healthdebuffToSerialise = store->storedHealth_disease;
        auto staminadebuffToSerialise = store->storedStamina_disease;
        auto magickadebuffToSerialise = store->storedMagicka_disease;

        auto av_store = stweaks::AVStorage::GetSingleton();
        auto health_xp = av_store->get_attribute_xp(RE::ActorValue::kHealth);
        auto stamina_xp = av_store->get_attribute_xp(RE::ActorValue::kStamina);
        auto magicka_xp = av_store->get_attribute_xp(RE::ActorValue::kMagicka);
        auto carry_xp = av_store->get_attribute_xp(RE::ActorValue::kCarryWeight);

        if (!a_skse->WriteRecordData(healthdebuffToSerialise))
            return;
        if (!a_skse->WriteRecordData(staminadebuffToSerialise))
            return;
        if (!a_skse->WriteRecordData(magickadebuffToSerialise))
            return;
        if (!a_skse->WriteRecordData(health_xp))
            return;
        if (!a_skse->WriteRecordData(stamina_xp))
            return;
        if (!a_skse->WriteRecordData(magicka_xp))
            return;
        if (!a_skse->WriteRecordData(carry_xp))
            return;
        else
        {
            REX::INFO("saved Health Penalty: {}", healthdebuffToSerialise);
            REX::INFO("saved Stamina Penalty: {}", staminadebuffToSerialise);
            REX::INFO("saved Magicka Penalty: {}", magickadebuffToSerialise);
            REX::INFO("saved Health XP: {}", health_xp);
            REX::INFO("saved Stamina XP: {}", stamina_xp);
            REX::INFO("saved Magicka XP: {}", magicka_xp);
            REX::INFO("saved Carry Weight XP: {}", carry_xp);
        }
    }
}

inline void LoadCallback(SKSE::SerializationInterface *a_skse)
{
    std::uint32_t type;
    std::uint32_t version;
    std::uint32_t length;
    a_skse->GetNextRecordInfo(type, version, length);

    auto *store = stweaks::ModStorage::GetSingleton();
    auto *av_store = stweaks::AVStorage::GetSingleton();

    if (type != SerializationType)
    {
        return;
    }

    if (version != SerializationVersion)
    {
        REX::ERROR("Unable to load data");
        return;
    }

    float deserialisedHealth;
    float deserialisedStamina;
    float deserialisedMagicka;
    uint16_t health_xp;
    uint16_t stamina_xp;
    uint16_t magicka_xp;
    uint16_t carry_xp;

    if (!a_skse->ReadRecordData(deserialisedHealth))
        return;
    if (!a_skse->ReadRecordData(deserialisedStamina))
        return;
    if (!a_skse->ReadRecordData(deserialisedMagicka))
        return;
    if (!a_skse->ReadRecordData(health_xp))
        return;
    if (!a_skse->ReadRecordData(stamina_xp))
        return;
    if (!a_skse->ReadRecordData(magicka_xp))
        return;
    if (!a_skse->ReadRecordData(carry_xp))
        return;

    else
    {
        store->storedHealth_disease = deserialisedHealth;
        store->storedStamina_disease = deserialisedStamina;
        store->storedMagicka_disease = deserialisedMagicka;
        av_store->attribute_xp [RE::ActorValue::kHealth] = health_xp;
        av_store->attribute_xp[RE::ActorValue::kStamina] = stamina_xp;
        av_store->attribute_xp[RE::ActorValue::kMagicka] = magicka_xp;
        av_store->attribute_xp[RE::ActorValue::kCarryWeight] = carry_xp;

        REX::INFO("Loaded Health Penalty: {}", deserialisedHealth);
        REX::INFO("Loaded Stamina Penalty: {}", deserialisedStamina);
        REX::INFO("Loaded Magicka Penalty: {}", deserialisedMagicka);
        REX::INFO("Loaded Health XP: {}", health_xp);
        REX::INFO("Loaded Stamina XP: {}", stamina_xp);
        REX::INFO("Loaded Magicka XP: {}", magicka_xp);
        REX::INFO("Loaded Carry Weight XP: {}", carry_xp);
    }
}

inline void RevertCallback([[maybe_unused]] SKSE::SerializationInterface *a_skse)
{
    const auto store = stweaks::ModStorage::GetSingleton();
    store->storedHealth_disease = 0.0f;
    store->storedStamina_disease = 0.0f;
    store->storedMagicka_disease = 0.0f;
    auto *av_store = stweaks::AVStorage::GetSingleton();
    av_store->attribute_xp[RE::ActorValue::kHealth] = 0;
    av_store->attribute_xp[RE::ActorValue::kStamina] = 0;
    av_store->attribute_xp[RE::ActorValue::kMagicka] = 0;
    av_store->attribute_xp[RE::ActorValue::kCarryWeight] = 0;
}
} // namespace Serialisation
