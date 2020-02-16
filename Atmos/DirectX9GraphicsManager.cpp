#include "DirectX9GraphicsManager.h"

#include <Windows.h>
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
#pragma comment(lib, "dxerr.lib")

#include "DirectX9Renderer.h"
#include "DirectX9ImageAssetData.h"
#include "DirectX9ShaderAssetData.h"
#include "DirectX9CanvasData.h"
#include "DirectX9SurfaceData.h"
#include "DirectX9Utilities.h"

#include "FileManagerProvider.h"

#include "TimeSettings.h"

namespace Atmos::Render::DirectX9
{
    GraphicsManager::GraphicsManager(HWND hwnd, bool fullscreen) :
        Render::GraphicsManager(std::make_unique<DirectX9::Renderer>(*this)), hwnd(hwnd)
    {
        ZeroMemory(&presentationParameters, sizeof presentationParameters);

        SetFullscreen(fullscreen);

        d3d = Direct3DCreate9(D3D_SDK_VERSION);
        presentationParameters.hDeviceWindow = hwnd;
        SetupPresentationParameters();

        device = CreateDevice();

        SetRenderStates();
    }

    GraphicsManager::~GraphicsManager()
    {
        if (d3d)
            d3d->Release();
        if (device)
            device->Release();
    }

    void GraphicsManager::Initialize(Arca::Reliquary& reliquary)
    {
        Renderer<DirectX9::Renderer>().Initialize(device, reliquary);
    }

    std::unique_ptr<Asset::ImageAssetData> GraphicsManager::CreateImageData(
        const Buffer& buffer, const Name& name)
    {
        D3DXIMAGE_INFO info;
        LogIfError(
            D3DXGetImageInfoFromFileInMemory(buffer.data(), buffer.size(), &info),
            "An image asset could not be created.",
            Logging::Severity::SevereError,
            Logging::Details{ {"Name", name} });

        LPDIRECT3DTEXTURE9 tex;
        LogIfError(
            D3DXCreateTextureFromFileInMemoryEx
            (
                device,
                buffer.data(),
                buffer.size(),
                info.Width,
                info.Height,
                D3DX_DEFAULT,
                0,
                D3DFMT_A8R8G8B8,
                D3DPOOL_MANAGED,
                D3DX_DEFAULT,
                D3DX_DEFAULT,
                0,
                nullptr,
                nullptr,
                &tex
            ),
            "An image asset could not be created.",
            Logging::Severity::SevereError,
            Logging::Details{ {"Name", name} });

        return std::make_unique<ImageAssetDataImplementation>(tex);
    }

    std::unique_ptr<Asset::ShaderAssetData> GraphicsManager::CreateShaderData(
        const Buffer& buffer, const Name& name)
    {
        LPD3DXEFFECT effect;
        LogIfError(
            D3DXCreateEffect
            (
                device,
                buffer.data(),
                buffer.size(),
                nullptr,
                nullptr,
                D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER,
                nullptr,
                &effect,
                nullptr
            ),
            "A shader asset could not be created.",
            Logging::Severity::SevereError,
            Logging::Details{ {"Name", name} });

        return std::make_unique<ShaderAssetDataImplementation>(effect);
    }

    std::unique_ptr<SurfaceData> GraphicsManager::CreateSurfaceData(
        void* window)
    {
        D3DPRESENT_PARAMETERS presentationParameters;
        ZeroMemory(&presentationParameters, sizeof presentationParameters);

        presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        presentationParameters.BackBufferCount = 1;
        presentationParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
        presentationParameters.BackBufferWidth = 0;
        presentationParameters.BackBufferHeight = 0;
        presentationParameters.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        presentationParameters.Windowed = true;
        presentationParameters.hDeviceWindow = static_cast<HWND>(window);
        presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        presentationParameters.MultiSampleQuality = 0;

        LPDIRECT3DSWAPCHAIN9 swapChain;
        LogIfError(
            device->CreateAdditionalSwapChain(&presentationParameters, &swapChain),
            "A render surface could not be created.",
            Logging::Severity::SevereError);

        LPDIRECT3DSURFACE9 backBuffer;
        LogIfError(
            swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer),
            "A render surface could not be created.",
            Logging::Severity::SevereError);

