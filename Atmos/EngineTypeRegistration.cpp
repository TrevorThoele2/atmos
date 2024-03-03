#include "EngineTypeRegistration.h"

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

namespace Atmos
{
    void RegisterGlobalTypes(TypeRegistration::Group& to)
    {
        to.Register<Asset::Asset>();
        to.Register<Asset::FileAsset>();
        to.Register<Asset::AudioAsset>();
        to.Register<Asset::ImageAsset>();
        to.Register<Asset::MaterialAsset>();
        to.Register<Asset::ShaderAsset>();

        to.Register<Asset::AudioAssetSystem>();
        to.Register<Asset::ImageAssetSystem>();
        to.Register<Asset::MaterialAssetSystem>();
        to.Register<Asset::ScriptAssetSystem>();
        to.Register<Asset::ShaderAssetSystem>();
    }

    void RegisterLocalTypes(TypeRegistration::Group& to)
    {
        to.Register<Grid::Tile>();

        to.Register<Entity::Entity>();
        to.Register<Entity::Component>();
        to.Register<Entity::ActionComponent>();

        to.Register<AxisAlignedObject>();
        to.Register<Render::RenderFragment>();
        to.Register<Render::Sprite>();

        to.Register<Entity::ActionSystem>();
        to.Register<Entity::AvatarSystem>();
        to.Register<Entity::AISystem>();
        to.Register<Entity::PositionSystem>();

        to.Register<Grid::TileSystem>();
        to.Register<Render::RenderFragmentSystem>();
        to.Register<Audio::SoundSystem>();
        to.Register<Audio::MusicSystem>();
    }

    void RegisterInfrastructureTypes(TypeRegistration::Group& to)
    {
        to.RegisterInfrastructure<Object>();
        to.RegisterInfrastructure<ObjectSystem>();
        to.RegisterInfrastructure<ObjectBatchSourceBase>();
        to.RegisterInfrastructure<ObjectManager>();
    }
}