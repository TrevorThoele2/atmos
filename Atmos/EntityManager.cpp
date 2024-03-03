
#include "EntityManager.h"

#include "EntityComponent.h"
#include "GeneralComponent.h"
#include "WorldManager.h"
#include "CurrentField.h"

#include "EntityNameSystem.h"

#include <Inscription\Scribe.h>
#include <Inscription\Const.h>

namespace Atmos
{
    namespace Ent
    {
        Manager::ComponentMapFactoryBase::ComponentMapFactoryBase()
        {
            GetComponentMapFactories().push_back(this);
        }

        Manager::ComponentMapFactoryVector& Manager::GetComponentMapFactories()
        {
            static ComponentMapFactoryVector vector;
            return vector;
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Manager)
        {
            if (scribe.IsOutput())
            {
                // Save
                scribe.Save(idSequencer);

                // Component maps
                ::Inscription::ContainerSize size(componentMaps.size());
                scribe.Save(size);

                for (auto &loop : componentMaps)
                {
                    scribe.Save(::Inscription::RemoveConst(loop.first));
                    loop.second->Serialize(scribe);
                }
            }
            else
            {
                // Load
                scribe.Load(idSequencer);

                // Component maps
                ::Inscription::ContainerSize size;
                scribe.Load(size);

                while (size-- > 0)
                {
                    ComponentTypeName typeName;
                    scribe.Load(typeName);

                    componentMaps.find(typeName)->second->Serialize(scribe);
                }
            }
        }

        Manager::ComponentMapContainer Manager::temporaryTransferComponents;

        void Manager::OnFinalizeField()
        {
            TransferFirst();
        }

        void Manager::OnBeforeFieldSet(Field &newField)
        {
            TransferSecond(newField.entities);
        }

        void Manager::CreateMapsFromFactories(ComponentMapContainer &componentMaps)
        {
            for (auto &loop : GetComponentMapFactories())
                loop->CreateMap(componentMaps);
        }

        void Manager::DestroyComponentsForEntity(Entity entity)
        {
            for (auto &loop : componentMaps)
                loop.second->DestroyComponent(entity);
        }

        void Manager::TransferFirst()
        {
            for (auto &loop : temporaryTransferComponents)
                loop.second->Clear();

            for (auto &loop : transferSet)
            {
                for (auto &mapLoop : componentMaps)
                    mapLoop.second->MoveComponentTemporary(*temporaryTransferComponents.find(mapLoop.first)->second, loop);
                idSequencer.IDRemoved(loop);
            }
        }

        void Manager::TransferSecond(Manager &to)
        {
            for (auto &loop : transferSet)
            {
                Entity newID = to.idSequencer.ReserveNext();
                temporaryTransferComponents.find(GeneralComponent::TypeName())->second->MoveComponent(*to.componentMaps.find(GeneralComponent::TypeName())->second, loop, newID);
                for (auto &mapLoop : temporaryTransferComponents)
                {
                    if (mapLoop.first == GeneralComponent::TypeName())
                        continue;

                    mapLoop.second->MoveComponent(*to.componentMaps.find(mapLoop.first)->second, loop, newID);
                }
            }

            for (auto &loop : temporaryTransferComponents)
                loop.second->Clear();
        }

        Event<Entity> Manager::eventEntityCreated;
        Event<Entity> Manager::eventBeforeEntityDestroyed;
        Event<Entity, Entity> Manager::eventEntityTransferred;

        Manager::Manager(Field &field) : FieldComponent(field)
        {
            CreateMapsFromFactories(componentMaps);
            if (temporaryTransferComponents.empty())
                CreateMapsFromFactories(temporaryTransferComponents);
        }

        Manager::Manager(Field &field, Manager &&arg) : FieldComponent(field), idSequencer(std::move(arg.idSequencer)), componentMaps(std::move(arg.componentMaps))
        {}

        Manager::~Manager()
        {
            for (auto &loop : idSequencer)
            {
                for(auto intervalLoop = loop.GetStart(); intervalLoop <= loop.GetEnd(); ++intervalLoop)
                    eventBeforeEntityDestroyed(intervalLoop);
            }
        }

        Manager& Manager::operator=(Manager &&arg)
        {
            idSequencer = std::move(arg.idSequencer);
            componentMaps = std::move(arg.componentMaps);
            return *this;
        }

        Entity Manager::CreateEntity(const Name &name, const GridPosition &position)
        {
            return CreateEntity(name, position, Direction());
        }

        Entity Manager::CreateEntity(const Name &name, const GridPosition &position, const Direction &direction)
        {
            if (!idSequencer.CanMakeAnother())
                return nullEntity;

            if (NameSystem::FindEntity(name) != nullEntity)
                return nullEntity;

            Entity entity = idSequencer.ReserveNext();
            CreateComponent<GeneralComponent>(entity, name, position, direction, nullptr);
            eventEntityCreated(entity);
            return entity;
        }

        void Manager::DestroyEntity(Entity entity)
        {
            if (entity == nullEntity)
                return;

            if (!idSequencer.IsIDTaken(entity))
                return;

            eventBeforeEntityDestroyed(entity);

            idSequencer.IDRemoved(entity);
            DestroyComponentsForEntity(entity);
        }

        bool Manager::CheckEntityExists(Entity entity) const
        {
            return idSequencer.IsIDTaken(entity);
        }

        void Manager::SignalTransfer(Entity entity, bool signal)
        {
            if (signal)
                transferSet.emplace(entity);
            else
                transferSet.erase(entity);
        }

        bool Manager::WillTransfer(Entity entity) const
        {
            return transferSet.find(entity) != transferSet.end();
        }

        Optional<Name> Manager::GetName(Entity entity) const
        {
            typedef Optional<Name> Ret;

            auto found = FindComponent<GeneralComponent>(entity);
            if (found)
                return Ret(found->name);
            else
                return Ret();
        }

        Optional<GridPosition> Manager::GetPosition(Entity entity) const
        {
            typedef Optional<GridPosition> Ret;
            auto found = FindComponent<GeneralComponent>(entity);
            if (found)
                return Ret(found->position);
            else
                return Ret();
        }

        Optional<Direction> Manager::GetDirection(Entity entity) const
        {
            typedef Optional<Direction> Ret;
            auto found = FindComponent<GeneralComponent>(entity);
            if (found)
                return Ret(found->direction);
            else
                return Ret();
        }

        std::pair<bool, ComponentBase&> Manager::CreateComponentFromName(Entity entity, const ComponentTypeName &typeName)
        {
            return componentMaps.find(typeName)->second->CreateComponent(entity);
        }

        void Manager::DestroyComponent(Entity entity, const ComponentTypeName &typeName)
        {
            componentMaps.find(typeName)->second->DestroyComponent(entity);
        }

        ComponentBase* Manager::FindComponent(Entity entity, const ComponentTypeName &typeName)
        {
            return componentMaps.find(typeName)->second->FindComponent(entity);
        }

        const ComponentBase* Manager::FindComponent(Entity entity, const ComponentTypeName &typeName) const
        {
            return componentMaps.find(typeName)->second->FindComponent(entity);
        }

        void Manager::Work()
        {}

        Manager::const_iterator Manager::begin() const
        {
            return idSequencer.begin();
        }

        Manager::const_iterator Manager::end() const
        {
            return idSequencer.end();
        }
    }
}