#pragma once

#include "Event.h"

#include "Input.h"

#include "ModulatorObserver.h"
#include "ModulatorGenerator.h"
#include "ModulatorController.h"
#include <Affecter\System.h>

#include "FilePath.h"

namespace Atmos
{
    class GameEnvironment
    {
    public:
        typedef Modulator::GeneratorBase::GenerateTrackPtrT ModulatorTrackGenerateT;
        typedef std::unique_ptr<Modulator::GeneratorBase> ModulatorGeneratorPtr;
    private:
        GameEnvironment() = default;

        std::unordered_map<Name, ModulatorGeneratorPtr> modulatorGenerators;
        Modulator::Controller modulatorController;

        ::affecter::System affecterSystem;

        static void OnInputPressed(const Input::Key &args);
        static void OnFocusLost();
        static void OnFocusRegain();

        static void Work();
    public:
        static NullEvent eventFrameStart;
        static NullEvent eventFrameStop;

        static GameEnvironment& Instance();

        static void Init();
        static void Loop();
        static void StopAll();

        static void LoadWorld(const FilePath &path);
        static void LoadStasis(const FilePath &path);

        static void AddModulatorGenerator(const Name &name, std::unique_ptr<Modulator::GeneratorBase> &&add);
        static Modulator::Observer GenerateModulator(const Name &name);
        static ModulatorTrackGenerateT GenerateModulatorTrack(const Name &modName, const Name &trackName);
        static Modulator::Controller::ID AttachModulator(const Modulator::Observer &attach);
        static void DetachModulator(const Modulator::Observer &detach);
        static void DetachModulator(const Modulator::ModulatorBase &detach);
        static Modulator::Observer FindModulator(Modulator::Controller::ID find);
        static bool IsModulatorWorkingObject(void *obj);

        static ::affecter::System& GetAffecterSystem();
    };
}