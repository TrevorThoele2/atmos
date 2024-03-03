#include "DirectX9ShaderAssetData.h"

namespace Atmos::Render::DirectX9
{
    ShaderAssetDataImplementation::ShaderAssetDataImplementation(LPD3DXEFFECT effect) : effect(effect)
    {}

    ShaderAssetDataImplementation::~ShaderAssetDataImplementation()
    {
        effect->Release();
    }

    std::unique_ptr<Asset::ShaderAssetData> ShaderAssetDataImplementation::Clone() const
    {
        return std::make_unique<ShaderAssetDataImplementation>(*this);
    }

    void ShaderAssetDataImplementation::Reset()
    {
        effect->OnResetDevice();
    }

    void ShaderAssetDataImplementation::Release()
    {
        effect->OnLostDevice();
    }

    auto ShaderAssetDataImplementation::Begin() const -> PassCount
    {
        {
            D3DXHANDLE technique;
            effect->FindNextValidTechnique(nullptr, &technique);
            effect->SetTechnique(technique);
        }

        PassCount pass;
        effect->Begin(&pass, 0);
        return pass;
    }

    void ShaderAssetDataImplementation::End() const
    {
        effect->End();
    }

    void ShaderAssetDataImplementation::BeginNextPass(PassCount pass) const
    {
        effect->BeginPass(pass);
    }

    void ShaderAssetDataImplementation::EndPass() const
    {
        effect->EndPass();
    }

    LPD3DXEFFECT ShaderAssetDataImplementation::Effect() const
    {
        return effect;
    }
}