
#include "Speech.h"

#include "Shop.h"
#include "StringUtility.h"

#include "GeneralComponent.h"
#include "DialogueComponent.h"
#include "WorldManager.h"

#include "CurrentField.h"

#include "FontDefines.h"

#include "Logger.h"

#include <AGUI\System.h>

namespace Atmos
{
    namespace Speech
    {
        Handler::Input::Input() : cursor(nullptr), handlerTextbox(nullptr)
        {}

        void Handler::Input::Init(Agui::Textbox &textbox)
        {
            handlerTextbox = &textbox;
            cursor = CursorT::Factory(handlerTextbox, "cursor", Agui::RelativePosition(Agui::Dimension(0.0f, -20.0f), Agui::Dimension(), Agui::HorizontalAlignment::LEFT, Agui::VerticalAlignment::TOP), 0);
        }

        void Handler::Input::Activate(std::vector<String> &&strings)
        {
            ID id = 0;
            for (auto &loop : strings)
            {
                auto &emplaced = entries.emplace(id, Entry(*handlerTextbox, ToString(id), loop)).first->second;
                cursor->AddEntry(emplaced.textboxConnection.GetText());
                ++id;
            }

            cursor->Show();
        }

        void Handler::Input::Deactivate()
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

        Handler::Input::ID Handler::Input::GetCurrentPosition() const
        {
            return cursor->GetCurrentEntry();
        }

        Handler::Input::Entry::Entry(Agui::Textbox &textbox, const std::string &name, const std::string &string) : textboxConnection(textbox.CreateText(name, 1, Agui::RelativePosition(Agui::Dimension(0, 60), Agui::Dimension()), Agui::Size(), Agui::Text(string, Agui::Text::Format(), *Agui::fontSlender, Agui::colorBlack)))
        {}

        Handler Handler::instance;
        Agui::Root* Handler::root = nullptr;
        Agui::Textbox* Handler::textbox = nullptr;
        Agui::TextComponent* Handler::nameOutput = nullptr;
        Agui::TextComponent* Handler::mainOutput = nullptr;
        Ent::DialogueComponent* Handler::currentDialogue = nullptr;

        void Handler::InitGui()
        {
            const float width = 1024;
            root = Agui::System::CreateRoot("speech");
            textbox = Agui::Textbox::Factory(root, "textbox", Agui::RelativePosition(Agui::HorizontalAlignment::LEFT, Agui::VerticalAlignment::BOT), 0);
            textbox->CreateLayout<Agui::SequenceLayout>()->SetSelfModifySize(false);
            textbox->GetSprite()->color.Edit(255, 100, 255, 100);
            textbox->GetLayout()->ChangeDimensions(width, 200);

            nameOutput = &textbox->CreateText("name", 1, Agui::Text("", Agui::Text::Format(), *Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
            nameOutput->SetAutoCalcTextSize();

            mainOutput = &textbox->CreateText("main", 1, Agui::RelativePosition(Agui::Dimension(0, 30), Agui::Dimension(0, 0)), Agui::Size(width - 60, 1), Agui::Text("", Agui::Text::Format(), *Agui::fontSlender, Agui::Color(255, 0, 0, 0))).GetText();
            mainOutput->SetAutoCalcTextHeight();

            instance.input.Init(*textbox);
        }

        void Handler::OnActionPressed(const ::Atmos::Input::Action &args)
        {
            if (Shop::IsActive())
            {
                Shop::Instance().OnActionPressed(args);
                return;
            }
        }

        Handler& Handler::Instance()
        {
            return instance;
        }

        void Handler::Init()
        {
            InitGui();
            Environment::GetInput()->eventActionPressed.Subscribe(&Handler::OnActionPressed, instance);
            Shop::Init();
        }

        void Handler::Work()
        {
            if (currentDialogue)
            {
                if (!currentDialogue->script.IsExecuting())
                    Leave();
                else
                    currentDialogue->script.Resume();
            }
        }

        bool Handler::Enter(Ent::DialogueComponent &dialogue)
        {
            if (currentDialogue)
                return false;

            if (!dialogue.script.IsValid())
            {
                Logger::Log("The dialogue from an entity is empty.",
                    Logger::Type::ERROR_LOW,
                    Logger::NameValueVector{ NameValuePair("Entity", ToString(WorldManager::GetCurrentField()->entities.GetName(dialogue.GetOwnerEntity()).Get())) });
                return false;
            }

            currentDialogue = &dialogue;
            nameOutput->SetString(GetCurrentEntities()->GetName(dialogue.GetOwnerEntity()).Get());
            mainOutput->ClearString();
            root->Show();
            currentDialogue->script.ExecuteDeferred();
            return true;
        }

        void Handler::Leave()
        {
            if (currentDialogue)
                currentDialogue->script.ForceQuit();

            currentDialogue = nullptr;
            root->Hide();
        }

        bool Handler::IsWorking()
        {
            return currentDialogue != nullptr;
        }

        Entity Handler::GetEntityTalking()
        {
            if (!currentDialogue)
                return Ent::nullEntity;

            return currentDialogue->GetOwnerEntity();
        }

        void Handler::SetString(const std::string &str)
        {
            mainOutput->SetString(str);
        }

        void Handler::AppendString(const std::string &append)
        {
            mainOutput->AppendString(append);
        }

        void Handler::ClearString()
        {
            mainOutput->ClearString();
        }

        const std::string& Handler::GetOutput()
        {
            return mainOutput->GetString();
        }

        void Handler::ActivateInput(std::vector<std::string> &&strings)
        {
            instance.input.Activate(std::move(strings));
        }

        void Handler::DeactivateInput()
        {
            instance.input.Deactivate();
        }

        Handler::InputID Handler::GetInputPosition()
        {
            return instance.input.GetCurrentPosition();
        }
    }
}