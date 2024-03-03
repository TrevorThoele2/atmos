#include "DirectX9GraphicsManager.h"

#include <Windows.h>
#include <stdio.h>
int (WINAPIV * __vsnprintf)(char *, size_t, const char*, va_list) = _vsnprintf;
#pragma comment(lib, "dxerr.lib")

#include "DirectX9Renderer.h"
#include "DirectX9Data.h"
#include "DirectX9Utilities.h"

#include "ImageAsset.h"

#include "CanvasView.h"
#include "Line.h"
#include "Camera.h"
#include "FileManagerProvider.h"

#include "TimeSettings.h"

#include "Flags.h"

namespace Atmos::Render::DirectX9
{
    GraphicsManager::GraphicsManager
    (
        Arca::Reliquary& reliquary,
        HWND hwnd,
        const ScreenDimensions& backbuffer,
        bool fullscreen
    ) :
        Render::GraphicsManager(reliquary),
        hwnd(hwnd),
        renderer(std::make_unique<Renderer>(reliquary)),
        timeSettings(reliquary)
    {
        ZeroMemory(&presentationParameters, sizeof presentationParameters);

        SetMainDimensions(backbuffer);
        SetFullscreen(fullscreen);

        d3d = Direct3DCreate9(D3D_SDK_VERSION);
        presentationParameters.hDeviceWindow = hwnd;
        SetupPresentationParameters();

        CreateDevice();

        renderer->Initialize(*this, device);

        SetRenderStates();

        device->GetRenderTarget(0, &mainSurface);
    }

    GraphicsManager::~GraphicsManager()
    {
        d3d->Release();
        device->Release();
    }

    void GraphicsManager::SetFullscreen(bool set)
    {
        presentationParameters.Windowed = set
            ? false
            : true;
    }

    void GraphicsManager::ClearTarget(const Flags<Target>& target)
    {
        DWORD pass = 0;
        if (target.Get(Target::Main))
            pass |= D3DCLEAR_TARGET;

        if (target.Get(Target::Stencil))
            pass |= D3DCLEAR_STENCIL;

        device->Clear(0, nullptr, pass, 0, 1.0f, 0);
    }

    void GraphicsManager::ClearTarget(const Flags<Target>& target, const Color& color)
    {
        DWORD pass = 0;
        if (target.Get(Target::Main))
            pass |= D3DCLEAR_TARGET;

        if (target.Get(Target::Stencil))
            pass |= D3DCLEAR_STENCIL;

        device->Clear(0, nullptr, pass, ColorToD3D(color), 1.0f, 0);
    }

