#pragma once

#include <tuple>

#include "Entity.h"
#include "GeneralComponent.h"

#include "CurrentField.h"

#include <Function\Iterate.h>

namespace Atmos
{
    namespace Ent
    {
        template<class... ComponentTs>
        // Every type should be given as pointers
        // This will assume that all types are unique
        class ComponentPackage
        {
        public:
            typedef size_t ID;
        private:
            typedef std::tuple<ComponentTs...> Tuple;
        private:
            template<function::RangeSize I>
            class TupleIterator
            {
            public:
                static void Do(Entity entity)
                {
                    ptr = GetCurrentEntities()->FindComponent<std::tuple_element<I, Tuple>::type>(entity);
                }

                // Reset
                static void Do(Tuple &tuple)
                {
                    std::get<I>(tuple) = nullptr;
                }
            };

            template<function::RangeSize I>
            class TupleCheckHas
            {
            public:
                static bool Check(Entity entity)
                {
                    if (!GetCurrentEntities()->FindComponent<std::tuple_element<I, Tuple>::type>(entity))
                        return false;

                    return TupleCheckHas<I - 1>::Check(entity);
                }
            };

            template<>
            class TupleCheckHas<0>
            {
            public:
                static bool Check(Entity entity)
                {
                    return GetCurrentEntities()->FindComponent<std::tuple_element<0, Tuple>::type>(entity);
                }
            };
        private:
            Entity entity;
            Tuple tuple;

            void SetupComponents(Entity entity);
            void ResetComponents();
        public:
            ComponentPackage();
            ComponentPackage(Entity entity);
            ComponentPackage(const ComponentPackage &arg) = default;
            ComponentPackage& operator=(const ComponentPackage &arg) = default;
            explicit operator bool() const;
            explicit operator Entity() const;

            void SetEntity(Entity set);
            void ResetEntity();
            bool IsOccupied() const;
            Entity GetEntity() const;

            Optional<Name> GetName() const;
            Optional<GridPosition> GetPosition() const;
            Optional<Direction> GetDirection() const;

            template<ID I>
            typename std::tuple_element<I, Tuple>::type GetAt()
            {
                return std::get<I>(tuple);
            }

            template<ID I>
            typename const std::tuple_element<I, Tuple>::type GetAt() const
            {
                return std::get<I>(tuple);
            }

            template<class OtherT>
            OtherT* FindOther();
            template<class OtherT>
            const OtherT* FindOther() const;

            static bool HasComponents(Entity entity);
        };

        template<class... ComponentTs>
        void ComponentPackage<ComponentTs...>::SetupComponents(Entity entity)
        {
            function::IterateRange<ID, std::tuple_size<Tuple>::value, 0>(entity);
        }

        template<class... ComponentTs>
        void ComponentPackage<ComponentTs...>::ResetComponents()
        {
            function::IterateRange<ID, std::tuple_size<Tuple>::value, 0>(tuple);
        }

        template<class... ComponentTs>
        ComponentPackage<ComponentTs...>::ComponentPackage() : entity(nullEntity)
        {
            ResetComponents();
        }

        template<class... ComponentTs>
        ComponentPackage<ComponentTs...>::ComponentPackage(Entity entity) : entity(entity)
        {
            if(entity == nullEntity) ? ResetComponents() : SetupComponents(entity);
        }

        template<class... ComponentTs>
        ComponentPackage<ComponentTs...>::operator bool() const
        {
            return IsOccupied();
        }

        template<class... ComponentTs>
        ComponentPackage<ComponentTs...>::operator Entity() const
        {
            return entity;
        }

        template<class... ComponentTs>
        void ComponentPackage<ComponentTs...>::SetEntity(Entity set)
        {
            entity = set;
            SetupComponents(entity);
        }

        template<class... ComponentTs>
        void ComponentPackage<ComponentTs...>::ResetEntity()
        {
            entity = nullEntity;
            field.Reset();
            ResetComponents();
        }

        template<class... ComponentTs>
        bool ComponentPackage<ComponentTs...>::IsOccupied() const
        {
            return entity != nullEntity;
        }

        template<class... ComponentTs>
        Entity ComponentPackage<ComponentTs...>::GetEntity() const
        {
            return entity;
        }

