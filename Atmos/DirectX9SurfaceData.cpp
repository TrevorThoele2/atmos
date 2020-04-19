#include "DirectX9SurfaceData.h"

#include "DirectX9GraphicsManager.h"
#include "DirectX9Utilities.h"

namespace Atmos::Render::DirectX9
{
    SurfaceDataImplementation::SurfaceDataImplementation
    (
        GraphicsManager& owner,
        LPDIRECT3DSWAPCHAIN9 swapChain,
        LPDIRECT3DSURFACE9 backBuffer,
        Arca::Index<Asset::ShaderAsset> defaultTexturedMaterialShader,
        bool setAsRenderTarget,
        Arca::Reliquary& reliquary)
        :
        owner(&owner),
        renderer(std::make_unique<Renderer>(owner, defaultTexturedMaterialShader, reliquary)),
        swapChain(swapChain),
        backBuffer(backBuffer),
        setAsRenderTarget(setAsRenderTarget)
    {
        swapChain->GetPresentParameters(&presentationParameters);
    }

    SurfaceDataImplementation::~SurfaceDataImplementation()
    {
        Release();
    }

    void SurfaceDataImplementation::StageRender(const ImageRender& imageRender)
    {
        renderer->StageRender(imageRender);
    }

    void SurfaceDataImplementation::StageRender(const LineRender& lineRender)
    {
        renderer->StageRender(lineRender);
    }

    void SurfaceDataImplementation::DrawFrame(const Color& backgroundColor)
    {
        if (!setAsRenderTarget)
            DrawFrameNormal(backgroundColor);
        else
            DrawFrameAsRenderTarget(backgroundColor);
    }

    void SurfaceDataImplementation::Reset()
    {
        renderer->OnResetDevice();

        presentationParameters.BackBufferWidth = 0;
        presentationParameters.BackBufferHeight = 0;

        LogIfError
        (
            owner->Device()->CreateAdditionalSwapChain
            (
                &presentationParameters,
                &swapChain
            ),
            []() { return Logging::Log(
                "A DirectX device could not create an additional swap chain.",
                Logging::Severity::Error); }
        );

        if (swapChain)
        {
            LogIfError
            (
                swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer),
                []() { return Logging::Log(
                    "A DirectX device could not retrieve the back buffer from a swap chain.",
                    Logging::Severity::Error); }
            );
        }
    }

    void SurfaceDataImplementation::Release()
    {
        if (backBuffer)
        {
            backBuffer->Release();
            backBuffer = nullptr;
        }

        if (swapChain)
        {
            swapChain->Release();
            swapChain = nullptr;
        }

        renderer->OnLostDevice();
    }

    ScreenSize SurfaceDataImplementation::Size() const
    {
        RECT rect;
        GetClientRect(presentationParameters.hDeviceWindow, &rect);
        return ScreenSize(rect.right, rect.bottom);
    }

    D3DPRESENT_PARAMETERS SurfaceDataImplementation::PresentationParameters() const
    {
        return presentationParameters;
    }

    LPDIRECT3DSURFACE9 SurfaceDataImplementation::BackBuffer() const
    {
        return backBuffer;
    }

    void SurfaceDataImplementation::DrawFrameNormal(const Color& backgroundColor)
    {
        renderer->DrawFrame(Size(), backgroundColor);
        LogIfError(
            swapChain->Present(nullptr, nullptr, nullptr, nullptr, 0),
            []() { return Logging::Log(
                "A swap chain failed when presenting.",
                Logging::Severity::Error); });
    }

    void SurfaceDataImplementation::DrawFrameAsRenderTarget(const Color& backgroundColor)
    {
        LPDIRECT3DSURFACE9 previousRenderSurface;
        owner->Device()->GetRenderTarget(0, &previousRenderSurface);
        owner->Device()->SetRenderTarget(0, BackBuffer());

        renderer->DrawFrame(Size(), backgroundColor);

        LogIfError(
            swapChain->Present(nullptr, nullptr, nullptr, nullptr, 0),
            []() { return Logging::Log(
                "A swap chain failed when presenting.",
                Logging::Severity::Error); });

        owner->Device()->SetRenderTarget(0, previousRenderSurface);
    }
}