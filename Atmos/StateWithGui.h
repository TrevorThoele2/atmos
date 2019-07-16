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
        StateWithGui(typename const ::Inscription::BinaryTableData<StateWithGui>& data);
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
    StateWithGui<Gui>::StateWithGui(typename const ::Inscription::BinaryTableData<StateWithGui>& data) :
        State(std::get<0>(data.bases))
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

namespace Inscription
{
    template<class Gui>
    struct TableData<::Atmos::StateWithGui<Gui>, BinaryArchive> :
        public ObjectTableDataBase<::Atmos::StateWithGui<Gui>, BinaryArchive>
    {};

    template<class Gui>
    class Scribe<::Atmos::StateWithGui<Gui>, BinaryArchive> :
        public ObjectScribe<::Atmos::StateWithGui<Gui>, BinaryArchive>
    {
    private:
        using BaseT = ObjectScribe<::Atmos::StateWithGui<Gui>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        using DataBase = typename BaseT::DataBase;
        using TableBase = typename BaseT::TableBase;
    public:
        class Table : public TableBase
        {};
    };
}