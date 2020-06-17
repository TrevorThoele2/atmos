#include "DirectX9ShaderAssetResource.h"

#include "DirectX9Utilities.h"

namespace Atmos::Asset::Resource::DirectX9
{
    Shader::Shader(LPD3DXEFFECT effect) : effect(effect)
    {}

    Shader::~Shader()
    {
        effect->Release();
    }

    void Shader::Reset()
    {
        Render::DirectX9::LogIfError(
            effect->OnResetDevice(),
            []() { return Logging::Log(
                "Resetting shader failed.",
                Logging::Severity::Error); });
    }

    void Shader::Release()
    {
        Render::DirectX9::LogIfError(
            effect->OnLostDevice(),
            []() { return Logging::Log(
                "Releasing shader failed.",
                Logging::Severity::Error); });
    }

    auto Shader::Begin() const -> PassCount
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

    void Shader::End() const
    {
        effect->End();
    }

    void Shader::BeginNextPass(PassCount pass) const
    {
        effect->BeginPass(pass);
    }

    void Shader::EndPass() const
    {
        effect->EndPass();
    }

    LPD3DXEFFECT Shader::Effect() const
    {
        return effect;
    }
}