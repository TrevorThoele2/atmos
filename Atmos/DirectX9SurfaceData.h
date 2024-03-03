#pragma once

#include "SurfaceData.h"
#include "DirectX9Includes.h"
#include "DirectX9Renderer.h"

namespace Atmos::Render::DirectX9
{
    class SurfaceDataImplementation final : public SurfaceData
    {
    public:
        SurfaceDataImplementation(
            GraphicsManager& owner,
            LPDIRECT3DSWAPCHAIN9 swapChain,
            LPDIRECT3DSURFACE9 backBuffer,
            bool setAsRenderTarget);

        ~SurfaceDataImplementation();

        void StageRender(const ImageRender& imageRender) override;
        void StageRender(const LineRender& lineRender) override;

        void DrawFrame(Arca::Reliquary& reliquary, const Color& backgroundColor) override;

        void Reset();
        void Release();

        [[nodiscard]] ScreenSize Size() const override;

        [[nodiscard]] D3DPRESENT_PARAMETERS PresentationParameters() const;
        [[nodiscard]] LPDIRECT3DSURFACE9 BackBuffer() const;
    private:
        void DrawFrameNormal(const Color& backgroundColor);
        void DrawFrameAsRenderTarget(const Color& backgroundColor);
    private:
        GraphicsManager* owner = nullptr;
        std::unique_ptr<Renderer> renderer;
    private:
        D3DPRESENT_PARAMETERS presentationParameters{};
        LPDIRECT3DSWAPCHAIN9 swapChain = nullptr;
        LPDIRECT3DSURFACE9 backBuffer = nullptr;
    private:
        bool setAsRenderTarget = false;
    };
}