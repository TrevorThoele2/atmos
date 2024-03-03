
#pragma once

#include "StateWithGui.h"
#include "StateGui.h"

#include <AGUI/Resolution.h>
#include <AGUI/MenuButton.h>
#include <AGUI/Checkbox.h>
#include <AGUI/Fillbar.h>
#include <AGUI/Label.h>

#include "ObjectSerialization.h"

namespace Atmos
{
    class StateSystem;
    class InitializationFileSystem;
    class WindowSystem;
    class MasterSoundSystem;

    class OptionsStateGui : public StateGui
    {
    public:
        OptionsStateGui(ObjectManager& objectManager);
    private:
        class AttributeBase
        {
        public:
            AttributeBase(OptionsStateGui& owner);

            virtual ~AttributeBase() = 0;

            virtual void Accept() = 0;
            virtual void Decline() = 0;
        };

        friend AttributeBase;

        template<class T>
        class Attribute : public AttributeBase
        {
        public:
            Attribute(OptionsStateGui& owner);

            void Initialize(T set);
            void Accept() override;
            void Decline() override;

            void SetCurrent(T set);
            T GetCurrent() const;
        private:
            T previous, current;
        };
    private:
        std::vector<AttributeBase*> attributes;
        Attribute<float> masterSound;
        Attribute<bool> fullscreen;
        Attribute<const Agui::Resolution*> resolution;
    private:
        Agui::MenuButton* resolutionButton;
        Agui::MenuButton* ratioButton;
        Agui::Checkbox* fullscreenCheckbox;
        Agui::Fillbar* masterSoundFillbar;
        Agui::Label* masterSoundLabel;
    private:
        void DoInitialize() override;
        void DoShow() override;
        void Exit();
    private:
        void FillResolutionButton(const Agui::Resolution::AspectRatio& ratio);
        void SetMasterSoundLabelString();
    private:
        void OnFullscreenChanged(const Agui::Checkbox& args);
        void OnAspectRatioTextPressed(const Agui::Resolution::AspectRatio& ratio);
        void OnResolutionTextPressed(const Agui::Resolution& res);
        void OnMasterSoundPercentageChange(const Agui::Fillbar& args);

        void OnAcceptButtonPressed();
        void OnCancelButtonPressed();
    private:
        StateSystem* FindStateSystem();
        InitializationFileSystem* FindInitializationFileSystem();
        WindowSystem* FindWindowSystem();
        MasterSoundSystem* FindMasterSoundSystem();
    };

    template<class T>
    OptionsStateGui::Attribute<T>::Attribute(OptionsStateGui& owner) : AttributeBase(owner)
    {}

    template<class T>
    void OptionsStateGui::Attribute<T>::Initialize(T set)
    {
        current = previous = set;
    }

    template<class T>
    void OptionsStateGui::Attribute<T>::Accept()
    {
        previous = current;
    }

    template<class T>
    void OptionsStateGui::Attribute<T>::Decline()
    {
        current = previous;
    }

    template<class T>
    void OptionsStateGui::Attribute<T>::SetCurrent(T set)
    {
        current = set;
    }

    template<class T>
    T OptionsStateGui::Attribute<T>::GetCurrent() const
    {
        return current;
    }

    class OptionsState : public StateWithGui<OptionsStateGui>
    {};

    template<>
    struct ObjectTraits<OptionsState> : ObjectTraitsBase<OptionsState>
    {
        static const ObjectTypeName typeName;
        static constexpr ObjectTypeList<State> bases = {};
    };
}

namespace Inscription
{
    DECLARE_OBJECT_INSCRIPTER(::Atmos::OptionsState)
    {
    public:
        OBJECT_INSCRIPTER_DECLARE_MEMBERS;
    };
}