#include "MaterialAssetPass.h"

namespace Atmos::Asset
{
    MaterialPass::MaterialPass(Arca::Index<Shader> vertexShader, Arca::Index<Shader> fragmentShader) :
        vertexShader(vertexShader), fragmentShader(fragmentShader)
    {}

    bool MaterialPass::operator==(const MaterialPass& arg) const
    {
        return IndexEqualsValue(vertexShader, arg.vertexShader) && IndexEqualsValue(fragmentShader, arg.fragmentShader);
    }

    bool MaterialPass::operator!=(const MaterialPass& arg) const
    {
        return !(*this == arg);
    }

    Arca::Index<Shader> MaterialPass::VertexShader() const
    {
        return vertexShader;
    }

    Arca::Index<Shader> MaterialPass::FragmentShader() const
    {
        return fragmentShader;
    }
}