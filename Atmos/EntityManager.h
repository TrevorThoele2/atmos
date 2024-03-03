#pragma once

#include <unordered_map>
#include <memory>

#include "FieldComponent.h"

#include "Entity.h"
#include "Name.h"
#include "EntityComponentBase.h"
#include "ComponentTypeName.h"
#include "EntityNameSystem.h"

#include "GridPosition.h"
#include "IDSequencer.h"

#include "Event.h"
#include "Optional.h"

#include "EntityUtility.h"

#include <Function\Iterate.h>

#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class ComponentBase;
        template<class ComponentT>
        class ComponentMap;

        class Manager : public FieldComponent
        {
        public:
            typedef IDSequencerStatic<Entity::ValueT, 1> IDSequencerT;
            typedef typename IDSequencerT::const_iterator const_iterator;
        private:
            class ComponentMapBase
            {
            public:
                virtual ~ComponentMapBase() = 0 {}
                virtual std::pair<bool, ComponentBase&> CreateComponent(Entity entity) = 0;
                virtual void MoveComponent(ComponentMapBase &moveTo, Entity entity, Entity newID) = 0;
                // Doesn't emit any events
                virtual void MoveComponentTemporary(ComponentMapBase &moveTo, Entity entity) = 0;
                virtual ComponentBase* FindComponent(Entity entity) = 0;
                virtual const ComponentBase* FindComponent(Entity entity) const = 0;
                virtual void DestroyComponent(Entity entity) = 0;
                virtual void Clear() = 0;
                virtual const ComponentTypeName& GetTypeName() const = 0;

                virtual void Serialize(inscription::Scribe &scribe) = 0;
            };
        protected:
            typedef std::unique_ptr<ComponentMapBase> ComponentMapPtr;
            typedef std::unordered_map<ComponentTypeName, ComponentMapPtr> ComponentMapContainer;
        private:
            template<class ComponentT>
            class ComponentMapDerived : public ComponentMapBase
            {
            public:
                typedef ComponentMap<ComponentT> ComponentMapT;
                ComponentMapT map;

                ComponentMapDerived() = default;
                std::pair<bool, ComponentBase&> CreateComponent(Entity entity) override;
                template<class... Args>
                std::pair<bool, ComponentT&> CreateComponent(Entity entity, Args && ... args);
                void MoveComponent(ComponentMapBase &moveTo, Entity entity, Entity newID) override;
                void MoveComponentTemporary(ComponentMapBase &moveTo, Entity entity) override;
                ComponentT* FindComponent(Entity entity) override;
                const ComponentT* FindComponent(Entity entity) const override;
                void DestroyComponent(Entity entity) override;
                void Clear() override;
                const ComponentTypeName& GetTypeName() const override;

                void Serialize(inscription::Scribe &scribe) override;
            };

            class ComponentMapFactoryBase
            {
            public:
                ComponentMapFactoryBase();
                virtual ~ComponentMapFactoryBase() = 0 {}
                virtual void CreateMap(ComponentMapContainer &container) const = 0;
            };

            template<class ComponentT>
            class ComponentMapFactoryDerived : public ComponentMapFactoryBase
            {
            public:
                static ComponentMapFactoryDerived instance;
                void CreateMap(ComponentMapContainer &container) const override;
            };

            typedef std::vector<ComponentMapFactoryBase*> ComponentMapFactoryVector;
            static ComponentMapFactoryVector& GetComponentMapFactories();

            template<std::size_t I>
            struct FillTuple
            {
                template<class... ComponentTs>
                static void Do(ComponentMapContainer &maps, Entity entity, std::tuple<ComponentTs...> &tuple)
                {
                    typedef std::tuple<ComponentTs...> TupleT;
                    typedef typename std::remove_pointer<typename std::tuple_element<I, TupleT>::type>::type PieceT;

                    std::get<I>(tuple) = static_cast<ComponentMapDerived<PieceT>*>(maps.find(PieceT::TypeName())->second.get())->FindComponent(entity);
                }
            };

            typedef std::unordered_set<Entity> TransferSet;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            IDSequencerT idSequencer;
            ComponentMapContainer componentMaps;

            TransferSet transferSet;
            // Holds the components to transfer between the finalization of field and before the field is set
            static ComponentMapContainer temporaryTransferComponents;

            void OnFinalizeField() override;
            void OnBeforeFieldSet(Field &newField) override;

            static void CreateMapsFromFactories(ComponentMapContainer &componentMaps);
            void DestroyComponentsForEntity(Entity entity);

            // Removes all of the IDs
            // Moves the components to the temporaryTransferComponents
            void TransferFirst();
            void TransferSecond(Manager &to);
        public:
            static Event<Entity> eventEntityCreated;
            static Event<Entity> eventBeforeEntityDestroyed;
            // First: old. Second: new.
            static Event<Entity, Entity> eventEntityTransferred;

            Manager(Field &field);
            Manager(Field &field, Manager &&arg);
            ~Manager();
            Manager& operator=(Manager &&arg);

            // This will return nullEntity if this cannot make another or if the name is not unique
            Entity CreateEntity(const Name &name, const GridPosition &position);
            // This will return nullEntity if this cannot make another or if the name is not unique
            Entity CreateEntity(const Name &name, const GridPosition &position, const Direction &direction);
            void DestroyEntity(Entity entity);
            bool CheckEntityExists(Entity entity) const;

            void SignalTransfer(Entity entity, bool signal = true);
            bool WillTransfer(Entity entity) const;

            // This cannot be used to modify the entity's name
            Optional<Name> GetName(Entity entity) const;
            // This cannot be used to modify the entity's position
            Optional<GridPosition> GetPosition(Entity entity) const;
            // This cannot be used to modify the entity's direction
            Optional<Direction> GetDirection(Entity entity) const;

            std::pair<bool, ComponentBase&> CreateComponentFromName(Entity entity, const ComponentTypeName &typeName);
            template<class ComponentT>
            std::pair<bool, ComponentT&> CreateComponent(Entity entity);
            template<class ComponentT>
            std::pair<bool, ComponentT&> CreateComponent(Entity entity, const ComponentT &arg);
            template<class ComponentT>
            std::pair<bool, ComponentT&> CreateComponent(Entity entity, ComponentT &&arg);
            template<class ComponentT, class... Args>
            std::pair<bool, ComponentT&> CreateComponent(Entity entity, Args && ... args);
            template<class ComponentT>
            void DestroyComponent(Entity entity);
            void DestroyComponent(Entity entity, const ComponentTypeName &typeName);
            template<class ComponentT>
            ComponentT* FindComponent(Entity entity);
            template<class ComponentT>
            const ComponentT* FindComponent(Entity entity) const;
            ComponentBase* FindComponent(Entity entity, const ComponentTypeName &typeName);
            const ComponentBase* FindComponent(Entity entity, const ComponentTypeName &typeName) const;
            template<class... ComponentTs>
            MultipleComponents<ComponentTs...> FindMultipleComponents(Entity entity);

            template<class ComponentT>
            typename ComponentT::Map& GetMap();
            template<class ComponentT>
            typename ComponentT::Map::ContainerT& GetRawMap();
            template<class ComponentT>
            static void AddType();

            void Work();

            const_iterator begin() const;
            const_iterator end() const;
        };

        template<class ComponentT>
        Manager::ComponentMapFactoryDerived<ComponentT> Manager::ComponentMapFactoryDerived<ComponentT>::instance;

        template<class ComponentT>
        void Manager::ComponentMapFactoryDerived<ComponentT>::CreateMap(ComponentMapContainer &container) const
        {
            container.emplace(ComponentT::TypeName(), ComponentMapPtr(new ComponentMapDerived<ComponentT>()));
        }

        template<class ComponentT>
        std::pair<bool, ComponentBase&> Manager::ComponentMapDerived<ComponentT>::CreateComponent(Entity entity)
        {
            return map.Create(entity);
        }

        template<class ComponentT>
        template<class... Args>
        std::pair<bool, ComponentT&> Manager::ComponentMapDerived<ComponentT>::CreateComponent(Entity entity, Args && ... args)
        {
            return map.Create(entity, std::forward<Args>(args)...);
        }

        template<class ComponentT>
        void Manager::ComponentMapDerived<ComponentT>::MoveComponent(ComponentMapBase &moveTo, Entity entity, Entity newID)
        {
            if (moveTo.GetTypeName() != GetTypeName())
                return;

            map.Move(static_cast<ComponentMapDerived<ComponentT>&>(moveTo).map, entity, newID);
        }

        template<class ComponentT>
        void Manager::ComponentMapDerived<ComponentT>::MoveComponentTemporary(ComponentMapBase &moveTo, Entity entity)
        {
            if (moveTo.GetTypeName() != GetTypeName())
                return;

            map.MoveTemporary(static_cast<ComponentMapDerived<ComponentT>&>(moveTo).map, entity);
        }

        template<class ComponentT>
        ComponentT* Manager::ComponentMapDerived<ComponentT>::FindComponent(Entity entity)
        {
            return map.Find(entity);
        }

        template<class ComponentT>
        const ComponentT* Manager::ComponentMapDerived<ComponentT>::FindComponent(Entity entity) const
        {
            return map.Find(entity);
        }

        template<class ComponentT>
        void Manager::ComponentMapDerived<ComponentT>::DestroyComponent(Entity entity)
        {
            map.Destroy(entity);
        }

        template<class ComponentT>
        void Manager::ComponentMapDerived<ComponentT>::Clear()
        {
            map.Clear();
        }

        template<class ComponentT>
        const ComponentTypeName& Manager::ComponentMapDerived<ComponentT>::GetTypeName() const
        {
            return ComponentT::TypeName();
        }

        template<class ComponentT>
        void Manager::ComponentMapDerived<ComponentT>::Serialize(inscription::Scribe &scribe)
        {
            scribe(map);
        }

        template<class ComponentT>
        std::pair<bool, ComponentT&> Manager::CreateComponent(Entity entity)
        {
            typedef std::pair<bool, ComponentT&> Ret;
            auto &ret = static_cast<ComponentMapDerived<ComponentT>*>(componentMaps.find(ComponentT::TypeName())->second.get())->CreateComponent(entity);
            return Ret(ret.first, static_cast<ComponentT&>(ret.second));
        }

        template<class ComponentT>
        std::pair<bool, ComponentT&> Manager::CreateComponent(Entity entity, const ComponentT &arg)
        {
            return static_cast<ComponentMapDerived<ComponentT>*>(componentMaps.find(ComponentT::TypeName())->second.get())->CreateComponent(entity, arg);
        }

        template<class ComponentT>
        std::pair<bool, ComponentT&> Manager::CreateComponent(Entity entity, ComponentT &&arg)
        {
            return static_cast<ComponentMapDerived<ComponentT>*>(componentMaps.find(ComponentT::TypeName())->second.get())->CreateComponent(entity, std::move(arg));
        }

        template<class ComponentT, class... Args>
        std::pair<bool, ComponentT&> Manager::CreateComponent(Entity entity, Args && ... args)
        {
            typedef std::pair<bool, ComponentT&> Ret;
            auto &ret = static_cast<ComponentMapDerived<ComponentT>*>(componentMaps.find(ComponentT::TypeName())->second.get())->CreateComponent(entity, std::forward<Args>(args)...);
            return Ret(ret.first, static_cast<ComponentT&>(ret.second));
        }

        template<class ComponentT>
        void Manager::DestroyComponent(Entity entity)
        {
            componentMaps.find(ComponentT::TypeName())->second->DestroyComponent(entity);
        }

        template<class ComponentT>
        ComponentT* Manager::FindComponent(Entity entity)
        {
            return static_cast<ComponentMapDerived<ComponentT>*>(componentMaps.find(ComponentT::TypeName())->second.get())->FindComponent(entity);
        }

        template<class ComponentT>
        const ComponentT* Manager::FindComponent(Entity entity) const
        {
            return static_cast<ComponentMapDerived<ComponentT>*>(componentMaps.find(ComponentT::TypeName())->second.get())->FindComponent(entity);
        }

        template<class... ComponentTs>
        MultipleComponents<ComponentTs...> Manager::FindMultipleComponents(Entity entity)
        {
            static constexpr size_t componentSize = sizeof...(ComponentTs);
            static_assert(componentSize > 0, "You must request at least 1 component type.");

            typedef MultipleComponents<ComponentTs...> TupleT;
            TupleT tuple;
            ::function::IterateRange<std::size_t, FillTuple, componentSize - 1, 0>(componentMaps, entity, tuple);
            return tuple;
        }

        template<class ComponentT>
        typename ComponentT::Map& Manager::GetMap()
        {
            return static_cast<ComponentMapDerived<ComponentT>*>(componentMaps.find(ComponentT::TypeName())->second.get())->map;
        }

        template<class ComponentT>
        typename ComponentT::Map::ContainerT& Manager::GetRawMap()
        {
            return GetMap<ComponentT>().GetContainer();
        }

        template<class T>
        void Manager::AddType()
        {
            ComponentMapFactoryDerived<T>::instance;
        }
    }

    typedef Ent::Manager EntityManager;
}