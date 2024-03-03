#include "EntityCurator.h"

#include "CurrentActualizingEntity.h"
#include "Script.h"
#include "ExecuteScript.h"

#include <Arca/Reliquary.h>
#include <Arca/Created.h>
#include <Arca/Destroying.h>

namespace Atmos::Entity
{
    Curator::Curator(Init init) :
        Arca::Curator(init),
        prototypes(init.owner.Batch<Prototype>()),
        entities(init.owner.Batch<Entity>()),
        mapped(init.owner)
    {
        Owner().On<Arca::CreatedKnown<Entity>>(
            [this](const Arca::CreatedKnown<Entity>& signal)
            {
                const auto index = signal.index;

                auto mutableMappedEntities = MutablePointer().Of(mapped);
                AddEntityTo(mutableMappedEntities->nameToEntity, index->name, index);
                AddEntityTo(mutableMappedEntities->positionToEntity, index->position, index);
            });

        Owner().On<Arca::DestroyingKnown<Entity>>(
            [this](const Arca::DestroyingKnown<Entity>& signal)
            {
                const auto index = signal.index;

                auto mutableMappedEntities = MutablePointer().Of(mapped);
                RemoveEntityFrom(mutableMappedEntities->nameToEntity, index);
                RemoveEntityFrom(mutableMappedEntities->positionToEntity, index);
            });
    }

    void Curator::Handle(const Work&)
    {

    }

    void Curator::Handle(const ActualizeAllPrototypes&)
    {
        struct ToConstruct
        {
            Entity* entity;
            Arca::RelicID id;
            Arca::Index<Scripting::Script> constructor;
            ToConstruct(Entity* entity, Arca::RelicID id, Arca::Index<Scripting::Script> constructor) :
                entity(entity), id(id), constructor(constructor)
            {}
        };

        std::vector<ToConstruct> toConstruct;
        toConstruct.reserve(prototypes.Size());
        for (auto& prototype : prototypes)
        {
            auto entity = Owner().Do(
                Arca::Create<Entity>(prototype.name, "", prototype.position, prototype.direction, false));

            if (prototype.constructor)
                toConstruct.emplace_back(MutablePointer().Of(entity), entity.ID(), prototype.constructor);
        }

        auto currentActualizing = MutablePointer().Of<CurrentActualizing>();

        for(auto& currentToConstruct : toConstruct)
        {
            currentActualizing->entity = Arca::Index<Entity>(currentToConstruct.id, Owner());
            Owner().Do(Scripting::Execute{ currentToConstruct.constructor });
        }

        currentActualizing->entity = Arca::Index<Entity>();

        Owner().Do(Arca::Clear(Arca::TypeFor<Prototype>()));
    }

    Arca::Index<Entity> Curator::Handle(const FindByName& command)
    {
        const Arca::Index<Mapped> mappedEntities(Owner());
        auto& nameToEntity = mappedEntities->nameToEntity;
        const auto found = nameToEntity.find(command.name);
        if (found != nameToEntity.end())
            return found->second;
        else
            return {};
    }

    std::set<Arca::Index<Entity>> Curator::Handle(const FindByPosition& command)
    {
        const Arca::Index<Mapped> mappedEntities(Owner());
        auto& positionToEntity = mappedEntities->positionToEntity;
        const auto found = positionToEntity.find(command.position);
        if (found != positionToEntity.end())
            return found->second;
        else
            return {};
    }

    void Curator::Handle(const MoveTo& command)
    {
        auto mutableMappedEntities = MutablePointer().Of(mapped);

        auto& positionToEntity = mutableMappedEntities->positionToEntity;
        RemoveEntityFrom(positionToEntity, command.entity);

        auto mutableEntity = MutablePointer().Of(command.entity);
        mutableEntity->position = command.to;

        AddEntityTo(positionToEntity, command.to, command.entity);
    }

    void Curator::Handle(const ModifyData& command)
    {
        auto removeSet = std::set<String>(command.remove.begin(), command.remove.end());
        std::unordered_map<String, Variant> replaceMap;
        for (auto& datum : command.replace)
            replaceMap.emplace(datum.name, datum.value);

        auto mutableEntity = MutablePointer().Of(command.entity);

        for(auto element = mutableEntity->data.begin(); element != mutableEntity->data.end();)
        {
            if (removeSet.find(element->name) != removeSet.end())
                element = mutableEntity->data.erase(element);
            else
            {
                auto foundReplace = replaceMap.find(element->name);
                if (foundReplace != replaceMap.end())
                    element->value = foundReplace->second;
                ++element;
            }
        }
        mutableEntity->data.insert(mutableEntity->data.begin(), command.add.begin(), command.add.end());
    }

    void Curator::Handle(const ModifyTags& command)
    {
        auto removeSet = std::set<String>(command.remove.begin(), command.remove.end());

        auto mutableEntity = MutablePointer().Of(command.entity);

        for (auto element = mutableEntity->tags.begin(); element != mutableEntity->tags.end();)
        {
            if (removeSet.find(*element) != removeSet.end())
                element = mutableEntity->tags.erase(element);
            else
                ++element;
        }
        mutableEntity->tags.insert(mutableEntity->tags.begin(), command.add.begin(), command.add.end());
    }

    void Curator::AddEntityTo(Mapped::NameToEntity& to, const String& name, Arca::Index<Entity> entity)
    {
        to.emplace(name, entity);
    }

    void Curator::AddEntityTo(Mapped::PositionToEntity& to, const Spatial::Grid::Point& position, Arca::Index<Entity> entity)
    {
        auto value = to.find(position);
        if (value == to.end())
            value = to.emplace(position, std::set<Arca::Index<Entity>>{}).first;
        value->second.emplace(entity);
    }

    void Curator::RemoveEntityFrom(Mapped::NameToEntity& from, Arca::Index<Entity> entity)
    {
        from.erase(entity->name);
    }

    void Curator::RemoveEntityFrom(Mapped::PositionToEntity& from, Arca::Index<Entity> entity)
    {
        auto set = from.find(entity->position);
        set->second.erase(entity);
        if (set->second.empty())
            from.erase(set);
    }
}