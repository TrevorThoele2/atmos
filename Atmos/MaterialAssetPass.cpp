#include "MaterialAssetPass.h"

namespace Atmos::Asset
{
    MaterialPass::MaterialPass(Arca::Index<Shader> vertexShader, Arca::Index<Shader> fragmentShader) :
        vertexShader(vertexShader), fragmentShader(fragmentShader)
    {}

    Arca::Index<Shader> MaterialPass::VertexShader() const
    {
        return vertexShader;
    }

    Arca::Index<Shader> MaterialPass::FragmentShader() const
    {
        return fragmentShader;
    }
}