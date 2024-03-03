
#pragma once

#include <memory>
#include <vector>
#include <array>

#include "State.h"
#include "PlayerParty.h"
#include "UniqueStack.h"

#include "InventoryGui.h"
#include "Spellbook.h"

namespace Atmos
{
    class MainGame;
    class StatusScreen
    {
    private:
        typedef Ent::PlayerParty::SizeT PartySizeT;
    private:
        class Member;
        // Pages
        class Page
        {
        private:
            agui::Root *root;
            virtual void OnSelected() = 0;
            virtual void OnDeselected() = 0;
        protected:
            agui::Root* GetRoot();
            bool IsSelected() const;
        public:
            Page(const std::string &rootName, agui::Root &mainRoot);
            virtual void Update(Entity selected) = 0;
            void Select(Entity selected);
            void Deselect();

            virtual void OnActionPressed(const Input::Action &args) {}
        };

        class Stats : public Page
        {
        private:
            class Entry
            {
            private:
                enum class Type : unsigned char
                {
                    STAT,
                    RESOURCE,
                    NAME
                };
            private:
                agui::TextComponent *component;
                Name attributeName;
                Name niceName;
                Type type;
            public:
                Entry(agui::TextComponent &component, const Name &niceName);
                Entry(agui::TextComponent &component, const Name &attributeName, bool isStat);
                void Update(const Ent::CombatComponent *selected);
            };
        private:
            std::vector<Entry> entries;
            void OnSelected() override {}
            void OnDeselected() override {}
            // Name entry
            void CreateEntry(agui::Menu &menu);
            // Attribute entry
            void CreateEntry(agui::Menu &menu, const Name &attributeName, bool isStat);
        public:
            Stats(agui::Root &mainRoot);
            void Update(Entity selected) override;
        };

        class Inventory : public Page
        {
        private:
            class SectionBase
            {
            private:
                Inventory &handler;
                virtual void OnFocus() = 0;
                virtual void OnUnfocus() {}
            public:
                SectionBase *other;
                Atmos::InventoryGui gui;

                SectionBase(agui::Root &root, const agui::RelativePosition &position, agui::ItemDescriptionBox &description, agui::ItemContextMenu &contextMenu, const std::string &labelString, Inventory &handler);
                virtual void Update(Entity selected) = 0;
                virtual void Setup() = 0;
                void Focus();
                void Unfocus();
            };

            class CharacterSection : public SectionBase
            {
            private:
                void OnFocus() override;
            public:
                CharacterSection(agui::Root &root, const agui::RelativePosition &position, agui::ItemDescriptionBox &description, agui::ItemContextMenu &contextMenu, Inventory &handler);
                void Update(Entity selected) override;
                void Setup() override {}
            };

            class ReserveSection : public SectionBase
            {
            private:
                void OnFocus() override {}
            public:
                ReserveSection(agui::Root &oot, const agui::RelativePosition &position, agui::ItemDescriptionBox &description, agui::ItemContextMenu &contextMenu, Inventory &handler);
                void Update(Entity selected) override;
                void Setup() override;
            };
        private:
            typedef std::unique_ptr<SectionBase> SectionPtr;
            typedef std::array<SectionPtr, 2> Sections;
            Sections sections;
            Sections::iterator selectedSection;
            Sections::iterator characterSection;
            Sections::iterator reserveSection;

            agui::ItemContextMenu *contextMenu;

            EventBoundSubscriberManager boundSubscribers;

            void OnSelected() override;
            void OnDeselected() override;
            void OnSectionUpdated();

            void IncSelectedSection();
            void SelectSection(Sections::iterator select);
        public:
            Inventory(agui::Root &mainRoot);
            void Update(Entity selected) override;

            void OnActionPressed(const Input::Action &args) override;
            friend SectionBase;
        };

        class Spells : public Page
        {
        private:
            Spellbook gui;
            void OnSelected() override;
            void OnDeselected() override;
            void OnSpellSelected(const AbilityBase &ability);
        public:
            Spells(agui::Root &mainRoot);
            void Update(Entity selected) override;
        };
        // End pages

        class Member
        {
        public:
            agui::Image &frame;
            agui::Image &portrait;
            Entity player;
            Member(agui::Image &frame);
        };
    public:
        enum class PageID
        {
            STATS,
            INVENTORY,
            SPELLS
        };
    private:
        static agui::Root *root;

        static bool active;

        typedef std::unique_ptr<Page> PagePtr;
        typedef std::unordered_map<PageID, PagePtr> Pages;
        static Pages pages;
        static Pages::iterator currentPage;

        typedef std::vector<Member> Members;
        static Members members;
        static Members::iterator selectedMember;
        static const PartySizeT rowSize;

        StatusScreen() = default;
        StatusScreen(const StatusScreen &arg) = delete;
        StatusScreen& operator=(const StatusScreen &arg) = delete;

        static void OnActionPressed(const Input::Action &args);

        static void Activate();
        static void Leave();
        template<class T>
        static T* CreatePage(PageID id, agui::Root &mainRoot);
    public:
        static void Init();
        static void Goto(PageID page);
        static bool IsActive();
        friend MainGame;
    };

    template<class T>
    StatusScreen::Inventory::Sections::iterator StatusScreen::Inventory::CreateSection(agui::Root &mainRoot, const agui::RelativePosition &position, agui::ItemDescriptionBox &description, agui::ItemContextMenu &contextMenu)
    {
        sections.push_back(SectionPtr(new T(mainRoot, position, description, contextMenu, *this)));
        return --sections.end();
    }

    template<class T>
    T* StatusScreen::CreatePage(PageID id, agui::Root &mainRoot)
    {
        auto made = new T(mainRoot);
        pages.emplace(id, PagePtr(made));
        return made;
    }
}