#pragma once

#include "ObjectSystem.h"
#include "ObjectBatch.h"

#include "AvatarComponent.h"
#include "Entity.h"

namespace Atmos::Entity
{
    class AvatarSystem : public ObjectSystem
    {
    private:
        typedef TypedObjectReference<Entity> EntityReference;
        typedef TypedObjectReference<AvatarComponent> AvatarComponentReference;
    public:
        AvatarSystem(ObjectManager& manager);

        void Set(EntityReference set);

        EntityReference Avatar();
        AvatarComponentReference Component();
    private:
        void InitializeImpl() override;
    private:
        EntityReference avatar;
    private:
        ObjectBatch<AvatarComponent> avatarBatch;
        void OnAvatarComponentCreated(AvatarComponentReference reference);
        void OnAvatarComponentDestroyed(AvatarComponentReference reference);
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::Entity::AvatarSystem, BinaryArchive> :
        public ObjectSystemScribe<::Atmos::Entity::AvatarSystem, BinaryArchive>
    {
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive);
    };
}