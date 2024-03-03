#pragma once

#include "SurfaceData.h"
#include "DirectX9Includes.h"
#include "DirectX9GraphicsManager.h"

namespace Atmos::Render::DirectX9
{
    class SurfaceDataImplementation final : public SurfaceData
    {
    public:
        SurfaceDataImplementation
        (
            GraphicsManager& owner,
            LPDIRECT3DSWAPCHAIN9 swapChain,
            LPDIRECT3DSURFACE9 backBuffer
        );

        ~SurfaceDataImplementation();

        void FullColor(const Color& color) override;

        void Present() override;

        void Reset() override;
        void Release() override;

        [[nodiscard]] ScreenSize Size() const override;

        [[nodiscard]] D3DPRESENT_PARAMETERS PresentationParameters() const;
        [[nodiscard]] LPDIRECT3DSURFACE9 BackBuffer() const;
    private:
        GraphicsManager* owner = nullptr;
    private:
        D3DPRESENT_PARAMETERS presentationParameters{};
        LPDIRECT3DSWAPCHAIN9 swapChain = nullptr;
        LPDIRECT3DSURFACE9 backBuffer = nullptr;
    };
}