        template<class... ComponentTs>
        Optional<Name> ComponentPackage<ComponentTs...>::GetName() const
        {
            return GetCurrentEntities()->GetName(entity);
        }

        template<class... ComponentTs>
        Optional<GridPosition> ComponentPackage<ComponentTs...>::GetPosition() const
        {
            return GetCurrentEntities()->GetPosition(entity);
        }

        template<class... ComponentTs>
        Optional<Direction> ComponentPackage<ComponentTs...>::GetDirection() const
        {
            return GetCurrentEntities()->GetDirection(entity);
        }

        template<class... ComponentTs>
        template<class OtherT>
        OtherT* ComponentPackage<ComponentTs...>::FindOther()
        {
            return GetCurrentEntities()->FindComponent<OtherT>(entity);
        }

        template<class... ComponentTs>
        template<class OtherT>
        const OtherT* ComponentPackage<ComponentTs...>::FindOther() const
        {
            return GetCurrentEntities()->FindComponent<OtherT>(entity);
        }
        
        template<class... ComponentTs>
        bool ComponentPackage<ComponentTs...>::HasComponents(Entity entity)
        {
            return TupleCheckHas<std::tuple_size<Tuple>::value>::Check(entity);
        }

        template<class... ComponentTs>
        struct ComponentPackageInherit
        {
        public:
            typedef ComponentPackage<ComponentTs...> Package;
            Package package;

            ComponentPackageInherit(Entity entity);
            ComponentPackageInherit(const ComponentPackageInherit &arg) = default;
            ComponentPackageInherit& operator=(const ComponentPackageInherit &arg) = default;
            explicit operator bool() const;
            explicit operator Entity() const;
            Package* operator->();
            const Package* operator->() const;

            bool IsOccupied() const;
            Entity GetEntity() const;

            Optional<Name> GetName() const;
            Optional<GridPosition> GetPosition() const;
            Optional<Direction> GetDirection() const;
        };

        template<class... ComponentTs>
        ComponentPackageInherit<ComponentTs...>::ComponentPackageInherit(Entity entity) : package(entity)
        {}

        template<class... ComponentTs>
        ComponentPackageInherit<ComponentTs...>::operator bool() const
        {
            return static_cast<bool>(package);
        }

        template<class... ComponentTs>
        ComponentPackageInherit<ComponentTs...>::operator Entity() const
        {
            return static_cast<Entity>(package);
        }

        template<class... ComponentTs>
        typename ComponentPackageInherit<ComponentTs...>::Package* ComponentPackageInherit<ComponentTs...>::operator->()
        {
            return &package;
        }

        template<class... ComponentTs>
        typename const ComponentPackageInherit<ComponentTs...>::Package* ComponentPackageInherit<ComponentTs...>::operator->() const
        {
            return &package;
        }

        template<class... ComponentTs>
        bool ComponentPackageInherit<ComponentTs...>::IsOccupied() const
        {
            return package.IsOccupied();
        }

        template<class... ComponentTs>
        Entity ComponentPackageInherit<ComponentTs...>::GetEntity() const
        {
            return package.GetEntity();
        }

        template<class... ComponentTs>
        Optional<Name> ComponentPackageInherit<ComponentTs...>::GetName() const
        {
            return package.GetName();
        }

        template<class... ComponentTs>
        Optional<GridPosition> ComponentPackageInherit<ComponentTs...>::GetPosition() const
        {
            return package.GetPosition();
        }

        template<class... ComponentTs>
        Optional<Direction> ComponentPackageInherit<ComponentTs...>::GetDirection() const
        {
            return package.GetDirection();
        }

        template<class... ComponentTs>
        class ComponentPackageRequired
        {
        public:
            typedef size_t ID;
        private:
            typedef std::tuple<ComponentTs...> Tuple;
        private:
            template<ID I>
            class TupleIterator
            {
            private:
                template<class CompT>
                static bool DoOnPiece(Entity entity, CompT *&ptr)
                {
                    auto found = GetCurrentEntities()->FindComponent<CompT>(entity);
                    if (!found)
                        return false;

                    ptr = found;
                    return true;
                }
            public:
                static bool Do(Entity entity, Tuple &tuple)
                {
                    if (!DoOnPiece(entity, std::get<I - 1>(tuple)))
                        return false;

                    return TupleIterator<I - 1>::Do(entity, tuple);
                }

