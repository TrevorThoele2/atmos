#pragma once

#include <memory>
#include <map>
#include <unordered_map>

#include "Entity.h"

#include "ComponentTypeName.h"
#include "RandomAccessSequence.h"
#include "Event.h"

#include "Serialization.h"
#include <Inscription\Map.h>

#define ENTITY_COMPONENT_MAP_DECLARE(Name, ComponentT)                                                  \
template<>                                                                                              \
class ComponentMap<ComponentT> : public ComponentMapBase<ComponentT>                                    \
{                                                                                                       \
public:                                                                                                 \
    ComponentMap() { Manager::AddType<ComponentT>(); }                                              \
    static const ComponentTypeName& TypeName() { static ComponentTypeName name = Name; return name;}    \
};

#define ENTITY_COMPONENT_MAP_DECLARE_CUSTOM(Name, ComponentT, ContainerT)                               \
template<>                                                                                              \
class ComponentMap<ComponentT> : public ComponentMapBase<ComponentT, ContainerT>                        \
{                                                                                                       \
public:                                                                                                 \
    ComponentMap() { Manager::AddType<ComponentT>(); }                                                  \
    static const ComponentTypeName& TypeName() { static ComponentTypeName name = Name; return name; }   \
};

namespace Atmos
{
    namespace Ent
    {
        template<class ContainerT>
        class ComponentMapBaseTraits;

        template<class T>
        class ComponentMapBaseTraits<std::map<Entity, T>>
        {
        public:
            typedef std::map<Entity, T> ContainerT;
            typedef std::pair<bool, T&> CreateRet;
        public:
            template<class... Args>
            static CreateRet Create(ContainerT &container, Entity entity, Args && ... args);
            static T* Move(ContainerT &from, ContainerT &to, Entity entity, Entity newID);
            static void Destroy(ContainerT &container, Entity entity);
            static void Clear(ContainerT &container);
            static T* Find(ContainerT &container, Entity entity);
            static const T* FindConst(const ContainerT &container, Entity entity);

            static void Serialize(inscription::Scribe &scribe, ContainerT &container, Event<T&> &createdEvent, void(*setOwner)(T&, Entity));
        };

        template<class T>
        template<class... Args>
        typename ComponentMapBaseTraits<std::map<Entity, T>>::CreateRet ComponentMapBaseTraits<std::map<Entity, T>>::Create(ContainerT &container, Entity entity, Args && ... args)
        {
            auto found = container.find(entity);
            if (found != container.end())
                return CreateRet(false, found->second);
            else
                return CreateRet(true, container.emplace(entity, T(std::forward<Args>(args)...)).first->second);
        }

        template<class T>
        T* ComponentMapBaseTraits<std::map<Entity, T>>::Move(ContainerT &from, ContainerT &to, Entity entity, Entity newID)
        {
            auto found = from.find(entity);
            if (found == from.end())
                return nullptr;

            auto &ret = to.emplace(newID, std::move(found->second)).first->second;
            from.erase(found);
            return &ret;
        }

        template<class T>
        void ComponentMapBaseTraits<std::map<Entity, T>>::Destroy(ContainerT &container, Entity entity)
        {
            container.erase(entity);
        }

        template<class T>
        void ComponentMapBaseTraits<std::map<Entity, T>>::Clear(ContainerT &container)
        {
            container.clear();
        }

        template<class T>
        T* ComponentMapBaseTraits<std::map<Entity, T>>::Find(ContainerT &container, Entity entity)
        {
            auto found = container.find(entity);
            if (found == container.end())
                return nullptr;

            return &found->second;
        }

        template<class T>
        const T* ComponentMapBaseTraits<std::map<Entity, T>>::FindConst(const ContainerT &container, Entity entity)
        {
            auto found = container.find(entity);
            if (found == container.end())
                return nullptr;

            return &found->second;
        }

        template<class T>
        void ComponentMapBaseTraits<std::map<Entity, T>>::Serialize(inscription::Scribe &scribe, ContainerT &container, Event<T&> &createdEvent, void(*setOwner)(T&, Entity))
        {
            if (scribe.IsOutput())
            {
                inscription::ContainerSize size(container.size());
                scribe.Save(size);

                for (auto &loop : container)
                {
                    scribe.Save(inscription::RemoveConst(loop.first));
                    scribe.Save(loop.second);
                }
            }
            else
            {
                inscription::ContainerSize size;
                scribe.Load(size);

                while (size-- > 0)
                {
                    Entity entity;
                    scribe.Load(entity);

                    auto &emplaced = container.emplace(entity, T()).first->second;
                    scribe.Load(emplaced);
                    (setOwner)(emplaced, entity);
                    createdEvent(emplaced);
                }
            }
        }

        template<class ComponentT>
        class ComponentMap;

        template<class ComponentT, class UnderlyingContainer = std::map<Entity, ComponentT>>
        class ComponentMapBase
        {
        public:
            typedef ComponentMapBaseTraits<UnderlyingContainer> Traits;
            typedef typename Traits::CreateRet CreateRet;
            typedef typename Traits::ContainerT ContainerT;
            typedef typename ContainerT::iterator iterator;
            typedef typename ContainerT::const_iterator const_iterator;
            typedef typename ContainerT::size_type size_type;
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        private:
            ContainerT container;
        protected:
            ComponentMapBase() = default;
        public:
            static Event<ComponentT&> onCreated;
            static Event<Entity> beforeDestroyed;
            static Event<Entity> onDestroyed;

