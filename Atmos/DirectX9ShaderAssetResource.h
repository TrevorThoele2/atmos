#pragma once

#include "ShaderAssetResource.h"
#include "DirectX9Includes.h"

namespace Atmos::Asset::Resource::DirectX9
{
    class Shader final : public Resource::Shader
    {
    public:
        using PassCount = unsigned int;
    public:
        explicit Shader(LPD3DXEFFECT effect);
        ~Shader();

        void Reset();
        void Release();

        PassCount Begin() const;
        void End() const;

        void BeginNextPass(PassCount pass) const;
        void EndPass() const;

        [[nodiscard]] LPD3DXEFFECT Effect() const;
    private:
        LPD3DXEFFECT effect;
    };
}