        return std::make_unique<SurfaceDataImplementation>(*this, swapChain, backBuffer);
    }

    std::unique_ptr<SurfaceData> GraphicsManager::CreateMainSurfaceData()
    {
        LPDIRECT3DSWAPCHAIN9 swapChain;
        LogIfError(
            device->GetSwapChain(0, &swapChain),
            "The main render surface data could not be retrieved.",
            Logging::Severity::SevereError);

        LPDIRECT3DSURFACE9 backBuffer;
        LogIfError(
            swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer),
            "The main render surface data could not be retrieved.",
            Logging::Severity::SevereError);

        return std::make_unique<SurfaceDataImplementation>(*this, swapChain, backBuffer);
    }

    std::unique_ptr<CanvasData> GraphicsManager::CreateCanvasData(
        const ScreenSize& size)
    {
        LPDIRECT3DTEXTURE9 texture;
        LogIfError(
            D3DXCreateTexture
            (
                device,
                size.width,
                size.height,
                D3DX_DEFAULT,
                0,
                D3DFMT_X8R8G8B8,
                D3DPOOL_MANAGED,
                &texture
            ),
            "A canvas could not be created.",
            Logging::Severity::SevereError);

        return std::make_unique<CanvasDataImplementation>(*this, size, texture);
    }

    void GraphicsManager::SetFullscreen(bool set)
    {
        presentationParameters.Windowed = set
            ? false
            : true;
    }

    void GraphicsManager::ClearStencil(const Color& color)
    {
        LogIfError(
            device->Clear(0, nullptr, D3DCLEAR_STENCIL, ToDirectXColor(color), 1.0f, 0),
            "Could not clear stencil buffer.",
            Logging::Severity::SevereError);
    }

    void GraphicsManager::SetRenderState(RenderState state, bool set)
    {
        const auto stateType = RenderStateToD3D(state);
        DEBUG_ASSERT(stateType);

        const DWORD stateValue = set
            ? TRUE
            : FALSE;

        device->SetRenderState(*stateType, stateValue);
    }

    void GraphicsManager::ChangeVerticalSync(bool set)
    {
        presentationParameters.PresentationInterval = set
            ? D3DPRESENT_INTERVAL_DEFAULT
            : D3DPRESENT_INTERVAL_IMMEDIATE;
    }

    LPDIRECT3DDEVICE9 GraphicsManager::Device() const
    {
        return device;
    }

    LPDIRECT3DDEVICE9 GraphicsManager::CreateDevice()
    {
        LPDIRECT3DDEVICE9 device;
        LogIfError(
            d3d->CreateDevice
            (
                D3DADAPTER_DEFAULT,
                D3DDEVTYPE_HAL,
                hwnd,
                D3DCREATE_HARDWARE_VERTEXPROCESSING,
                &presentationParameters,
                &device
            ),
            "The Direct3D device could not be created.",
            Logging::Severity::SevereError);
        return device;
    }

    void GraphicsManager::ReconstructInternals()
    {
        SetupPresentationParameters();

        Renderer<DirectX9::Renderer>().OnLostDevice();

        {
            const auto hr = device->Reset(&presentationParameters);
            if (hr == D3DERR_DEVICELOST)
                return;
            else
                LogIfError(
                    hr,
                    "The DirectX device failed when resetting.",
                    Logging::Severity::SevereError);
        }

        // Recreate interfaces
        Renderer<DirectX9::Renderer>().OnResetDevice();

        // Reset render states
        SetRenderStates();
    }

    void GraphicsManager::SetupPresentationParameters()
    {
        presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        presentationParameters.BackBufferCount = 1;
        presentationParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
        presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        presentationParameters.MultiSampleQuality = 0;
    }

    void GraphicsManager::SetRenderStates() const
    {
        LogIfError(
            device->SetRenderState(D3DRS_ZENABLE, false),
            "DirectX9 Z buffer couldn't be set to false.",
            Logging::Severity::SevereError);
        LogIfError(
            device->SetRenderState(D3DRS_LIGHTING, false),
            "DirectX9 lighting couldn't be set to false.",
            Logging::Severity::SevereError);
        LogIfError(
            device->SetRenderState(D3DRS_ALPHABLENDENABLE, true),
            "DirectX9 alpha blend couldn't be set to true.",
            Logging::Severity::SevereError);
        LogIfError(
            device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA),
            "DirectX9 source blend couldn't be set to source alpha.",
            Logging::Severity::SevereError);
        LogIfError(
            device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA),
            "DirectX9 destination blend couldn't be set to inverse source alpha.",
            Logging::Severity::SevereError);
    }

    std::optional<D3DRENDERSTATETYPE> GraphicsManager::RenderStateToD3D(RenderState renderState)
    {
        switch (renderState)
        {
        case RenderState::Stencil:
            return D3DRS_STENCILENABLE;
        default:
            return {};
        }
    }
}