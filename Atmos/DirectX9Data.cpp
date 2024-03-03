#include "DirectX9Data.h"

#include "DirectX9Utilities.h"

namespace Atmos::Render::DirectX9
{
    ImageAssetDataImplementation::ImageAssetDataImplementation(LPDIRECT3DTEXTURE9 tex) : tex(tex)
    {}

    ImageAssetDataImplementation::~ImageAssetDataImplementation()
    {
        tex->Release();
    }

    std::unique_ptr<Asset::ImageAssetData> ImageAssetDataImplementation::Clone() const
    {
        return std::make_unique<ImageAssetDataImplementation>(*this);
    }

    ShaderAssetDataImplementation::ShaderAssetDataImplementation(LPD3DXEFFECT effect) : effect(effect)
    {}

    ShaderAssetDataImplementation::~ShaderAssetDataImplementation()
    {
        effect->Release();
    }

    std::unique_ptr<Asset::ShaderAssetData> ShaderAssetDataImplementation::Clone() const
    {
        return std::make_unique<ShaderAssetDataImplementation>(*this);
    }

    void ShaderAssetDataImplementation::Reset()
    {
        effect->OnResetDevice();
    }

    void ShaderAssetDataImplementation::Release()
    {
        effect->OnLostDevice();
    }

    auto ShaderAssetDataImplementation::Begin() const -> PassCount
    {
        {
            D3DXHANDLE technique;
            effect->FindNextValidTechnique(nullptr, &technique);
            effect->SetTechnique(technique);
        }

        PassCount pass;
        effect->Begin(&pass, 0);
        return pass;
    }

    void ShaderAssetDataImplementation::End() const
    {
        effect->End();
    }

    void ShaderAssetDataImplementation::BeginNextPass(PassCount pass) const
    {
        effect->BeginPass(pass);
    }

    void ShaderAssetDataImplementation::EndPass() const
    {
        effect->EndPass();
    }

    SurfaceData::SurfaceData
    (
        GraphicsManager& owner,
        Arca::Reliquary& reliquary,
        LPDIRECT3DSURFACE9 backBuffer
    ) :
        owner(&owner), reliquary(&reliquary), backBuffer(backBuffer)
    {}

    SurfaceData::SurfaceData
    (
        GraphicsManager& owner,
        Arca::Reliquary& reliquary,
        LPDIRECT3DSWAPCHAIN9 swapChain,
        LPDIRECT3DSURFACE9 backBuffer
    ) :
        owner(&owner), reliquary(&reliquary), swapChain(swapChain), backBuffer(backBuffer)
    {
        swapChain->GetPresentParameters(&presentationParameters);
    }

    SurfaceData::~SurfaceData()
    {
        Release();
    }

    void SurfaceData::SetAsRenderTarget()
    {
        owner->GetDevice()->SetRenderTarget(0, backBuffer);
    }

    void SurfaceData::Present()
    {
        LogIfError(
            swapChain->Present(nullptr, nullptr, nullptr, nullptr, 0),
            "A swap chain failed when presenting.",
            Logging::Severity::SevereError);
    }

    void SurfaceData::Reset()
    {
        presentationParameters.BackBufferWidth = 0;
        presentationParameters.BackBufferHeight = 0;

        LogIfError
        (
            owner->GetDevice()->CreateAdditionalSwapChain
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

    void SurfaceData::Release()
    {
        backBuffer->Release();
        backBuffer = nullptr;

        swapChain->Release();
        swapChain = nullptr;
    }

    ScreenDimensions SurfaceData::Size()
    {
        RECT rect;
        GetClientRect(presentationParameters.hDeviceWindow, &rect);
        return ScreenDimensions(rect.right, rect.bottom);
    }

    D3DPRESENT_PARAMETERS SurfaceData::GetPresentationParameters() const
    {
        return presentationParameters;
    }

    void SurfaceData::LogIfError(
        HRESULT hr,
        const String& message,
        Logging::Severity severity,
        const std::optional<Logging::Details>& details) const
    {
        DirectX9::LogIfError(hr, *reliquary, message, severity, details);
    }

    CanvasData::CanvasData(GraphicsManager& owner, Arca::Reliquary& reliquary, LPDIRECT3DTEXTURE9 tex) :
        owner(&owner), reliquary(&reliquary), tex(tex)
    {
        this->tex->GetSurfaceLevel(0, &backBuffer);
    }

    CanvasData::~CanvasData()
    {
        tex->Release();
    }

    void CanvasData::StartPainting()
    {
        LogIfError(
            backBuffer->LockRect(&lockedRect, nullptr, 0),
            "A canvas' back buffer could not be locked.",
            Logging::Severity::LowError);
    }

    void CanvasData::StopPainting()
    {
        LogIfError(
            backBuffer->UnlockRect(),
            "A canvas' back buffer could not be unlocked.",
            Logging::Severity::LowError);
    }

    void CanvasData::PaintPixel(
        const Canvas::Position& position,
        const Color& color,
        Canvas::DimensionValue height)
    {
        const auto row =
            reinterpret_cast<DWORD*>(reinterpret_cast<char*>(lockedRect.pBits)
                + lockedRect.Pitch * position.y);
        row[position.x] = ColorToD3D(color);
    }

    void CanvasData::Clear(const Color& color)
    {
        std::memset(lockedRect.pBits, ColorToD3D(color), lockedRect.Pitch * Owner()->Height());
    }

    void CanvasData::Release()
    {
        tex->Release();
    }

    void CanvasData::Reset(Canvas::DimensionValue width, Canvas::DimensionValue height)
    {
        D3DXCreateTexture
        (
            owner->GetDevice(),
            width,
            height,
            D3DX_DEFAULT,
            0,
            D3DFMT_X8R8G8B8,
            D3DPOOL_MANAGED,
            &tex
        );

        tex->GetSurfaceLevel(0, &backBuffer);
    }

    void CanvasData::LogIfError(
        HRESULT hr,
        const String& message,
        Logging::Severity severity,
        const std::optional<Logging::Details>& details) const
    {
        DirectX9::LogIfError(hr, *reliquary, message, severity, details);
    }
}