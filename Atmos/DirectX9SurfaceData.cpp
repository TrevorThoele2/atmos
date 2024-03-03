#include "DirectX9SurfaceData.h"

#include "DirectX9Utilities.h"

namespace Atmos::Render::DirectX9
{
    SurfaceDataImplementation::SurfaceDataImplementation
    (
        GraphicsManager& owner,
        LPDIRECT3DSWAPCHAIN9 swapChain,
        LPDIRECT3DSURFACE9 backBuffer
    ) :
        owner(&owner), swapChain(swapChain), backBuffer(backBuffer)
    {
        swapChain->GetPresentParameters(&presentationParameters);
    }

    SurfaceDataImplementation::~SurfaceDataImplementation()
    {
        Release();
    }

    void SurfaceDataImplementation::Present()
    {
        LogIfError(
            swapChain->Present(nullptr, nullptr, nullptr, nullptr, 0),
            "A swap chain failed when presenting.",
            Logging::Severity::SevereError);
    }

    void SurfaceDataImplementation::Reset()
    {
        presentationParameters.BackBufferWidth = 0;
        presentationParameters.BackBufferHeight = 0;

        LogIfError
        (
            owner->Device()->CreateAdditionalSwapChain
            (
                &presentationParameters,
                &swapChain
            ),
            "A DirectX device could not create an additional swap chain.",
            Logging::Severity::SevereError
        );

        LogIfError
        (
            swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer),
            "A DirectX device could not retrieve the back buffer from a swap chain.",
            Logging::Severity::SevereError
        );
    }

    void SurfaceDataImplementation::Release()
    {
        backBuffer->Release();
        backBuffer = nullptr;

        swapChain->Release();
        swapChain = nullptr;
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
}