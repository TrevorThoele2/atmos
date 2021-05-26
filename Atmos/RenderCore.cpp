#include "RenderCore.h"

namespace Atmos::Render
{
    RenderCore::RenderCore(Arca::Index<Asset::Material> material, const Color& color) :
        material(material), color(color)
    {}
}