
#include <sstream>
#include "IniManager.h"

#include "Environment.h"
#include "Fps.h"
#include "MasterSoundHandler.h"
#include "Input.h"

#include "FileUtility.h"
#include "InputException.h"

#include <AGUI/System.h>

#include <Inscription/TextFile.h>

namespace Atmos
{
    namespace Ini
    {
        Manager::EntryBase::~EntryBase()
        {}

        Manager::EntrySection::EntrySection(String &&string, bool first) : string(std::move(string)), first(first)
        {}

        void Manager::EntrySection::SetByFileString(const String &set)
        {}

        void Manager::EntrySection::SetFromInternal()
        {}

        String Manager::EntrySection::GetFileString() const
        {
            if(first)
                return "<" + string + ">\n";
            else
                return "\n<" + string + ">\n";
        }

        bool Manager::EntrySection::IsSection() const
        {
            return true;
        }

        const String Manager::fileName("Initialization.ini");

        void Manager::Init()
        {
            entries.push_back(EntryPtr(new EntrySection("GRAPHICS", true)));
            CreateEntry<ID::WINDOWED>("Windowed",
                std::function<bool()>([]() { return Environment::GetFlag(Environment::Flag::WINDOWED); }))
                ->value = true;
            CreateEntry<ID::RESOLUTION>("Resolution",
                std::function<::Agui::Resolution::Size()>([]() { return ::Agui::System::GetCurrentResolution()->GetSize(); }))
                ->value = Traits<ID::RESOLUTION>::T(1024, 768);
            CreateEntry<ID::FPS>("FPS Limit",
                std::function<FpsHandler::FPS()>([]() { return FpsHandler::GetFpsLimit(); }))
                ->value = 0;
            CreateEntry<ID::VSYNC>("Vsync",
                std::function<bool()>([]() { return FpsHandler::GetVSync(); }))
                ->value = true;

            entries.push_back(EntryPtr(new EntrySection("SOUND", false)));
            CreateEntry<ID::MASTER_SOUND>("Master Sound",
                std::function<Volume()>([]() { return MasterSoundHandler::GetMasterVolume(); }))
                ->value = 0.0f;

            entries.push_back(EntryPtr(new EntrySection("CONTROLS", false)));

#define INPUT_ENTRY_MAKER(name, actionID, defaultKey)                                                                                                               \
CreateEntry<ID::MOVE_LEFT>(name,                                                                                                                                    \
    std::function<Input::KeyID()>([]() { return Environment::GetInput()->GetAction(actionID)->GetMappedKey()->AsKey()->id; }))    \
    ->value = defaultKey;

            INPUT_ENTRY_MAKER("Move Left", Input::ActionID::MOVE_LEFT, Input::KeyID::A);
            INPUT_ENTRY_MAKER("Move Up", Input::ActionID::MOVE_UP, Input::KeyID::W);
            INPUT_ENTRY_MAKER("Move Right", Input::ActionID::MOVE_RIGHT, Input::KeyID::D);
            INPUT_ENTRY_MAKER("Move Down", Input::ActionID::MOVE_DOWN, Input::KeyID::S);
            INPUT_ENTRY_MAKER("Use", Input::ActionID::USE, Input::KeyID::E);
            INPUT_ENTRY_MAKER("Menu Left", Input::ActionID::NAVIGATE_MENU_LEFT, Input::KeyID::LEFT_ARROW);
            INPUT_ENTRY_MAKER("Move Up", Input::ActionID::NAVIGATE_MENU_UP, Input::KeyID::UP_ARROW);
            INPUT_ENTRY_MAKER("Move Right", Input::ActionID::NAVIGATE_MENU_RIGHT, Input::KeyID::RIGHT_ARROW);
            INPUT_ENTRY_MAKER("Move Down", Input::ActionID::NAVIGATE_MENU_DOWN, Input::KeyID::DOWN_ARROW);
            INPUT_ENTRY_MAKER("Select Character Left", Input::ActionID::CHANGE_SELECTED_CHARACTER_LEFT, Input::KeyID::L_BRACKET);
            INPUT_ENTRY_MAKER("Select Character Right", Input::ActionID::CHANGE_SELECTED_CHARACTER_RIGHT, Input::KeyID::R_BRACKET);
            INPUT_ENTRY_MAKER("Inventory", Input::ActionID::INVENTORY, Input::KeyID::I);
            INPUT_ENTRY_MAKER("End Turn", Input::ActionID::END_TURN, Input::KeyID::END);
            INPUT_ENTRY_MAKER("Attack", Input::ActionID::ATTACK, Input::KeyID::Q);
            INPUT_ENTRY_MAKER("Spells", Input::ActionID::OPEN_SPELLS, Input::KeyID::O);
            INPUT_ENTRY_MAKER("Cancel", Input::ActionID::INVENTORY, Input::KeyID::C);
            INPUT_ENTRY_MAKER("Stats", Input::ActionID::STATS, Input::KeyID::U);

#undef INPUT_ENTRY_MAKER
        }

        void Manager::Save()
        {
            ::Inscription::TextOutFile file(::Atmos::Environment::GetFileSystem()->GetExePath().Append(fileName).GetValue());
            file.Flush();

            for (auto& loop : entries)
            {
                loop->SetFromInternal();
                file << loop->GetFileString();
            }
        }

        void Manager::Load()
        {
            auto filePath(::Atmos::Environment::GetFileSystem()->GetExePath().Append(fileName).GetValue());
            if (!DoesFileExist(filePath))
            {
                Save();
                return;
            }

            ::Inscription::TextInFile file(filePath);

            String string;
            file.GetLine(string);
            for (auto loop = entries.begin(); loop != entries.end();)
            {
                if (file.Eof())
                    break;

                if (string == "")
                {
                    file.GetLine(string);
                    continue;
                }

                if ((*loop)->IsSection())
                {
                    file.GetLine(string);
                    ++loop;
                    continue;
                }

                (*loop)->SetByFileString(string);
                file.GetLine(string);

                ++loop;
            }

            eventPostLoad();
        }

        Manager::EntryBase* Manager::GetEntry(ID id)
        {
            return entries.at(static_cast<size_t>(id)).get();
        }

        const Manager::EntryBase* Manager::GetEntry(ID id) const
        {
            return entries.at(static_cast<size_t>(id)).get();
        }

        std::vector<Manager::InputEntry*> Manager::GetInputTotal()
        {
            std::vector<InputEntry*> ret;
            typedef std::underlying_type<ID>::type UnderlyingT;
            const UnderlyingT startID = static_cast<UnderlyingT>(ID::MOVE_LEFT);
            const UnderlyingT endID = static_cast<UnderlyingT>(ID::STATS) + 1;

            for (UnderlyingT loop = startID; loop != endID; ++loop)
                ret.push_back(static_cast<InputEntry*>(entries[loop].get()));

            return ret;
        }
    }
}