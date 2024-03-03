#include "DirectX9ShaderAssetData.h"

#include "DirectX9Utilities.h"

namespace Atmos::Render::DirectX9
{
    ShaderAssetDataImplementation::ShaderAssetDataImplementation(LPD3DXEFFECT effect) : effect(effect)
    {}

    ShaderAssetDataImplementation::~ShaderAssetDataImplementation()
    {
        effect->Release();
    }

    void ShaderAssetDataImplementation::Reset()
    {
        LogIfError(
            effect->OnResetDevice(),
            []() { return Logging::Log(
                "Resetting shader failed.",
                Logging::Severity::Error); });
    }

    void ShaderAssetDataImplementation::Release()
    {
        LogIfError(
            effect->OnLostDevice(),
            []() { return Logging::Log(
                "Releasing shader failed.",
                Logging::Severity::Error); });
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