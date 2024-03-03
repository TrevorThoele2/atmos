#include "DirectX9GraphicsManager.h"

#include "WindowsInclude.h"
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;

#include "DirectX9ImageAssetData.h"
#include "DirectX9ShaderAssetData.h"
#include "DirectX9SurfaceData.h"
#include "DirectX9Utilities.h"
#include "MainSurface.h"
#include "AncillarySurface.h"
#include "GraphicsError.h"

#include "FileManagerProvider.h"

#include "TimeSettings.h"

#include "SimpleFile.h"

namespace Atmos::Render::DirectX9
{
    GraphicsManager::GraphicsManager(HWND hwnd, bool fullscreen) : hwnd(hwnd)
    {
        ZeroMemory(&presentationParameters, sizeof presentationParameters);
        SetupPresentationParameters();
        SetFullscreen(fullscreen);
        presentationParameters.hDeviceWindow = hwnd;

        d3d = Direct3DCreate9(D3D_SDK_VERSION);

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

    bool GraphicsManager::IsOk() const
    {
        return IsDeviceOk();
    }

    void GraphicsManager::SetFullscreen(bool set)
    {
        presentationParameters.Windowed = set
            ? false
            : true;
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

    std::unique_ptr<Asset::ImageData> GraphicsManager::CreateImageDataImpl(
        const Buffer& buffer,
        const Name& name,
        const Asset::ImageSize& size)
    {
        D3DXIMAGE_INFO info;
        const auto imageInfoResult = D3DXGetImageInfoFromFileInMemory(buffer.data(), buffer.size(), &info);
        if (IsError(imageInfoResult))
            throw GraphicsError("An image asset could not be created.",
                { {"Name", name } });

        LPDIRECT3DTEXTURE9 texture;
        const auto createTextureResult = D3DXCreateTextureFromFileInMemoryEx(
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
            &texture);

        if (IsError(createTextureResult))
            throw GraphicsError("An image asset could not be created.",
                { {"Name", name } });

        return std::make_unique<ImageAssetDataImplementation>(texture);
    }

    std::unique_ptr<Asset::ShaderData> GraphicsManager::CreateShaderDataImpl(
        const Buffer& buffer, const Name& name)
    {
        LPD3DXEFFECT effect;
        const auto createShaderResult = D3DXCreateEffect(
            device,
            buffer.data(),
            buffer.size(),
            nullptr,
            nullptr,
            D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER,
            nullptr,
            &effect,
            nullptr);

        if (IsError(createShaderResult))
            throw GraphicsError("A shader asset could not be created.",
                { {"Name", name } });

        return std::make_unique<ShaderAssetDataImplementation>(effect);
    }

    std::unique_ptr<SurfaceData> GraphicsManager::CreateMainSurfaceDataImpl(void* window)
    {
        LPDIRECT3DSWAPCHAIN9 swapChain;
        const auto swapChainResult = device->GetSwapChain(0, &swapChain);
        if (IsError(swapChainResult))
            throw GraphicsError("The main render surface data could not be retrieved.");

        LPDIRECT3DSURFACE9 backBuffer;
        const auto backBufferResult = swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
        if (IsError(backBufferResult))
            throw GraphicsError("The main render surface data could not be retrieved.");

        return std::make_unique<SurfaceDataImplementation>(
            *this,
            swapChain,
            backBuffer,
            false);
    }

    std::unique_ptr<SurfaceData> GraphicsManager::CreateSurfaceDataImpl(
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
        const auto createSwapChainResult = device->CreateAdditionalSwapChain(&presentationParameters, &swapChain);
        if (IsError(createSwapChainResult))
            throw GraphicsError("A render surface could not be created.");

        LPDIRECT3DSURFACE9 backBuffer;
        const auto createBackBufferResult = swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer);
        if (IsError(createBackBufferResult))
            throw GraphicsError("A render surface could not be created.");

        return std::make_unique<SurfaceDataImplementation>(
            *this,
            swapChain,
            backBuffer,
            true);
    }

    bool GraphicsManager::ShouldReconstructInternals() const
    {
        return !IsDeviceLost();
    }

    void GraphicsManager::ReconstructInternals(GraphicsReconstructionObjects objects)
    {
        for (auto& asset : objects.shaderAssets)
            asset->FileDataAs<ShaderAssetDataImplementation>()->Release();

        objects.mainSurface->Data<SurfaceDataImplementation>()->Release();

        for (auto& surface : objects.ancillarySurfaces)
            surface->Data<SurfaceDataImplementation>()->Release();

        if (!IsDeviceNotReset())
        {
            SetupPresentationParameters();
            presentationParameters.BackBufferWidth = objects.screenSize.width;
            presentationParameters.BackBufferHeight = objects.screenSize.height;
        }

        {
            const auto hr = device->Reset(&presentationParameters);
            if (IsError(hr))
            {
                LogIfError(
                    hr,
                    []() { return Logging::Log(
                        "The DirectX device failed when resetting.",
                        Logging::Severity::Error); });
                return;
            }
        }

        SetRenderStates();

        if (IsOk())
        {
            for (auto& asset : objects.shaderAssets)
                asset->FileDataAs<ShaderAssetDataImplementation>()->Reset();

            objects.mainSurface->Data<SurfaceDataImplementation>()->Reset();

            for (auto& surface : objects.ancillarySurfaces)
                surface->Data<SurfaceDataImplementation>()->Reset();
        }
    }

    LPDIRECT3DDEVICE9 GraphicsManager::CreateDevice()
    {
        LPDIRECT3DDEVICE9 device;
        const auto result = d3d->CreateDevice
        (
            D3DADAPTER_DEFAULT,
            D3DDEVTYPE_HAL,
            hwnd,
            D3DCREATE_HARDWARE_VERTEXPROCESSING,
            &presentationParameters,
            &device
        );
        if (IsError(result))
            throw GraphicsError("The Direct3D device could not be created.");
        return device;
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
            []() { return Logging::Log(
                "DirectX9 Z buffer couldn't be set to false.",
                Logging::Severity::Error); });
        LogIfError(
            device->SetRenderState(D3DRS_LIGHTING, false),
            []() { return Logging::Log(
                "DirectX9 lighting couldn't be set to false.",
                Logging::Severity::Error); });
        LogIfError(
            device->SetRenderState(D3DRS_ALPHABLENDENABLE, true),
            []() { return Logging::Log(
                "DirectX9 alpha blend couldn't be set to true.",
                Logging::Severity::Error); });
        LogIfError(
            device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA),
            []() { return Logging::Log(
                "DirectX9 source blend couldn't be set to source alpha.",
                Logging::Severity::Error); });
        LogIfError(
            device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA),
            []() { return Logging::Log(
                "DirectX9 destination blend couldn't be set to inverse source alpha.",
                Logging::Severity::Error); });
    }

    bool GraphicsManager::IsDeviceOk() const
    {
        const auto cooperativeLevel = device->TestCooperativeLevel();
        return cooperativeLevel == D3D_OK;
    }

    bool GraphicsManager::IsDeviceLost() const
    {
        const auto cooperativeLevel = device->TestCooperativeLevel();
        return cooperativeLevel == D3DERR_DEVICELOST;
    }

    bool GraphicsManager::IsDeviceNotReset() const
    {
        const auto cooperativeLevel = device->TestCooperativeLevel();
        return cooperativeLevel == D3DERR_DEVICENOTRESET;
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