                // Reset
                static void Do(Tuple &tuple)
                {
                    std::get<I - 1>(tuple) = nullptr;
                }

                static bool CheckHas(Entity entity)
                {
                    if (!GetCurrentEntities()->FindComponent<std::tuple_element<I, Tuple>::type>(entity))
                        return false;

                    return TupleIterator<I - 1>::CheckHas(entity);
                }
            };

            template<>
            class TupleIterator<0>
            {
            private:
                template<class CompT>
                static bool DoOnPiece(Entity entity, CompT *&ptr)
                {
                    auto found = GetCurrentEntities()->FindComponent<CompT>(entity);
                    if (!found)
                        return false;

                    ptr = found;
                    return true;
                }
            public:
                static bool Do(Entity entity, Tuple &tuple)
                {
                    return DoOnPiece(entity, std::get<0>(tuple));
                }

                // Reset
                static void Do(Tuple &tuple)
                {
                    std::get<0>(tuple) = nullptr;
                }

                static bool CheckHas(Entity entity)
                {
                    return GetCurrentEntities()->FindComponent<std::tuple_element<0, Tuple>::type>(entity);
                }
            };
        private:
            Entity entity;
            Tuple tuple;

            bool SetupComponents(Entity entity);
            void ResetComponents();
        public:
            ComponentPackageRequired();
            ComponentPackageRequired(Entity entity);
            ComponentPackageRequired(const ComponentPackageRequired &arg) = default;
            ComponentPackageRequired& operator=(const ComponentPackageRequired &arg) = default;
            explicit operator bool() const;
            explicit operator Entity() const;

            // Returns false if the entity could not be set because it doesn't have the right components
            bool SetEntity(Entity set);
            void ResetEntity();
            bool IsOccupied() const;
            Entity GetEntity() const;

            Optional<Name> GetName() const;
            Optional<GridPosition> GetPosition() const;
            Optional<Direction> GetDirection() const;

            template<ID I>
            typename std::tuple_element<I, Tuple>::type GetAt()
            {
                return std::get<I>(tuple);
            }

            template<ID I>
            typename const std::tuple_element<I, Tuple>::type GetAt() const
            {
                return std::get<I>(tuple);
            }

            template<class OtherT>
            OtherT* FindOther();
            template<class OtherT>
            const OtherT* FindOther() const;

            static bool HasComponents(Entity entity);
        };

        template<class... ComponentTs>
        bool ComponentPackageRequired<ComponentTs...>::SetupComponents(Entity entity)
        {
            return TupleIterator<sizeof...(ComponentTs)>::Do(entity, tuple);
        }

        template<class... ComponentTs>
        void ComponentPackageRequired<ComponentTs...>::ResetComponents()
        {
            function::IterateRange<ID, TupleIterator, std::tuple_size<Tuple>::value, 0>(tuple);
        }

        template<class... ComponentTs>
        ComponentPackageRequired<ComponentTs...>::ComponentPackageRequired() : entity(nullEntity)
        {
            ResetComponents();
        }

        template<class... ComponentTs>
        ComponentPackageRequired<ComponentTs...>::ComponentPackageRequired(Entity entity)
        {
            SetEntity(entity);
        }

        template<class... ComponentTs>
        ComponentPackageRequired<ComponentTs...>::operator bool() const
        {
            return IsOccupied();
        }

        template<class... ComponentTs>
        ComponentPackageRequired<ComponentTs...>::operator Entity() const
        {
            return entity;
        }

        template<class... ComponentTs>
        bool ComponentPackageRequired<ComponentTs...>::SetEntity(Entity set)
        {
            if (set == nullEntity)
            {
                ResetEntity();
                return false;
            }

            if (SetupComponents(set))
            {
                entity = set;
                return true;
            }
            else
            {
                ResetEntity();
                return false;
            }
        }

        template<class... ComponentTs>
        void ComponentPackageRequired<ComponentTs...>::ResetEntity()
        {
            entity = nullEntity;
            ResetComponents();
        }

        template<class... ComponentTs>
        bool ComponentPackageRequired<ComponentTs...>::IsOccupied() const
        {
            return entity != nullEntity;
        }

