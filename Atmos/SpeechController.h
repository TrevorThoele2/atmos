
#pragma once

#include "ObjectSystem.h"

#include "ActionComponent.h"
#include "nEntity.h"

#include "String.h"

#include <AGUI\Root.h>
#include <AGUI\Textbox.h>
#include "Cursor.h"

namespace Atmos
{
    namespace Speech
    {
        class Shop;

        class Controller : public ObjectSystem
        {
        public:
            typedef int InputID;
        public:
            typedef TypedObjectReference<Ent::nEntity> EntityReference;
            typedef TypedObjectReference<Ent::nActionComponent> ActionComponentReference;
        public:
            Controller(ObjectManager& manager);

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
            class Input
            {
            public:
                Input();
                void Initialize(Agui::Textbox& textbox);

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