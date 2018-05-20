
#include "GameEnvironment.h"

#include "WorldManager.h"
#include "StateManager.h"
#include "FPS.h"
#include "ScriptController.h"
#include "Debug.h"
#include "Render.h"

#include "CurrentMusic.h"

#include "Sprite.h"
#include "ModulatorTrack.h"
#include "ModulatorDescribers.h"

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
        {
            auto &modulatorGenerators = Instance().modulatorGenerators;

#define CREATE_GENERATOR(describer) static_cast<Modulator::Generator<decltype(describer)::Type>*>(modulatorGenerators.emplace(describer.name, ModulatorGeneratorPtr(new Modulator::Generator<decltype(describer)::Type>(describer.name))).first->second.get())

#define START_TRACK_GENERATOR(describer, objectType)                    \
gen->AddTrackGenerator(describer.name, describer.variantType,                                \
    Modulator::Track<objectType>::ModifierT(static_cast<void(*)(const Modulator::TrackModifierArgs<objectType>&)>([](const Modulator::TrackModifierArgs<objectType> &args)
#define END_MODIFIER(objectType) \
)), Modulator::Track<objectType>::GetCurrentValueT(static_cast<Modulator::Value(*)(const objectType&)>([](const objectType &object)

#define END_TRACK_GENERATOR \
)))

            // Sprite generator
            {
                auto gen = CREATE_GENERATOR(Modulator::Description::Sprite);
                // Position 3D X
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionX, Sprite)
                {
                    args.object.SetX(static_cast<Sprite::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Sprite) { return Modulator::Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position 3D Y
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionY, Sprite)
                {
                    args.object.SetY(static_cast<Sprite::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Sprite) { return Modulator::Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position 3D Z
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionZ, Sprite)
                {
                    args.object.SetZ(static_cast<Sprite::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Sprite) { return Modulator::Value(object.GetZ()); }
                END_TRACK_GENERATOR;

                // Scaling X
                START_TRACK_GENERATOR(Modulator::Description::Track::ScalingX, Sprite)
                {
                    args.object.SetXScaler(static_cast<Size3D::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Sprite) { return Modulator::Value(object.GetXScaler()); }
                END_TRACK_GENERATOR;

                // Scaling Y
                START_TRACK_GENERATOR(Modulator::Description::Track::ScalingY, Sprite)
                {
                    args.object.SetXScaler(static_cast<Size3D::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Sprite) { return Modulator::Value(object.GetYScaler()); }
                END_TRACK_GENERATOR;

                // Scaling Z
                START_TRACK_GENERATOR(Modulator::Description::Track::ScalingZ, Sprite)
                {
                    args.object.SetXScaler(static_cast<Size3D::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Sprite) { return Modulator::Value(object.GetZScaler()); }
                END_TRACK_GENERATOR;

                // Rotation X
                START_TRACK_GENERATOR(Modulator::Description::Track::RotationX, Sprite)
                {
                    args.object.SetXRotation(Angle(Angle::RADIANS, static_cast<Angle::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta)))));
                }
                END_MODIFIER(Sprite) { return Modulator::Value(object.GetXRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Rotation Y
                START_TRACK_GENERATOR(Modulator::Description::Track::RotationY, Sprite)
                {
                    args.object.SetYRotation(Angle(Angle::RADIANS, static_cast<Angle::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta)))));
                }
                END_MODIFIER(Sprite) { return Modulator::Value(object.GetXRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Rotation Z
                START_TRACK_GENERATOR(Modulator::Description::Track::RotationZ, Sprite)
                {
                    args.object.SetZRotation(Angle(Angle::RADIANS, static_cast<Angle::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta)))));
                }
                END_MODIFIER(Sprite) { return Modulator::Value(object.GetZRotation().AsRadians()); }
                END_TRACK_GENERATOR;

                // Index
                START_TRACK_GENERATOR(Modulator::Description::Track::Index, Sprite)
                {
                    args.object.SetIndex(static_cast<Sprite::Index>((Modulator::Delta(args.ending.AsInt()) * args.delta) + (Modulator::Delta(args.starting.AsInt()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Sprite) { return Modulator::Value(std::int64_t(object.GetIndex())); }
                END_TRACK_GENERATOR;
            }

            // Sound generator
            {
                auto gen = CREATE_GENERATOR(Modulator::Description::Sound);
                // Position 3D X
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionX, Sound)
                {
                    args.object.SetX(static_cast<Sound::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Sound) { return Modulator::Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position 3D Y
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionY, Sound)
                {
                    args.object.SetY(static_cast<Sound::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Sound) { return Modulator::Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position 3D Z
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionZ, Sound)
                {
                    args.object.SetZ(static_cast<Sound::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Sound) { return Modulator::Value(object.GetZ()); }
                END_TRACK_GENERATOR;

                // Volume
                START_TRACK_GENERATOR(Modulator::Description::Track::Volume, Sound)
                {
                    args.object.SetBaseVolume(static_cast<::Atmos::Sound::Volume>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Sound) { return Modulator::Value(object.GetBaseVolume()); }
                END_TRACK_GENERATOR;
            }

            // AVEffect generator
            {
                auto gen = CREATE_GENERATOR(Modulator::Description::AVEffect);
                // Position 3D X
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionX, AVEffect)
                {
                    args.object.SetX(static_cast<Sprite::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(AVEffect) { return Modulator::Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position 3D Y
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionY, AVEffect)
                {
                    args.object.SetY(static_cast<Sprite::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(AVEffect) { return Modulator::Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position 3D Z
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionZ, AVEffect)
                {
                    args.object.SetZ(static_cast<Sprite::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(AVEffect) { return Modulator::Value(object.GetZ()); }
                END_TRACK_GENERATOR;
            }

            // Sense component generator
            {
                auto gen = CREATE_GENERATOR(Modulator::Description::SenseComponent);
                // Position 3D X
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionX, Ent::SenseComponent)
                {
                    args.object.SetX(static_cast<Sprite::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Ent::SenseComponent) { return Modulator::Value(object.GetX()); }
                END_TRACK_GENERATOR;

                // Position 3D Y
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionY, Ent::SenseComponent)
                {
                    args.object.SetY(static_cast<Sprite::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Ent::SenseComponent) { return Modulator::Value(object.GetY()); }
                END_TRACK_GENERATOR;

                // Position 3D Z
                START_TRACK_GENERATOR(Modulator::Description::Track::PositionZ, Ent::SenseComponent)
                {
                    args.object.SetZ(static_cast<Sprite::PositionT::ValueT>((Modulator::Delta(args.ending.AsFloat()) * args.delta) + (Modulator::Delta(args.starting.AsFloat()) * (Modulator::Delta(1) - args.delta))));
                }
                END_MODIFIER(Ent::SenseComponent){ return Modulator::Value(object.GetZ()); }
                END_TRACK_GENERATOR;
            }

#undef CREATE_GENERATOR
#undef START_TRACK_GENERATOR
#undef END_TRACK_GENERATOR

        }

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

    Modulator::Controller::ID GameEnvironment::AttachModulator(const Modulator::Observer &attach)
    {
        return Instance().modulatorController.Attach(attach);
    }

    void GameEnvironment::DetachModulator(const Modulator::Observer &detach)
    {
        Instance().modulatorController.Detach(detach);
    }

    void GameEnvironment::DetachModulator(const Modulator::ModulatorBase &detach)
    {
        Instance().modulatorController.Detach(detach);
    }

    Modulator::Observer GameEnvironment::FindModulator(Modulator::Controller::ID find)
    {
        return Instance().modulatorController.Find(find);
    }

    ::affecter::System& GameEnvironment::GetAffecterSystem()
    {
        return Instance().affecterSystem;
    }
}