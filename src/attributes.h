//
// Created by styyx on 15/02/2026.
//

#pragma once

namespace stweaks
{
    struct Attributes : REX::TSingleton<Attributes>
    {
        static void UpdateAttributes(RE::PlayerCharacter* player);
    private:
        static inline TimerUtil::Timer av_timer;
        static void ManageAttributeGrowthPlayer(RE::PlayerCharacter* a_this);
        static float GetCarryPercentage(RE::PlayerCharacter* a_player);
        static inline constexpr std::array restricted_menu_names{
            RE::BarterMenu::MENU_NAME,    RE::BookMenu::MENU_NAME,     RE::Console::MENU_NAME,
            RE::ContainerMenu::MENU_NAME, RE::CraftingMenu::MENU_NAME, RE::DialogueMenu::MENU_NAME,
            RE::FavoritesMenu::MENU_NAME, RE::GiftMenu::MENU_NAME,     RE::InventoryMenu::MENU_NAME,
            RE::JournalMenu::MENU_NAME,   RE::LevelUpMenu::MENU_NAME,  RE::LockpickingMenu::MENU_NAME,
            RE::MagicMenu::MENU_NAME,     RE::MapMenu::MENU_NAME,      RE::RaceSexMenu::MENU_NAME,
            RE::SleepWaitMenu::MENU_NAME, RE::StatsMenu::MENU_NAME,    RE::TrainingMenu::MENU_NAME,
            RE::TutorialMenu::MENU_NAME,  RE::TweenMenu::MENU_NAME,
        };
        static inline std::vector<std::string> a_menuNames{restricted_menu_names.begin(), restricted_menu_names.end()};
        static inline bool IsAnyOfMenuOpen(const std::vector<std::string> &m_menuNames)
        {
            auto a_ui = RE::UI::GetSingleton();
            if (!a_ui)
                return true;
            for (const std::string_view menuName : m_menuNames)
            {
                if (a_ui->IsMenuOpen(menuName))
                {
                    return true;
                }
            }
            return false;
        }
    };
}
