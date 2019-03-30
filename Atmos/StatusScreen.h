
#pragma once

#include <memory>
#include <vector>
#include <array>

#include "State.h"
#include "UniqueStack.h"

#include "ObjectReference.h"
#include "nEntity.h"
#include "CombatComponent.h"
#include "Spell.h"

namespace Atmos
{
    /*
    class MainGame;
    class StatusScreen
    {
    public:
        enum class PageID
        {
            STATS,
            INVENTORY,
            SPELLS
        };
    public:
        static void Init();
        static void Goto(PageID page);
        static bool IsActive();
    private:
        typedef size_t PartySizeT;
    private:
        typedef TypedObjectReference<Ent::nEntity> EntityReference;
        typedef TypedObjectReference<Ent::nCombatComponent> CombatComponentReference;
        typedef TypedObjectReference<nSpell> SpellReference;
    private:
        class Member;
        // Pages
        class Page
        {
        public:
            Page(const std::string &rootName, Agui::Root &mainRoot);
            virtual void Update(EntityReference selected) = 0;
            void Select(EntityReference selected);
            void Deselect();

            virtual void OnActionPressed(const Input::Action &args) {} 
        protected:
            Agui::Root* GetRoot();
            bool IsSelected() const;
        private:
            Agui::Root *root;
            virtual void OnSelected() = 0;
            virtual void OnDeselected() = 0;
        };

        class Stats : public Page
        {
        public:
            Stats(Agui::Root &mainRoot);
            void Update(EntityReference selected) override;
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
                Agui::TextComponent *component;
                Name attributeName;
                Name niceName;
                Type type;
            public:
                Entry(Agui::TextComponent &component, const Name &niceName);
                Entry(Agui::TextComponent &component, const Name &attributeName, bool isStat);
                void Update(CombatComponentReference selected);
            };
        private:
            std::vector<Entry> entries;
            void OnSelected() override {}
            void OnDeselected() override {}
            // Name entry
            void CreateEntry(Agui::Menu &menu);
            // Attribute entry
            void CreateEntry(Agui::Menu &menu, const Name &attributeName, bool isStat);
        };

        class Inventory : public Page
        {
        public:
            Inventory(Agui::Root &mainRoot);
            void Update(EntityReference selected) override;

            void OnActionPressed(const Input::Action &args) override;
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

                SectionBase(Agui::Root &root, const Agui::RelativePosition &position, Agui::ItemDescriptionBox &description, Agui::ItemContextMenu &contextMenu, const std::string &labelString, Inventory &handler);
                virtual void Update(EntityReference selected) = 0;
                virtual void Setup() = 0;
                void Focus();
                void Unfocus();
            };

            class CharacterSection : public SectionBase
            {
            private:
                void OnFocus() override;
            public:
                CharacterSection(Agui::Root &root, const Agui::RelativePosition &position, Agui::ItemDescriptionBox &description, Agui::ItemContextMenu &contextMenu, Inventory &handler);
                void Update(EntityReference selected) override;
                void Setup() override {}
            };

            class ReserveSection : public SectionBase
            {
            private:
                void OnFocus() override {}
            public:
                ReserveSection(Agui::Root &oot, const Agui::RelativePosition &position, Agui::ItemDescriptionBox &description, Agui::ItemContextMenu &contextMenu, Inventory &handler);
                void Update(EntityReference selected) override;
                void Setup() override;
            };
        private:
            typedef std::unique_ptr<SectionBase> SectionPtr;
            typedef std::array<SectionPtr, 2> Sections;
            Sections sections;
            Sections::iterator selectedSection;
            Sections::iterator characterSection;
            Sections::iterator reserveSection;

            Agui::ItemContextMenu *contextMenu;

            EventBoundSubscriberManager boundSubscribers;

            void OnSelected() override;
            void OnDeselected() override;
            void OnSectionUpdated();

            void IncSelectedSection();
            void SelectSection(Sections::iterator select);
        private:
            friend SectionBase;
        };

        class Spells : public Page
        {
        public:
            Spells(Agui::Root &mainRoot);
            void Update(EntityReference selected) override;
        private:
            Spellbook gui;
            void OnSelected() override;
            void OnDeselected() override;
            void OnSpellSelected(SpellReference spell);
        };
        // End pages

        class Member
        {
        public:
            Agui::Image &frame;
            Agui::Image &portrait;
            EntityReference player;
            Member(Agui::Image &frame);
        };
    private:
        static Agui::Root *root;

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
        static T* CreatePage(PageID id, Agui::Root &mainRoot);
    private:
        friend MainGame;
    };

    template<class T>
    T* StatusScreen::CreatePage(PageID id, Agui::Root &mainRoot)
    {
        auto made = new T(mainRoot);
        pages.emplace(id, PagePtr(made));
        return made;
    }
    */
}