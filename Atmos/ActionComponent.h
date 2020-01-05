#pragma once

#include "ScriptInstance.h"

namespace Atmos::Entity
{
    class ActionComponent
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
        Activation activation = Activation::UseOn;
        Script::ScriptInstance* script = nullptr;

        ActionComponent() = default;

        void FireMovedInto() const;
        void FireAttemptMovedInto() const;
        void FireUseOn() const;
        void FireUseInto() const;
        void FireFieldEntered() const;
        void FireFieldLeft() const;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Entity::ActionComponent>
    {
        static const ObjectType objectType = ObjectType::Shard;
        static inline const TypeName typeName = "ActionComponent";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Entity::ActionComponent, BinaryArchive> final
        : public ArcaNullScribe<Atmos::Entity::ActionComponent, BinaryArchive>
    {};
}