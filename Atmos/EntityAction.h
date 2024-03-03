#pragma once

#include "ScriptInstance.h"

#include "Serialization.h"

namespace Atmos::Entity
{
    class Action
    {
    public:
        enum class Activation : std::int32_t
        {
            EnterTile,
            AttemptEnterTile,
            UseOn,
            UseInto,
            EnterField,
            LeaveField
        };
    public:
        Activation activation;
        Arca::Index<Script::Instance> script;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Entity::Action::Activation, BinaryArchive> final :
        public EnumScribe<Atmos::Entity::Action::Activation, BinaryArchive>
    {};

    template<>
    class Scribe<Atmos::Entity::Action, BinaryArchive> final :
        public CompositeScribe<Atmos::Entity::Action, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}