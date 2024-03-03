
#include "GameEnvironment.h"

#include "WorldManager.h"
#include "StateManager.h"
#include "FPS.h"
#include "ScriptController.h"
#include "Debug.h"
#include "Render.h"

#include "CurrentMusic.h"

#include "Sprite.h"
#include "ModulatorGeneratorCreator.h"

#include "ActionSystem.h"

namespace Atmos
{
    void GameEnvironment::OnInputPressed(const Input::Key &args)
    {
        if (args.id == Input::KeyID::F6)
            RenderHandler::Instance().flags.Flip(RenderFlags::DONT_DRAW);
    }

    void GameEnvironment::OnFocusLost()
    {
        CurrentMusic::Pause();
    }

    void GameEnvironment::OnFocusRegain()
    {
        CurrentMusic::Resume();
    }

    void GameEnvironment::Work()
    {
        DebugScreen::Logic().Start();

        Instance().modulatorController.Work();
        Instance().affecterSystem.Work();
        StateManager::Work();

        DebugScreen::Logic().Calc();
    }

    NullEvent GameEnvironment::eventFrameStart;
    NullEvent GameEnvironment::eventFrameStop;

    GameEnvironment& GameEnvironment::Instance()
    {
        static GameEnvironment instance;
        return instance;
    }

    void GameEnvironment::Init()
    {
        Environment::GetInput()->eventKeys.pressed.Subscribe(&GameEnvironment::OnInputPressed);

        // Modulators
        Modulator::CreateAllGenerators();

        // Entities
        Ent::SystemHandler::Init();

        // Field
        WorldManager::Init();
        WorldManager::LockIn();

        // Speech
        Speech::Handler::Init();
        // States
        StateManager::Init();
        // Scripts
        ScriptController::Instance();

        // Debug
        DebugScreen::Init();
        DebugScreen::Idle().Start();

        return;

        // Entity systems
        Ent::ActionSystem::Instance();
    }

    void GameEnvironment::Loop()
    {
        while (Environment::StartFrame())
        {
            if (Environment::IsFocused()) // Check for focus on the window 
            {
                // Handle focus regained
                if (Environment::GetFlag(Environment::Flag::FOCUS_LOST))
                    OnFocusRegain();

                // Profiling and FPS handling
                DebugScreen::Idle().Calc();
                eventFrameStart();
                FpsHandler::StartFrame();

                // Input, logic, and rendering
                Environment::GetInput()->Work();
                Work();
                RenderHandler::Render();

                // Lock in new stuff
                StateManager::LockIn();
                WorldManager::LockIn();

                eventFrameStop();
                // Update debugscreen and start the idle profiler
                DebugScreen::Update();
                DebugScreen::Idle().Start();

                // Handle fps idling
                while (FpsHandler::DoIdle())
                    Environment::Suspend(TimeValue(FixedPoint64(0), TimeValueEpoch::MILLISECONDS));
            }
            else // Window is not in focus
            {
                if (!Environment::GetFlag(Environment::Flag::FOCUS_LOST))
                    OnFocusLost();

                Environment::Suspend(TimeValue(FixedPoint64(1), TimeValueEpoch::MILLISECONDS));
            }
        }
    }

    void GameEnvironment::StopAll()
    {
        Instance().affecterSystem.StopAll(true);
    }

    void GameEnvironment::LoadWorld(const FilePath &path)
    {
        WorldManager::UseWorld(path);
    }

    void GameEnvironment::LoadStasis(const FilePath &path)
    {
        WorldManager::UseStasis(path);
    }

    Modulator::Controller& GameEnvironment::GetModulatorController()
    {
        return Instance().modulatorController;
    }

    void GameEnvironment::AddModulatorGenerator(const Name &name, std::unique_ptr<Modulator::GeneratorBase> &&add)
    {
        Instance().modulatorGenerators.emplace(name, std::move(add));
    }

    Modulator::Observer GameEnvironment::GenerateModulator(const Name &name)
    {
        auto &modulatorGenerators = Instance().modulatorGenerators;
        auto found = modulatorGenerators.find(name);
        if (found == modulatorGenerators.end())
            return Modulator::Observer();

        return found->second->Generate();
    }

    GameEnvironment::ModulatorTrackGenerateT GameEnvironment::GenerateModulatorTrack(const Name &modName, const Name &trackName)
    {
        auto &modulatorGenerators = Instance().modulatorGenerators;
        auto found = modulatorGenerators.find(modName);
        if (found == modulatorGenerators.end())
            return ModulatorTrackGenerateT();

        return found->second->GenerateTrack(trackName);
    }

    ::affecter::System& GameEnvironment::GetAffecterSystem()
    {
        return Instance().affecterSystem;
    }
}