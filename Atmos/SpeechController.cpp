
#include "SpeechController.h"

#include "Shop.h"
#include "StringUtility.h"

#include "GeneralComponent.h"
#include "ActionComponent.h"
#include "ScriptInstance.h"
#include "RunningScript.h"

#include "LoggingSystem.h"

#include "FontDefines.h"
#include <AGUI/System.h>

namespace Atmos
{
    namespace Speech
    {
        Controller::Controller(ObjectManager& manager) : ObjectSystem(manager)
        {}

        Controller::Controller(const ::Inscription::Table<Controller>& table) :
            INSCRIPTION_TABLE_GET_BASE(ObjectSystem)
        {}

        bool Controller::Enter(ActionComponentReference actionComponent)
        {
            if (currentDialogue)
                return false;

            if (!actionComponent->script.IsOccupied())
            {
                FindLoggingSystem()->Log("The dialogue from an entity is empty.",
                    LogType::ERROR_LOW,
                    LogNameValueVector{NameValuePair(
                        "Entity",
                        ToString(NameFor(actionComponent->owner))) });
                return false;
            }

            currentDialogue = actionComponent;
            nameOutput->SetString(NameFor(actionComponent->owner));
            mainOutput->ClearString();
            root->Show();
            return true;
        }

        void Controller::Leave()
        {
            if (currentDialogue)
                Manager()->DestroyObject(currentDialogue->script->RunningForThis());

            currentDialogue.Reset();
            root->Hide();
        }

        bool Controller::IsActive() const
        {
            return currentDialogue.IsOccupied();
        }

        Controller::EntityReference Controller::EntityTalkingTo() const
        {
            if (!currentDialogue)
                return EntityReference();

            return currentDialogue->owner;
        }

        void Controller::SetOutput(const String& str)
        {
            mainOutput->SetString(str);
        }

        void Controller::AppendToOutput(const String& append)
        {
            mainOutput->AppendString(append);
        }

        void Controller::ClearOutput()
        {
            mainOutput->ClearString();
        }

        const std::string& Controller::Output()
        {
            return mainOutput->GetString();
        }

        void Controller::ActivateInput(std::vector<std::string> &&strings)
        {
            input.Activate(std::move(strings));
        }

        void Controller::DeactivateInput()
        {
            input.Deactivate();
        }

        Controller::InputID Controller::InputPosition()
        {
            return input.CurrentPosition();
        }

        void Controller::InitializeImpl()
        {
            InitializeGui();
            shop = Manager()->FindSystem<Shop>();
        }

        void Controller::WorkImpl()
        {
            if (!currentDialogue)
                return;

            if (currentDialogue->script->IsRunning())
                currentDialogue->script->RunningForThis()->Resume();
            else
                Leave();
        }

        void Controller::InitializeGui()
        {
            const float width = 1024;
            root = Agui::System::CreateRoot("speech");
            textbox = Agui::Textbox::Factory(root, "textbox", Agui::RelativePosition(Agui::HorizontalAlignment::LEFT, Agui::VerticalAlignment::BOT), 0);
            textbox->CreateLayout<Agui::SequenceLayout>()->SetSelfModifySize(false);
            textbox->GetSprite()->color.Edit(255, 100, 255, 100);
            textbox->GetLayout()->ChangeDimensions(width, 200);

            nameOutput = &textbox->CreateText("name", 1, Agui::Text("", Agui::Text::Format(), *Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
            nameOutput->SetAutoCalcTextSize();

            mainOutput = &textbox->CreateText(
                "main",
                1,
                Agui::RelativePosition(Agui::Dimension(0, 30),
                Agui::Dimension(0, 0)), Agui::Size(width - 60, 1),
                Agui::Text("", Agui::Text::Format(),*Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
            mainOutput->SetAutoCalcTextHeight();

            input.Initialize(*textbox, *Manager());
        }

        Name Controller::NameFor(EntityReference entity)
        {
            return entity->RetrieveComponent<Entity::GeneralComponent>()->name;
        }

        LoggingSystem* Controller::FindLoggingSystem()
        {
            return Manager()->FindSystem<LoggingSystem>();
        }

        Controller::Input::Input() : cursor(nullptr), handlerTextbox(nullptr)
        {}

        void Controller::Input::Initialize(Agui::Textbox& textbox, ObjectManager& objectManager)
        {
            handlerTextbox = &textbox;
            cursor = ::Agui::Cursor::Factory(
                handlerTextbox,
                "cursor",
                Agui::RelativePosition(Agui::Dimension(0.0f, -20.0f), Agui::Dimension(), Agui::HorizontalAlignment::LEFT, Agui::VerticalAlignment::TOP),
                0,
                objectManager);
        }

        void Controller::Input::Activate(std::vector<String>&& strings)
        {
            InputID id = 0;
            for (auto& loop : strings)
            {
                auto &emplaced = entries.emplace(id, Entry(*handlerTextbox, ToString(id), loop)).first->second;
                cursor->AddEntry(emplaced.textboxConnection.GetText());
                ++id;
            }

            cursor->Show();
        }

        void Controller::Input::Deactivate()
        {
            cursor->Hide();
            cursor->ClearEntries();

            auto loop = entries.begin();
            while (loop != entries.end())
            {
                loop->second.textboxConnection.Destroy();
                loop = entries.erase(loop);
            }
        }

        Controller::InputID Controller::Input::CurrentPosition() const
        {
            return cursor->GetCurrentEntry();
        }

        Controller::Input::Entry::Entry(Agui::Textbox& textbox, const std::string& name, const std::string& string) :
            textboxConnection(textbox.CreateText(
                name,
                1,
                Agui::RelativePosition(Agui::Dimension(0, 60), Agui::Dimension()),
                Agui::Size(),
                Agui::Text(string, Agui::Text::Format(), *Agui::fontSlender, Agui::colorBlack)))
        {}
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Speech::Controller)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD_BASE(::Atmos::ObjectSystem);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::Speech::Controller, "SpeechController");
}