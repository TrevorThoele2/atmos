#pragma once

#include "SurfaceResource.h"
#include "DirectX9Includes.h"
#include "DirectX9Renderer.h"

namespace Atmos::Render::Resource::DirectX9
{
    using namespace Render::DirectX9;

    class Surface final : public Resource::Surface
    {
    public:
        Surface(
            DirectX9::GraphicsManager& owner,
            LPDIRECT3DSWAPCHAIN9 swapChain,
            LPDIRECT3DSURFACE9 backBuffer,
            bool setAsRenderTarget);

        ~Surface();

        void StageRender(const ImageRender& imageRender) override;
        void StageRender(const LineRender& lineRender) override;
        void StageRender(const RegionRender& regionRender) override;

        void DrawFrame(Arca::Reliquary& reliquary, const Color& backgroundColor) override;

        void Reset();
        void Release();

        [[nodiscard]] Spatial::ScreenSize Size() const override;

        [[nodiscard]] D3DPRESENT_PARAMETERS PresentationParameters() const;
        [[nodiscard]] LPDIRECT3DSURFACE9 BackBuffer() const;
    private:
        void DrawFrameNormal(const Color& backgroundColor);
        void DrawFrameAsRenderTarget(const Color& backgroundColor);
    private:
        DirectX9::GraphicsManager* owner = nullptr;
        std::unique_ptr<Renderer> renderer{};
    private:
        D3DPRESENT_PARAMETERS presentationParameters{};
        LPDIRECT3DSWAPCHAIN9 swapChain = nullptr;
        LPDIRECT3DSURFACE9 backBuffer = nullptr;
    private:
        bool setAsRenderTarget = false;
    };
}