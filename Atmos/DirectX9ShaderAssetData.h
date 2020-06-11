#pragma once

#include "ShaderAssetData.h"
#include "DirectX9Includes.h"

namespace Atmos::Render::DirectX9
{
    class ShaderAssetDataImplementation final : public Asset::ShaderData
    {
    public:
        using PassCount = unsigned int;
    public:
        explicit ShaderAssetDataImplementation(LPD3DXEFFECT effect);
        ~ShaderAssetDataImplementation();

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