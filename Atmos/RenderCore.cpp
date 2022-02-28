#include "RenderCore.h"

namespace Atmos::Render
{
    RenderCore::RenderCore(const Material& material, const Color& color) :
        material(material), color(color)
    {}

    bool RenderCore::operator==(const RenderCore& arg) const
    {
        return material == arg.material && color == arg.color;
    }

    bool RenderCore::operator!=(const RenderCore& arg) const
    {
        return !(*this == arg);
    }
}