
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
                void Init(Agui::Textbox &textbox);
                void Activate(std::vector<String> &&strings);
                void Deactivate();
                ID GetCurrentPosition() const;
            private:
                class Entry
                {
                public:
                    Agui::Textbox::Connection textboxConnection;
                    Entry(Agui::Textbox &textbox, const std::string &name, const std::string &string);
                };
            private:
                typedef std::unordered_map<ID, Entry> Entries;
                typedef Agui::Cursor CursorT;
                CursorT *cursor;

                Entries entries;

                Agui::Textbox *handlerTextbox;
            };
        public:
            typedef Input::ID InputID;
        private:
            static Handler instance;
            Input input;

            static Agui::Root *root;
            static Agui::Textbox *textbox;
            static Agui::TextComponent *nameOutput, *mainOutput;

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