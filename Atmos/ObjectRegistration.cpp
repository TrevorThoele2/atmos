
#include "ObjectRegistration.h"

#include "ObjectManager.h"

#include "Asset.h"
#include "AudioAsset.h"
#include "ImageAsset.h"
#include "MaterialAsset.h"
#include "ShaderAsset.h"

#include "AudioAssetSystem.h"
#include "ImageAssetSystem.h"
#include "MaterialAssetSystem.h"
#include "ScriptAssetSystem.h"
#include "ShaderAssetSystem.h"

#include "Tile.h"

#include "EntityActionSystem.h"
#include "EntityAvatarSystem.h"
#include "EntityAISystem.h"
#include "EntityPositionSystem.h"

#include "TileSystem.h"
#include "RenderFragmentSystem.h"
#include "SoundSystem.h"
#include "MusicSystem.h"

#include "AudioAssetFactory.h"
#include "ImageAssetFactory.h"
#include "MaterialAssetFactory.h"
#include "ScriptAssetFactory.h"
#include "ShaderAssetFactory.h"
#include "TileFactory.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    class ObjectRegistration::Impl
    {
    public:
        class Registrator
        {
        public:
            virtual ~Registrator() = 0;
            virtual void Register(ObjectManager& manager) = 0;
            virtual void Register(ObjectTypeGraph& graph) = 0;
            virtual void Register(::Inscription::Scribe& scribe) = 0;
        };

        template<class T>
        class ObjectRegistrator : public Registrator
        {
        public:
            void Register(ObjectManager& manager) override;
            void Register(ObjectTypeGraph& graph) override;
            void Register(::Inscription::Scribe& scribe) override;
        };

        template<class T>
        class SystemRegistrator : public Registrator
        {
        public:
            void Register(ObjectManager& manager) override;
            void Register(ObjectTypeGraph& graph) override;
            void Register(::Inscription::Scribe& scribe) override;
        };

        typedef std::unique_ptr<Registrator> RegistratorPtr;
        typedef std::vector<RegistratorPtr> RegistratorList;
        RegistratorList globalRegistratorList;
        RegistratorList localRegistratorList;

        template<class T>
        void CreateGlobalObjectRegistrator();
        template<class T>
        void CreateGlobalSystemRegistrator();
        template<class T>
        void CreateLocalObjectRegistrator();
        template<class T>
        void CreateLocalSystemRegistrator();
    };

    ObjectRegistration::Impl::Registrator::~Registrator()
    {}

    template<class T>
    void ObjectRegistration::Impl::ObjectRegistrator<T>::Register(ObjectManager& manager)
    {
        manager.RegisterObjectType<T>();
    }

    template<class T>
    void ObjectRegistration::Impl::ObjectRegistrator<T>::Register(ObjectTypeGraph& graph)
    {
        auto description = TypeDescriptionFor<T>();
        graph.AddDescription(description);
    }

    template<class T>
    void ObjectRegistration::Impl::ObjectRegistrator<T>::Register(::Inscription::Scribe& scribe)
    {
        scribe.RegisterType<T>();
    }

    template<class T>
    void ObjectRegistration::Impl::SystemRegistrator<T>::Register(ObjectManager& manager)
    {
        manager.RegisterSystemType<T>();
    }

    template<class T>
    void ObjectRegistration::Impl::SystemRegistrator<T>::Register(ObjectTypeGraph& graph)
    {}

    template<class T>
    void ObjectRegistration::Impl::SystemRegistrator<T>::Register(::Inscription::Scribe& scribe)
    {
        scribe.RegisterType<T>();
    }

    template<class T>
    void ObjectRegistration::Impl::CreateGlobalObjectRegistrator()
    {
        globalRegistratorList.push_back(RegistratorPtr(new ObjectRegistrator<T>()));
    }

    template<class T>
    void ObjectRegistration::Impl::CreateGlobalSystemRegistrator()
    {
        globalRegistratorList.push_back(RegistratorPtr(new SystemRegistrator<T>()));
    }

    template<class T>
    void ObjectRegistration::Impl::CreateLocalObjectRegistrator()
    {
        localRegistratorList.push_back(RegistratorPtr(new ObjectRegistrator<T>()));
    }

    template<class T>
    void ObjectRegistration::Impl::CreateLocalSystemRegistrator()
    {
        localRegistratorList.push_back(RegistratorPtr(new SystemRegistrator<T>()));
    }

    ObjectRegistration::ObjectRegistration()
    {
        EnsureImplCreated();
        StoreAllGlobals();
        StoreAllLocals();
    }

    void ObjectRegistration::RegisterGlobal(ObjectManager& manager)
    {
        for (auto& loop : impl->globalRegistratorList)
            loop->Register(manager);
    }

    void ObjectRegistration::RegisterLocal(ObjectManager& manager)
    {
        for (auto& loop : impl->localRegistratorList)
            loop->Register(manager);
    }

    void ObjectRegistration::Register(ObjectTypeGraph& graph)
    {
        for (auto& loop : impl->globalRegistratorList)
            loop->Register(graph);
        for (auto& loop : impl->localRegistratorList)
            loop->Register(graph);
    }

    void ObjectRegistration::Register(::Inscription::Scribe& scribe)
    {
        for (auto& loop : impl->globalRegistratorList)
            loop->Register(scribe);
        for (auto& loop : impl->localRegistratorList)
            loop->Register(scribe);
    }

    void ObjectRegistration::StoreAllGlobals()
    {
        impl->CreateGlobalObjectRegistrator<Asset>();
        impl->CreateGlobalObjectRegistrator<FileAsset>();
        impl->CreateGlobalObjectRegistrator<AudioAsset>();
        impl->CreateGlobalObjectRegistrator<ImageAsset>();
        impl->CreateGlobalObjectRegistrator<MaterialAsset>();
        impl->CreateGlobalObjectRegistrator<ShaderAsset>();

        impl->CreateGlobalSystemRegistrator<AudioAssetSystem>();
        impl->CreateGlobalSystemRegistrator<ImageAssetSystem>();
        impl->CreateGlobalSystemRegistrator<MaterialAssetSystem>();
        impl->CreateGlobalSystemRegistrator<ScriptAssetSystem>();
        impl->CreateGlobalSystemRegistrator<ShaderAssetSystem>();
    }

    void ObjectRegistration::StoreAllLocals()
    {
        impl->CreateLocalObjectRegistrator<Tile>();

        impl->CreateLocalObjectRegistrator<Entity::Entity>();
        impl->CreateLocalObjectRegistrator<Entity::Component>();
        impl->CreateLocalObjectRegistrator<Entity::ActionComponent>();

        impl->CreateLocalObjectRegistrator<PositionalObject>();
        impl->CreateLocalObjectRegistrator<RenderFragment>();
        impl->CreateLocalObjectRegistrator<Sense>();
        impl->CreateLocalObjectRegistrator<Sprite>();

        impl->CreateLocalSystemRegistrator<Entity::ActionSystem>();
        impl->CreateLocalSystemRegistrator<Entity::AvatarSystem>();
        impl->CreateLocalSystemRegistrator<Entity::AISystem>();
        impl->CreateLocalSystemRegistrator<Entity::PositionSystem>();

        impl->CreateLocalSystemRegistrator<TileSystem>();
        impl->CreateLocalSystemRegistrator<RenderFragmentSystem>();
        impl->CreateLocalSystemRegistrator<SoundSystem>();
        impl->CreateLocalSystemRegistrator<MusicSystem>();
    }

    ObjectRegistration::ImplPtr ObjectRegistration::impl;

    void ObjectRegistration::EnsureImplCreated()
    {
        if (impl)
            return;

        impl.reset(new Impl());
    }
}