        template<class... ComponentTs>
        Entity ComponentPackageRequired<ComponentTs...>::GetEntity() const
        {
            return entity;
        }

        template<class... ComponentTs>
        Optional<Name> ComponentPackageRequired<ComponentTs...>::GetName() const
        {
            return GetCurrentEntities()->GetName(entity);
        }

        template<class... ComponentTs>
        Optional<GridPosition> ComponentPackageRequired<ComponentTs...>::GetPosition() const
        {
            return GetCurrentEntities()->GetPosition(entity);
        }

        template<class... ComponentTs>
        Optional<Direction> ComponentPackageRequired<ComponentTs...>::GetDirection() const
        {
            return GetCurrentEntities()->GetDirection(entity);
        }

        template<class... ComponentTs>
        template<class OtherT>
        OtherT* ComponentPackageRequired<ComponentTs...>::FindOther()
        {
            return GetCurrentEntities()->FindComponent<OtherT>(entity);
        }

        template<class... ComponentTs>
        template<class OtherT>
        const OtherT* ComponentPackageRequired<ComponentTs...>::FindOther() const
        {
            return GetCurrentEntities()->FindComponent<OtherT>(entity);
        }

        template<class... ComponentTs>
        bool ComponentPackageRequired<ComponentTs...>::HasComponents(Entity entity)
        {
            return TupleIterator<std::tuple_size<Tuple>::value>::CheckHas(entity);
        }

        template<class... ComponentTs>
        struct ComponentPackageRequiredInherit
        {
        public:
            typedef ComponentPackageRequired<ComponentTs...> Package;
            Package package;

            ComponentPackageRequiredInherit(Entity entity);
            ComponentPackageRequiredInherit(const ComponentPackageRequiredInherit &arg) = default;
            ComponentPackageRequiredInherit& operator=(const ComponentPackageRequiredInherit &arg) = default;
            explicit operator bool() const;
            explicit operator Entity() const;
            Package* operator->();
            const Package* operator->() const;

            bool SetEntity(Entity entity);
            Entity GetEntity() const;
            bool IsOccupied() const;

            Optional<Name> GetName() const;
            Optional<GridPosition> GetPosition() const;
            Optional<Direction> GetDirection() const;
        };

        template<class... ComponentTs>
        ComponentPackageRequiredInherit<ComponentTs...>::ComponentPackageRequiredInherit(Entity entity) : package(entity)
        {}

        template<class... ComponentTs>
        ComponentPackageRequiredInherit<ComponentTs...>::operator bool() const
        {
            return static_cast<bool>(package);
        }

        template<class... ComponentTs>
        ComponentPackageRequiredInherit<ComponentTs...>::operator Entity() const
        {
            return static_cast<Entity>(package);
        }

        template<class... ComponentTs>
        typename ComponentPackageRequiredInherit<ComponentTs...>::Package* ComponentPackageRequiredInherit<ComponentTs...>::operator->()
        {
            return &package;
        }

        template<class... ComponentTs>
        typename const ComponentPackageRequiredInherit<ComponentTs...>::Package* ComponentPackageRequiredInherit<ComponentTs...>::operator->() const
        {
            return &package;
        }

        template<class... ComponentTs>
        bool ComponentPackageRequiredInherit<ComponentTs...>::SetEntity(Entity entity)
        {
            return package.SetEntity(entity);
        }

        template<class... ComponentTs>
        Entity ComponentPackageRequiredInherit<ComponentTs...>::GetEntity() const
        {
            return package.GetEntity();
        }

        template<class... ComponentTs>
        bool ComponentPackageRequiredInherit<ComponentTs...>::IsOccupied() const
        {
            return package.IsOccupied();
        }

        template<class... ComponentTs>
        Optional<Name> ComponentPackageRequiredInherit<ComponentTs...>::GetName() const
        {
            return package.GetName();
        }

        template<class... ComponentTs>
        Optional<GridPosition> ComponentPackageRequiredInherit<ComponentTs...>::GetPosition() const
        {
            return package.GetPosition();
        }

        template<class... ComponentTs>
        Optional<Direction> ComponentPackageRequiredInherit<ComponentTs...>::GetDirection() const
        {
            return package.GetDirection();
        }
    }
}