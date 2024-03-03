#pragma once

#include "Entity.h"
#include "Event.h"

namespace Atmos
{
    namespace Ent
    {
        // Will respond to entity changed events
        class Reference
        {
        private:
            Entity entity;
            EventScopedConnection changedConnection;
            void OnEntityChanged(Entity from, Entity to);
        public:
            Reference();
            Reference(Entity entity);
            Reference(const Reference &arg);
            Reference(Reference &&arg);
            Reference& operator=(const Reference &arg);
            Reference& operator=(Reference &&arg);

            Reference& operator=(Entity set);

            Entity operator*() const;
            explicit operator Entity() const;
            Entity Get() const;
            void Set(Entity set);
        };
    }
}