            template<class... Args>
            CreateRet Create(Entity entity, Args && ... args);
            void Move(ComponentMapBase &moveTo, Entity entity, Entity newID);
            void MoveTemporary(ComponentMapBase &moveTo, Entity entity);
            void Destroy(Entity entity);
            void Clear();
            ComponentT* Find(Entity entity);
            const ComponentT* Find(Entity entity) const;

            ContainerT& GetContainer();

            size_type size() const;
            iterator begin();
            iterator end();
            const_iterator begin() const;
            const_iterator end() const;
        };

        template<class ComponentT, class UnderlyingContainer>
        void ComponentMapBase<ComponentT, UnderlyingContainer>::Serialize(::inscription::Scribe &scribe)
        {
            Traits::Serialize(scribe, container, onCreated, static_cast<void(*)(ComponentT&, Entity)>([](ComponentT &comp, Entity entity) { comp.ownerEntity = entity; }));
        }

        template<class ComponentT, class UnderlyingContainer>
        Event<ComponentT&> ComponentMapBase<ComponentT, UnderlyingContainer>::onCreated;

        template<class ComponentT, class UnderlyingContainer>
        Event<Entity> ComponentMapBase<ComponentT, UnderlyingContainer>::beforeDestroyed;

        template<class ComponentT, class UnderlyingContainer>
        Event<Entity> ComponentMapBase<ComponentT, UnderlyingContainer>::onDestroyed;

        template<class ComponentT, class UnderlyingContainer>
        template<class... Args>
        typename ComponentMapBase<ComponentT, UnderlyingContainer>::CreateRet ComponentMapBase<ComponentT, UnderlyingContainer>::Create(Entity entity, Args && ... args)
        {
            auto &returned = Traits::Create(container, entity, std::forward<Args>(args)...);
            if (returned.first)
            {
                returned.second.ownerEntity = entity;
                onCreated(returned.second);
            }

            return returned;
        }

        template<class ComponentT, class UnderlyingContainer>
        void ComponentMapBase<ComponentT, UnderlyingContainer>::Move(ComponentMapBase &moveTo, Entity entity, Entity newID)
        {
            auto moved = Traits::Move(container, moveTo.container, entity, newID);
            if (moved)
            {
                moved->ownerEntity = newID;
                onCreated(*moved);
            }
        }

        template<class ComponentT, class UnderlyingContainer>
        void ComponentMapBase<ComponentT, UnderlyingContainer>::MoveTemporary(ComponentMapBase &moveTo, Entity entity)
        {
            Traits::Move(container, moveTo.container, entity, entity);
        }

        template<class ComponentT, class UnderlyingContainer>
        void ComponentMapBase<ComponentT, UnderlyingContainer>::Destroy(Entity entity)
        {
            if (!Find(entity))
                return;

            beforeDestroyed(entity);
            Traits::Destroy(container, entity);
            onDestroyed(entity);
        }

        template<class ComponentT, class UnderlyingContainer>
        void ComponentMapBase<ComponentT, UnderlyingContainer>::Clear()
        {
            Traits::Clear(container);
        }

        template<class ComponentT, class UnderlyingContainer>
        ComponentT* ComponentMapBase<ComponentT, UnderlyingContainer>::Find(Entity entity)
        {
            return Traits::Find(container, entity);
        }

        template<class ComponentT, class UnderlyingContainer>
        const ComponentT* ComponentMapBase<ComponentT, UnderlyingContainer>::Find(Entity entity) const
        {
            return Traits::FindConst(container, entity);
        }

        template<class ComponentT, class UnderlyingContainer>
        typename ComponentMapBase<ComponentT, UnderlyingContainer>::ContainerT& ComponentMapBase<ComponentT, UnderlyingContainer>::GetContainer()
        {
            return container;
        }

        template<class ComponentT, class UnderlyingContainer>
        typename ComponentMapBase<ComponentT, UnderlyingContainer>::size_type ComponentMapBase<ComponentT, UnderlyingContainer>::size() const
        {
            return container.size();
        }

        template<class ComponentT, class UnderlyingContainer>
        typename ComponentMapBase<ComponentT, UnderlyingContainer>::iterator ComponentMapBase<ComponentT, UnderlyingContainer>::begin()
        {
            return container.begin();
        }

        template<class ComponentT, class UnderlyingContainer>
        typename ComponentMapBase<ComponentT, UnderlyingContainer>::iterator ComponentMapBase<ComponentT, UnderlyingContainer>::end()
        {
            return container.end();
        }

        template<class ComponentT, class UnderlyingContainer>
        typename ComponentMapBase<ComponentT, UnderlyingContainer>::const_iterator ComponentMapBase<ComponentT, UnderlyingContainer>::begin() const
        {
            return container.begin();
        }

        template<class ComponentT, class UnderlyingContainer>
        typename ComponentMapBase<ComponentT, UnderlyingContainer>::const_iterator ComponentMapBase<ComponentT, UnderlyingContainer>::end() const
        {
            return container.end();
        }
    }
}