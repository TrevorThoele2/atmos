
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

#include <Inscription/OutputBinaryScribe.h>
#include <Inscription/InputBinaryScribe.h>

namespace Atmos
{
    ObjectRegistration::ObjectRegistration()
    {}

    void ObjectRegistration::RegisterDefaultGlobals()
    {
        RegisterGlobal<Asset>();
        RegisterGlobal<FileAsset>();
        RegisterGlobal<AudioAsset>();
        RegisterGlobal<ImageAsset>();
        RegisterGlobal<MaterialAsset>();
        RegisterGlobal<ShaderAsset>();

        RegisterGlobal<AudioAssetSystem>();
        RegisterGlobal<ImageAssetSystem>();
        RegisterGlobal<MaterialAssetSystem>();
        RegisterGlobal<ScriptAssetSystem>();
        RegisterGlobal<ShaderAssetSystem>();
    }

    void ObjectRegistration::RegisterDefaultLocals()
    {
        RegisterLocal<Tile>();

        RegisterLocal<Entity::Entity>();
        RegisterLocal<Entity::Component>();
        RegisterLocal<Entity::ActionComponent>();

        RegisterLocal<AxisAlignedObject>();
        RegisterLocal<RenderFragment>();
        RegisterLocal<Sense>();
        RegisterLocal<Sprite>();

        RegisterLocal<Entity::ActionSystem>();
        RegisterLocal<Entity::AvatarSystem>();
        RegisterLocal<Entity::AISystem>();
        RegisterLocal<Entity::PositionSystem>();

        RegisterLocal<TileSystem>();
        RegisterLocal<RenderFragmentSystem>();
        RegisterLocal<SoundSystem>();
        RegisterLocal<MusicSystem>();
    }

    void ObjectRegistration::RegisterDefaultInfrastructures()
    {
        RegisterInfrastructure<Object>();
        RegisterInfrastructure<ObjectSystem>();
        RegisterInfrastructure<ObjectBatchSourceBase>();
        RegisterInfrastructure<ObjectManager>();
    }

    void ObjectRegistration::RegisterAllDefaults()
    {
        RegisterDefaultGlobals();
        RegisterDefaultLocals();
        RegisterDefaultInfrastructures();
    }

    void ObjectRegistration::PushGlobalsTo(ObjectManager& manager)
    {
        for (auto& loop : globalEntryList)
            loop->PushTo(manager);
    }

    void ObjectRegistration::PushLocalsTo(ObjectManager& manager)
    {
        for (auto& loop : localEntryList)
            loop->PushTo(manager);
    }

    void ObjectRegistration::PushTo(ObjectTypeGraph& graph)
    {
        for (auto& loop : globalEntryList)
            loop->PushTo(graph);
        for (auto& loop : localEntryList)
            loop->PushTo(graph);
    }

    void ObjectRegistration::PushTo(::Inscription::BinaryScribe& scribe)
    {
        for (auto& loop : globalEntryList)
            loop->PushTo(scribe);
        for (auto& loop : localEntryList)
            loop->PushTo(scribe);
        for (auto& loop : infrastructureEntryList)
            loop->PushTo(scribe);
    }

    ObjectRegistration::Entry::~Entry()
    {}
}