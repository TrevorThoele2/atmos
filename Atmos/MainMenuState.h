
#pragma once

#include "StateWithGui.h"
#include "StateGui.h"

#include "ObjectReference.h"
#include "AudioAsset.h"

#include "FilePath.h"

namespace Atmos
{
    class StateSystem;
    class EngineSystem;
    class WorldSystem;

    class MainMenuStateGui : public StateGui
    {
    public:
        MainMenuStateGui(ObjectManager& objectManager);
    private:
        void DoInitialize() override;
        void AddButton(const std::string& name, const std::string& label, const Agui::Event<Agui::Object>::Callback& clickedFunction);
        void OnNew();
        void OnLoad();
    private:
        StateSystem* FindStateSystem();
        EngineSystem* FindEngineSystem();
        WorldSystem* FindWorldSystem();
    };

    class FileSystem;
    class AudioSystem;
    class MusicSystem;

    class MainMenuState : public StateWithGui<MainMenuStateGui>
    {
    private:
        static const FileName mainMenuMusicName;
    private:
        TypedObjectReference<AudioAsset> music;

        void AddMainMenuMusic();
        void RemoveMainMenuMusic();

        FilePath MainMenuPath() const;
    private:
        void DoOnFocused() override;
        void DoOnUnfocused() override;
    private:
        const FileSystem* FindFileSystem() const;
        WorldSystem* FindWorldSystem();
        AudioSystem* FindAudioSystem();
        MusicSystem* FindMusicSystem();
    };

    template<>
    struct ObjectTraits<MainMenuState> : ObjectTraitsBase<MainMenuState>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<State> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::MainMenuState)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}