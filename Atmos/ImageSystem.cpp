
#include "ImageSystem.h"

namespace Atmos
{
    ImageSystem::ImageSystem(ObjectManager& manager) : AssetSystem(manager)
    {}

    ImageSystem::ImageSystem(const ::Inscription::Table<ImageSystem>& table) : INSCRIPTION_TABLE_GET_BASE(AssetSystem)
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ImageSystem)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;
        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }

    INSCRIPTION_DEFINE_SIMPLE_CLASS_NAME_RESOLVER(::Atmos::ImageSystem, "ImageSystem");
}