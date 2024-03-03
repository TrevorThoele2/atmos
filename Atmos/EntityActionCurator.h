#pragma once

#include <Arca/Curator.h>
#include <Arca/RelicBatch.h>

#include "Entity.h"

namespace Atmos::Entity
{
    class ActionCurator final : public Arca::Curator
    {
    public:
        explicit ActionCurator(Init init);

        void Work();
    private:
        Arca::Batch<Entity> entities;

        template<Action::Activation MatchActivation>
        void Fire();
    };

    template<Action::Activation MatchActivation>
    void ActionCurator::Fire()
    {
        for (auto& entity : entities)
        {
            for (auto& action : entity.actions)
            {
                if (action.second.activation == MatchActivation)
                {
                    auto mutableScript = MutablePointer().Of(action.second.script);
                    mutableScript->ExecuteDeferred();
                }
            }
        }
    }
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Entity::ActionCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "Atmos::Entity::ActionCurator";
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::ActionCurator, BinaryArchive> final :
        public ArcaNullScribe<::Atmos::Entity::ActionCurator, BinaryArchive>
    {};
}