    void GraphicsManager::Flush()
    {
        renderer->Flush(projection);
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

    bool GraphicsManager::Start()
    {
        const auto cooperativeLevelHr = device->TestCooperativeLevel();

        // Device is lost but can't reset it
        if (cooperativeLevelHr == D3DERR_DEVICELOST)
            return false;

        // Device is lost, and can reset
        if (cooperativeLevelHr == D3DERR_DEVICENOTRESET)
            ReconstructAll();

        device->BeginScene();
        return true;
    }

    void GraphicsManager::Stop()
    {
        device->EndScene();
    }

    void GraphicsManager::StartObjects(size_t spriteCount)
    {
        renderer->StartObjects(spriteCount);
    }

    void GraphicsManager::StopObjects()
    {
        if (projection)
            renderer->Flush(projection);

        renderer->StopObjects();
    }

    void GraphicsManager::StartLines()
    {
        renderer->StartLines();
    }

    void GraphicsManager::StopLines()
    {
        renderer->StopLines();
    }

    void GraphicsManager::StartStencil()
    {
        device->SetRenderState(D3DRS_STENCILENABLE, TRUE);
        device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_NEVER);

        device->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_REPLACE);
        device->SetRenderState(D3DRS_STENCILREF, 1);
        device->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
    }

    void GraphicsManager::StopStencil()
    {
        Flush();

        device->SetRenderState(D3DRS_STENCILENABLE, FALSE);
        device->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

        device->SetRenderState(D3DRS_STENCILREF, 0);
        device->SetRenderState(D3DRS_STENCILWRITEMASK, 0);
    }

    LPDIRECT3DDEVICE9& GraphicsManager::Device()
    {
        return device;
    }

    void GraphicsManager::RenderObject
    (
        LPDIRECT3DTEXTURE9 tex,
        Asset::ShaderAsset* shader,
        float X,
        float Y,
        float Z,
        const AxisAlignedBox2D& imageBounds,
        const Size2D& size,
        const Position2D& center,
        const Scalers2D& scalers,
        const Angle& rotation,
        const Color& color
    ) const {
        if (!shader)
            shader = renderer->DefaultTexturedImageViewShader();

        renderer->Draw(tex, &*shader, X, Y, Z, imageBounds, size, center, scalers, rotation, color);
    }

    void GraphicsManager::CreateDevice()
    {
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
    }

    void GraphicsManager::ReconstructInternals()
    {
        SetupPresentationParameters();

        // Destroy interfaces
        renderer->OnLostDevice();

        // Reset
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
        renderer->OnResetDevice();

        // Reset render states
        SetRenderStates();
    }

    void GraphicsManager::SetMainDimensionsImpl(const ScreenDimensions& dimensions)
    {
        presentationParameters.BackBufferWidth = dimensions.width;
        presentationParameters.BackBufferHeight = dimensions.height;
        SetProjectionMatrix();
    }

    ScreenDimensions GraphicsManager::MainDimensionsImpl() const
    {
        return ScreenDimensions(presentationParameters.BackBufferWidth, presentationParameters.BackBufferHeight);
    }

    std::unique_ptr<Asset::ImageAssetData> GraphicsManager::CreateImageDataImpl(
        const File::Path& path)
    {
        D3DXIMAGE_INFO info;
        LogIfError(
            D3DXGetImageInfoFromFile(path.c_str(), &info),
            "An image asset could not be created.",
            Logging::Severity::SevereError,
            Logging::Details{ {"File Path", path.GetValue()} });

        LPDIRECT3DTEXTURE9 tex;
        LogIfError(
            D3DXCreateTextureFromFileEx
            (
                device,
                path.c_str(),
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
            Logging::Details{ {"File Path", path.GetValue()} });

        return std::make_unique<ImageAssetDataImplementation>(tex);
    }

    std::unique_ptr<Asset::ImageAssetData> GraphicsManager::CreateImageDataImpl(
        void* buffer, std::int32_t size, const File::Name& name)
    {
        D3DXIMAGE_INFO info;
        LogIfError(
            D3DXGetImageInfoFromFileInMemory(buffer, size, &info),
            "An image asset could not be created.",
            Logging::Severity::SevereError,
            Logging::Details{ {"File Name", name.GetValue()} });
        
        LPDIRECT3DTEXTURE9 tex;
        LogIfError(
            D3DXCreateTextureFromFileInMemoryEx
            (
                device,
                buffer,
                size,
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
            Logging::Details{ {"File Name", name.GetValue()} });

        return std::make_unique<ImageAssetDataImplementation>(tex);
    }

    std::unique_ptr<Asset::ShaderAssetData> GraphicsManager::CreateShaderDataImpl(
        const File::Path& path)
    {
        LPD3DXEFFECT effect;
        LogIfError(
            D3DXCreateEffectFromFile
            (
                device,
                path.c_str(),
                nullptr,
                nullptr,
                D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER,
                nullptr,
                &effect,
                nullptr
            ),
            "A shader asset could not be created.",
            Logging::Severity::SevereError,
            Logging::Details{ {"File Path", path.GetValue()} });

        return std::make_unique<ShaderAssetDataImplementation>(effect);
    }

    std::unique_ptr<Asset::ShaderAssetData> GraphicsManager::CreateShaderDataImpl(
        void* buffer, std::int32_t size, const File::Name& name)
    {
        LPD3DXEFFECT effect;
        LogIfError(
            D3DXCreateEffect
            (
                device,
                buffer,
                size,
                nullptr,
                nullptr,
                D3DXFX_NOT_CLONEABLE | D3DXSHADER_NO_PRESHADER,
                nullptr,
                &effect,
                nullptr
            ),
            "A shader asset could not be created.",
            Logging::Severity::SevereError,
            Logging::Details{ {"File Path", name.GetValue()} });

        return std::make_unique<ShaderAssetDataImplementation>(effect);
    }

    Surface GraphicsManager::CreateSurfaceImpl(
        void* window)
    {
        D3DPRESENT_PARAMETERS pp;
        ZeroMemory(&pp, sizeof(pp));

        pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        pp.BackBufferCount = 1;
        pp.BackBufferFormat = D3DFMT_A8R8G8B8;
        pp.BackBufferWidth = 0;
        pp.BackBufferHeight = 0;
        pp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
        pp.Windowed = true;
        pp.hDeviceWindow = static_cast<HWND>(window);
        pp.MultiSampleType = D3DMULTISAMPLE_NONE;
        pp.MultiSampleQuality = 0;

        LPDIRECT3DSWAPCHAIN9 swapChain;
        LogIfError(
            device->CreateAdditionalSwapChain(&pp, &swapChain),
            "A render surface could not be created.",
            Logging::Severity::SevereError);

        LPDIRECT3DSURFACE9 backBuffer;
        LogIfError(
            swapChain->GetBackBuffer(0, D3DBACKBUFFER_TYPE_MONO, &backBuffer),
            "A render surface could not be created.",
            Logging::Severity::SevereError);

        return Surface(std::make_unique<SurfaceData>(*this, *reliquary, swapChain, backBuffer));
    }

    Canvas GraphicsManager::CreateCanvasImpl(
        const ScreenDimensions& dimensions)
    {
        LPDIRECT3DTEXTURE9 tex;
        LogIfError(
            D3DXCreateTexture
            (
                device,
                dimensions.width,
                dimensions.height,
                D3DX_DEFAULT,
                0,
                D3DFMT_X8R8G8B8,
                D3DPOOL_MANAGED,
                &tex
            ),
            "A canvas could not be created.",
            Logging::Severity::SevereError);

        auto data = std::make_unique<CanvasData>(*this, *reliquary, tex);
        return Canvas(std::move(data), dimensions.width, dimensions.height);
    }

    bool GraphicsManager::CanMakeImageImpl(const File::Path& path) const
    {
        D3DXIMAGE_INFO info;
        return D3DXGetImageInfoFromFile(path.c_str(), &info) == S_OK;
    }

    bool GraphicsManager::CanMakeImageImpl(void* buffer, std::int32_t size) const
    {
        D3DXIMAGE_INFO info;
        return D3DXGetImageInfoFromFileInMemory(buffer, size, &info) == S_OK;
    }

    void GraphicsManager::ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions)
    {
        LPDIRECT3DTEXTURE9 tex;
        LogIfError(
            D3DXCreateTexture
            (
                device,
                dimensions.width,
                dimensions.height,
                D3DX_DEFAULT,
                D3DUSAGE_RENDERTARGET,
                D3DFMT_X8R8G8B8,
                D3DPOOL_DEFAULT,
                &tex
            ),
            "Resizing of a canvas was attempted and failed.",
            Logging::Severity::SevereError);

        canvas.GetData<CanvasData>()->tex->Release();
        canvas.GetData<CanvasData>()->tex = tex;
    }

    void GraphicsManager::SetRenderTargetImpl(Surface& set)
    {
        SetProjectionMatrix();
    }

    void GraphicsManager::SetRenderTargetToMainImpl()
    {
        device->SetRenderTarget(0, mainSurface);
        SetProjectionMatrix();
    }

    void GraphicsManager::ReleaseMainRenderTarget()
    {
        mainSurface->Release();
    }

    void GraphicsManager::ResetMainRenderTarget()
    {
        LogIfError(
            device->GetRenderTarget(0, &mainSurface),
            "The main render target is not retrievable.",
            Logging::Severity::SevereError);
    }

    void GraphicsManager::PresentImpl()
    {
        LogIfError(
            device->Present(nullptr, nullptr, nullptr, nullptr),
            "The frame is not presentable.",
            Logging::Severity::SevereError);
    }

    void GraphicsManager::PresentImpl(void* windowOverride)
    {
        LogIfError(
            device->Present(nullptr, nullptr, static_cast<HWND>(windowOverride), nullptr),
            "The frame is not presentable.",
            Logging::Severity::SevereError);
    }

    void GraphicsManager::RenderMaterialViewImpl(MaterialRender& materialRender)
    {
        if (!materialRender.material)
            return;

        const auto material = materialRender.material;
        auto image = material->Image();
        const auto materialSlice = materialRender.materialSlice;
        const auto size = materialRender.size;
        const auto position = materialRender.position;

        RenderObject
        (
            image->DataAs<ImageAssetDataImplementation>()->tex,
            materialRender.patchShader ? materialRender.patchShader : material->Shader(),
            position.x,
            position.y,
            materialRender.position.z,
            materialSlice,
            materialSlice.Size(),
            materialSlice.Center(),
            Scalers2D{ size.width, size.height },
            Angle(),
            materialRender.color
        );
    }

    void GraphicsManager::RenderCanvasViewImpl(CanvasRender& canvasRender)
    {
        const auto position = canvasRender.position;
        const auto size = canvasRender.size;

        RenderObject
        (
            canvasRender.canvas.GetData<CanvasData>()->tex,
            renderer->DefaultTexturedImageViewShader(),
            position.x,
            position.y,
            position.z,
            AxisAlignedBox2D{ 0.0f, 0.0f, 1.0f, 1.0f },
            Size2D{ size.width, size.height },
            Position2D{ size.width / 2, size.height / 2 },
            Scalers2D{ 1.0f, 1.0f },
            Angle(),
            Color(255, 255, 255, 255)
        );
    }

    void GraphicsManager::RenderLineImpl(const Line& line)
    {
        renderer->Draw(line.from, line.to, line.width, line.color);
    }

    void GraphicsManager::SetupPresentationParameters()
    {
        presentationParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
        presentationParameters.BackBufferCount = 1;
        presentationParameters.BackBufferFormat = D3DFMT_A8R8G8B8;
        presentationParameters.MultiSampleType = D3DMULTISAMPLE_NONE;
        presentationParameters.MultiSampleQuality = 0;

        presentationParameters.PresentationInterval = timeSettings->verticalSync
            ? D3DPRESENT_INTERVAL_DEFAULT
            : D3DPRESENT_INTERVAL_IMMEDIATE;
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

    void GraphicsManager::SetProjectionMatrix()
    {
        const auto dimensions(CurrentDimensions());
        D3DXMatrixOrthoOffCenterLH
        (
            &projection,
            0,
            static_cast<FLOAT>(dimensions.width),
            static_cast<FLOAT>(dimensions.height),
            0,
            0.0f,
            1.0f
        );
    }

    void GraphicsManager::OnResolutionChanged(const Agui::Resolution& arg)
    {
        SetMainDimensions(ScreenDimensions
        (
            static_cast<Dimension>(arg.GetWidth()),
            static_cast<Dimension>(arg.GetHeight())
        ));
        ReconstructAll();
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

    void GraphicsManager::LogIfError(
        HRESULT hr,
        const String& message,
        Logging::Severity severity,
        const std::optional<Logging::Details>& details) const
    {
        DirectX9::LogIfError(hr, *reliquary, message, severity, details);
    }
}