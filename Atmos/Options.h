
#pragma once

#include "State.h"

#include <AGUI\Resolution.h>
#include <AGUI\MenuButton.h>
#include <AGUI\Checkbox.h>
#include <AGUI\Fillbar.h>
#include <AGUI\Label.h>

namespace Atmos
{
    class OptionsScreenGui : public StateGui
    {
    private:
        class AttributeBase
        {
        public:
            AttributeBase();
            virtual ~AttributeBase() = 0 {}
            virtual void Accept() = 0;
            virtual void Decline() = 0;
        };

        template<class T>
        class Attribute : public AttributeBase
        {
        private:
            T prev, current;
        public:
            void Init(T set);
            void Accept() override;
            void Decline() override;

            void SetCurrent(T set);
            T GetCurrent() const;
        };
    private:
        std::vector<AttributeBase*> attributes;
        Attribute<float> masterSound;
        Attribute<bool> fullscreen;
        Attribute<const Agui::Resolution*> resolution;

        Agui::MenuButton *resButton, *ratioButton;
        Agui::Checkbox *fullscreenCheckbox;
        Agui::Fillbar *masterSoundFillbar;
        Agui::Label *masterSoundLabel;

        void InitImpl() override;
        void ShowImpl() override;
        void Exit();

        void OnFullscreenChanged(const Agui::Checkbox &args);
        void OnAspectRatioTextPressed(const Agui::Resolution::AspectRatio &ratio);
        void OnResolutionTextPressed(const Agui::Resolution &res);
        void OnMasterSoundPercentageChange(const Agui::Fillbar &args);

        void OnAcceptButtonPressed();
        void OnCancelButtonPressed();

        void FillResButton(const Agui::Resolution::AspectRatio &ratio);
        void SetMasterSoundLabelString();
    public:
        OptionsScreenGui();
        friend AttributeBase;
    };

    template<class T>
    void OptionsScreenGui::Attribute<T>::Init(T set)
    {
        current = prev = set;
    }

    template<class T>
    void OptionsScreenGui::Attribute<T>::Accept()
    {
        prev = current;
    }

    template<class T>
    void OptionsScreenGui::Attribute<T>::Decline()
    {
        current = prev;
    }

    template<class T>
    void OptionsScreenGui::Attribute<T>::SetCurrent(T set)
    {
        current = set;
    }

    template<class T>
    T OptionsScreenGui::Attribute<T>::GetCurrent() const
    {
        return current;
    }

    typedef State<OptionsScreenGui> OptionsScreen;
    extern OptionsScreen optionsScreen;
}