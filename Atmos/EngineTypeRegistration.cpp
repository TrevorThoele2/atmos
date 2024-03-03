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
        to.Register<Asset>();
        to.Register<FileAsset>();
        to.Register<AudioAsset>();
        to.Register<ImageAsset>();
        to.Register<MaterialAsset>();
        to.Register<ShaderAsset>();

        to.Register<AudioAssetSystem>();
        to.Register<ImageAssetSystem>();
        to.Register<MaterialAssetSystem>();
        to.Register<ScriptAssetSystem>();
        to.Register<ShaderAssetSystem>();
    }

    void RegisterLocalTypes(TypeRegistration::Group& to)
    {
        to.Register<Tile>();

        to.Register<Entity::Entity>();
        to.Register<Entity::Component>();
        to.Register<Entity::ActionComponent>();

        to.Register<AxisAlignedObject>();
        to.Register<Fragment>();
        to.Register<Sense>();
        to.Register<Sprite>();

        to.Register<Entity::ActionSystem>();
        to.Register<Entity::AvatarSystem>();
        to.Register<Entity::AISystem>();
        to.Register<Entity::PositionSystem>();

        to.Register<TileSystem>();
        to.Register<FragmentSystem>();
        to.Register<SoundSystem>();
        to.Register<MusicSystem>();
    }

    void RegisterInfrastructureTypes(TypeRegistration::Group& to)
    {
        to.RegisterInfrastructure<Object>();
        to.RegisterInfrastructure<ObjectSystem>();
        to.RegisterInfrastructure<ObjectBatchSourceBase>();
        to.RegisterInfrastructure<ObjectManager>();
    }
}