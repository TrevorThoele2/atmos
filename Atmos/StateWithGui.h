
#pragma once

#include "State.h"
#include "StateGui.h"

#include "Event.h"
#include <Chroma\Any.h>

namespace Atmos
{
    template<class Gui>
    class StateWithGui : public State
    {
    public:
        Gui gui;
    public:
        virtual ~StateWithGui() = 0;

        void Update();
    protected:
        StateWithGui(ObjectManager& manager);
        StateWithGui(const ::Inscription::Table<StateWithGui>& table);
    private:
        void InitializeImpl() override final;
        void WorkImpl() override final;
        virtual void DoInitialize();
        virtual void DoWork();
    private:
        void Show() override final;
        void Hide() override final;
    };

    template<class Gui>
    StateWithGui<Gui>::~StateWithGui()
    {}

    template<class Gui>
    void StateWithGui<Gui>::Update()
    {
        gui.Update();
    }

    template<class Gui>
    StateWithGui<Gui>::StateWithGui(ObjectManager& manager) : State(manager), gui(manager)
    {}

    template<class Gui>
    StateWithGui<Gui>::StateWithGui(const ::Inscription::Table<StateWithGui>& table) : INSCRIPTION_TABLE_GET_BASE(State)
    {}

    template<class Gui>
    void StateWithGui<Gui>::InitializeImpl()
    {
        DoInitialize();
        gui.Initialize();
    }

    template<class Gui>
    void StateWithGui<Gui>::WorkImpl()
    {
        DoWork();
        gui.Update();
    }

    template<class Gui>
    void StateWithGui<Gui>::DoInitialize()
    {}

    template<class Gui>
    void StateWithGui<Gui>::DoWork()
    {}

    template<class Gui>
    void StateWithGui<Gui>::Show()
    {
        gui.Show();
    }

    template<class Gui>
    void StateWithGui<Gui>::Hide()
    {
        gui.Hide();
    }
}