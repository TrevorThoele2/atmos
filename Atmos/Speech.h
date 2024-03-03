
#pragma once

#include "Shop.h"
#include "Entity.h"
#include "String.h"

#include <AGUI\Root.h>
#include "Cursor.h"

namespace Atmos
{
    namespace Ent { class DialogueComponent; }

    namespace Speech
    {
        enum class InnkeeperFlags
        {
            NOT_ENOUGH_GOLD,
            OK
        };

        class Handler
        {
        private:
            class Input
            {
            public:
                typedef unsigned char ID;
            public:
                Input();
                void Init(agui::Textbox &textbox);
                void Activate(std::vector<String> &&strings);
                void Deactivate();
                ID GetCurrentPosition() const;
            private:
                class Entry
                {
                public:
                    agui::Textbox::Connection textboxConnection;
                    Entry(agui::Textbox &textbox, const std::string &name, const std::string &string);
                };
            private:
                typedef std::unordered_map<ID, Entry> Entries;
                typedef agui::Cursor<ID> CursorT;
                CursorT *cursor;

                Entries entries;

                agui::Textbox *handlerTextbox;
            };
        public:
            typedef Input::ID InputID;
        private:
            static Handler instance;
            Input input;

            static agui::Root *root;
            static agui::Textbox *textbox;
            static agui::TextComponent *nameOutput, *mainOutput;

            static Ent::DialogueComponent *currentDialogue;

            Handler() = default;
            Handler(const Handler &arg) = delete;
            Handler& operator=(const Handler &arg) = delete;
            static void InitGui();

            void OnActionPressed(const ::Atmos::Input::Action &args);
        public:
            static Handler& Instance();
            static void Init();
            static void Work();
            static bool Enter(Ent::DialogueComponent &dialogue);
            static void Leave();
            static bool IsWorking();

            static Entity GetEntityTalking();

            static void SetString(const std::string &str);
            static void AppendString(const std::string &append);
            static void ClearString();
            static const String& GetOutput();

            static void ActivateInput(std::vector<std::string> &&strings);
            static void DeactivateInput();
            static InputID GetInputPosition();

            friend Shop;
        };
    }
}