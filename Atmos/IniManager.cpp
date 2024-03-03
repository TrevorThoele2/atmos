
#include <sstream>
#include "IniManager.h"

// Components
#include "Environment.h"
#include "Fps.h"
#include "MasterSoundHandler.h"
#include "Input.h"

#include "FileUtility.h"
#include "InputException.h"

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
            entries.reserve(22);

            entries.push_back(EntryPtr(new EntrySection("GRAPHICS", true)));
            CreateEntry<ID::WINDOWED>("Windowed")->value = true;
            CreateEntry<ID::RESOLUTION>("Resolution")->value = Traits<ID::RESOLUTION>::T(1024, 768);
            CreateEntry<ID::FPS>("FPS Limit")->value = 0;
            CreateEntry<ID::VSYNC>("Vsync")->value = true;

            entries.push_back(EntryPtr(new EntrySection("SOUND", false)));
            CreateEntry<ID::MASTER_SOUND>("Master Sound")->value = 0.0f;

            entries.push_back(EntryPtr(new EntrySection("CONTROLS", false)));
            CreateEntry<ID::MOVE_LEFT>("Move Left")->value = ::Atmos::Input::KeyID::A;
            CreateEntry<ID::MOVE_UP>("Move Up")->value = ::Atmos::Input::KeyID::W;
            CreateEntry<ID::MOVE_RIGHT>("Move Right")->value = ::Atmos::Input::KeyID::D;
            CreateEntry<ID::MOVE_DOWN>("Move Down")->value = ::Atmos::Input::KeyID::S;
            CreateEntry<ID::USE>("Use")->value = ::Atmos::Input::KeyID::E;
            CreateEntry<ID::MENU_LEFT>("Menu Left")->value = ::Atmos::Input::KeyID::LEFT_ARROW;
            CreateEntry<ID::MENU_UP>("Menu Up")->value = ::Atmos::Input::KeyID::UP_ARROW;
            CreateEntry<ID::MENU_RIGHT>("Menu Right")->value = ::Atmos::Input::KeyID::RIGHT_ARROW;
            CreateEntry<ID::MENU_DOWN>("Menu Down")->value = ::Atmos::Input::KeyID::DOWN_ARROW;
            CreateEntry<ID::SELECT_CHARACTER_LEFT>("Select Character Left")->value = ::Atmos::Input::KeyID::L_BRACKET;
            CreateEntry<ID::SELECT_CHARACTER_RIGHT>("Select Character Right")->value = ::Atmos::Input::KeyID::R_BRACKET;
            CreateEntry<ID::INVENTORY>("Inventory")->value = ::Atmos::Input::KeyID::I;
            CreateEntry<ID::END_TURN>("End Turn")->value = ::Atmos::Input::KeyID::END;
            CreateEntry<ID::ATTACK>("Attack")->value = ::Atmos::Input::KeyID::Q;
            CreateEntry<ID::SPELLS>("Spells")->value = ::Atmos::Input::KeyID::O;
            CreateEntry<ID::CANCEL>("Cancel")->value = ::Atmos::Input::KeyID::C;
            CreateEntry<ID::STATS>("Stats")->value = ::Atmos::Input::KeyID::U;
        }

        void Manager::Save()
        {
            ::inscription::TextOutFile file(::Atmos::Environment::GetFileSystem()->GetExePath().Append(fileName).GetValue());
            file.Flush();

            for (auto &loop : entries)
                file << loop->GetFileString();
        }

        void Manager::Load()
        {
            auto filePath(::Atmos::Environment::GetFileSystem()->GetExePath().Append(fileName).GetValue());
            if (DoesFileExist(filePath))
            {
                Save();
                return;
            }

            ::inscription::TextInFile file(filePath);

            String string;
            file.GetLine(string);
            for (auto &loop : entries)
            {
                if (string == "")
                {
                    if (loop->IsSection() && !static_cast<EntrySection*>(loop.get())->first)
                        file.GetLine(string);
                    else
                        // Invalid input
                        throw InputException();
                }

                loop->SetByFileString(string);
                file.GetLine(string);
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

        /*
        const FileName Handler::FILE_NAME = "DatIni.ini";

        Handler::Section::Section(const String &header) : header(header)
        {}

        void Handler::Section::Save(String &string)
        {
            string.append(header + "\n");
            for (auto &loop : lines)
                string.append(loop->GetString() + "\n");
        }

        void Handler::Section::Load(const String &header, ::inscription::TextInFile &file)
        {
            if (header != this->header)
                return;

            String string;
            for (auto &loop : lines)
            {
                file.GetLine(string);
                loop->Load(string);
            }
        }

        Handler::Section::Section(Section &&arg) : header(arg.header), lines(std::move(arg.lines))
        {}

        void Handler::CreateSections()
        {
            sections.reserve(3);

            // Graphics
            {
                auto section = CreateSection("<GRAPHICS>");
                section->CreateLine<bool>("Windowed", ini::Environment::windowed, std::bind(::Atmos::Environment::GetFlag, ::Atmos::Environment::Flag::WINDOWED));
                section->CreateLine("Resolution", ini::GuiSystem::resolution, [&]() { return agui::System::GetCurrentRes()->GetSize(); });
                section->CreateLine("FPS", ini::FpsHandler::fpsLimit, &::Atmos::FpsHandler::GetFpsLimit);
                section->CreateLine("Vsync", ini::FpsHandler::vsync, &::Atmos::FpsHandler::GetVSync);
            }

            // Sound
            {
                auto section = CreateSection("<SOUND>");
                section->CreateLine("Master sound", MasterSoundHandler::volume, &::Atmos::MasterSoundHandler::GetMasterVolume);
            }

            // Controls
            {
                auto section = CreateSection("<CONTROLS>");
                Input::keys.resize(::Atmos::Environment::GetInput()->GetActions().size());

                auto iniLoop = Input::keys.begin();
                for (auto &actionLoop : ::Atmos::Environment::GetInput()->GetActions())
                {
                    *iniLoop = (section->CreateLine<Input::Key>(actionLoop->iniName, *iniLoop, std::bind([&](::Atmos::Input::ActionID id)
                    {
                        return Input::Key(::Atmos::Environment::GetInput()->GetAction(id)->GetMappedKey()->displayName, id);
                    }, actionLoop->id)));

                    (*iniLoop)->variable.actionID = actionLoop->id;

                    ++iniLoop;
                }
            }

            Environment::Init();
            GuiSystem::Init();
            FpsHandler::Init();
            MasterSoundHandler::Init();
            Input::Init();
        }

        agui::Resolution::Size Handler::IniFromString(const String &str, const Section::LineDerived<agui::Resolution::Size> &line)
        {
            std::istringstream stream(str);
            String string;
            stream >> string;

            auto pos = string.find_first_of("x");

            char *check1 = nullptr;
            auto first = static_cast<unsigned short>(strtol(string.substr(0, pos).c_str(), &check1, 10));
            char *check2 = nullptr;
            auto second = static_cast<unsigned short>(strtol(string.substr(pos + 1).c_str(), &check2, 10));
            if (*check1 || *check2)
                return line.variable;
            else
                return agui::Resolution::Size(first, second);
        }

        Input::Key Handler::IniFromString(const String &str, const Section::LineDerived<Input::Key> &line)
        {
            return Input::Key(str, line.variable.actionID);
        }

        Handler& Handler::Instance()
        {
            static Handler instance;
            return instance;
        }

        Handler::Section* Handler::CreateSection(const String &header)
        {
            Instance().sections.push_back(Section(header));
            return &Instance().sections.back();
        }

        void Handler::Save()
        {
            ::inscription::TextOutFile file(::Atmos::Environment::GetFileSystem()->GetExePath().Append(FILE_NAME).GetValue());
            file.Flush();

            String string;
            for (auto &loop : Instance().sections)
            {
                loop.Save(string);
                string.append("\n");
            }

            file << string;
        }

        void Handler::Load()
        {
            Instance().CreateSections();

            ::inscription::TextInFile file(::Atmos::Environment::GetFileSystem()->GetExePath().Append(FILE_NAME).GetValue());
            String header;
            file.GetLine(header);

            for (auto &loop : Instance().sections)
            {
                loop.Load(header, file);

                // There's a breakline here, so need to get two lines down
                file.GetLine(header);
                file.GetLine(header);
            }

            Instance().eventPostLoad();
        }

        Line<::agui::Resolution::Size>* Component<::agui::System>::resolution;
        void Component<::agui::System>::Init()
        {
            resolution->variable = agui::Resolution::Size(1024, 768);
        }
        */
    }
}