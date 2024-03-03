
#include "EntityReference.h"
#include "EntityManager.h"

namespace Atmos
{
    namespace Ent
    {
        void Reference::OnEntityChanged(Entity from, Entity to)
        {
            if (from == entity)
                entity = to;
        }

        Reference::Reference() : entity(nullEntity)
        {
            changedConnection.Set<Entity, Entity>(Manager::eventEntityTransferred.Subscribe(&Reference::OnEntityChanged, *this));
        }

        Reference::Reference(Entity entity) : entity(entity)
        {
            changedConnection.Set<Entity, Entity>(Manager::eventEntityTransferred.Subscribe(&Reference::OnEntityChanged, *this));
        }

        Reference::Reference(const Reference &arg) : entity(arg.entity)
        {
            changedConnection.Set<Entity, Entity>(Manager::eventEntityTransferred.Subscribe(&Reference::OnEntityChanged, *this));
        }

        Reference::Reference(Reference &&arg) : entity(arg.entity)
        {
            changedConnection.Set<Entity, Entity>(Manager::eventEntityTransferred.Subscribe(&Reference::OnEntityChanged, *this));
        }

        Reference& Reference::operator=(const Reference &arg)
        {
            entity = arg.entity;
            return *this;
        }

        Reference& Reference::operator=(Reference &&arg)
        {
            entity = arg.entity;
            return *this;
        }

        Reference& Reference::operator=(Entity set)
        {
            entity = set;
            return *this;
        }

        Entity Reference::operator*() const
        {
            return entity;
        }

        Reference::operator Entity() const
        {
            return entity;
        }

        Entity Reference::Get() const
        {
            return entity;
        }

        void Reference::Set(Entity set)
        {
            entity = set;
        }
    }
}