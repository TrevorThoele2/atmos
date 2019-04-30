
#pragma once

#include "ObjectSystem.h"

#include "ActionComponent.h"
#include "Entity.h"

#include "String.h"

#include <AGUI/Root.h>
#include <AGUI/Textbox.h>
#include "Cursor.h"

namespace Atmos
{
    class LoggingSystem;

    namespace Speech
    {
        class Shop;

        class Controller : public ObjectSystem
        {
        public:
            typedef int InputID;
        public:
            typedef TypedObjectReference<Entity::Entity> EntityReference;
            typedef TypedObjectReference<Entity::ActionComponent> ActionComponentReference;
        public:
            Controller(ObjectManager& manager);
            Controller(const ::Inscription::Table<Controller>& table);

            bool Enter(ActionComponentReference actionComponent);
            void Leave();
            bool IsActive() const;

            EntityReference EntityTalkingTo() const;

            void SetOutput(const String& str);
            void AppendToOutput(const String& append);
            void ClearOutput();
            const String& Output();

            void ActivateInput(std::vector<std::string>&& strings);
            void DeactivateInput();
            InputID InputPosition();
        private:
            ActionComponentReference currentDialogue;
        private:
            Shop* shop;
        private:
            void InitializeImpl() override;
            void WorkImpl() override;
        private:
            Agui::Root* root;
            Agui::Textbox* textbox;
            Agui::TextComponent* nameOutput;
            Agui::TextComponent* mainOutput;
            void InitializeGui();
        private:
            Name NameFor(EntityReference entity);
        private:
            LoggingSystem* FindLoggingSystem();
        private:
            class Input
            {
            public:
                Input();
                void Initialize(Agui::Textbox& textbox, ObjectManager& objectManager);

                void Activate(std::vector<String>&& strings);
                void Deactivate();

                InputID CurrentPosition() const;
            private:
                class Entry
                {
                public:
                    Agui::Textbox::Connection textboxConnection;
                    Entry(Agui::Textbox& textbox, const std::string& name, const std::string& string);
                };

                typedef std::unordered_map<InputID, Entry> Entries;
                Entries entries;
            private:
                Agui::Cursor* cursor;
                Agui::Textbox* handlerTextbox;
            };

            Input input;
        private:
            friend Shop;
        };
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DECLARE(::Atmos::Speech::Controller)
    {
    public:
        INSCRIPTION_INSCRIPTER_DECLARE_TABLE;
        INSCRIPTION_DECLARE_CLASS_NAME_RESOLVER;
    };
}