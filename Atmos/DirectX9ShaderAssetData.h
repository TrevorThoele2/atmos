#pragma once

#include "ShaderAssetData.h"
#include "DirectX9Includes.h"
#include "DirectX9GraphicsManager.h"

namespace Atmos::Render::DirectX9
{
    class ShaderAssetDataImplementation final : public Asset::ShaderAssetData
    {
    public:
        explicit ShaderAssetDataImplementation(LPD3DXEFFECT effect);
        ~ShaderAssetDataImplementation();

        [[nodiscard]] std::unique_ptr<ShaderAssetData> Clone() const override;

        void Reset() override;
        void Release() override;

        PassCount Begin() const override;
        void End() const override;

        void BeginNextPass(PassCount pass) const override;
        void EndPass() const override;

        [[nodiscard]] LPD3DXEFFECT Effect() const;
    private:
        LPD3DXEFFECT effect;
    };
}