#include "DirectX9CanvasData.h"

#include "DirectX9Utilities.h"

namespace Atmos::Render::DirectX9
{
    CanvasDataImplementation::CanvasDataImplementation(
        GraphicsManager& owner,
        ScreenSize canvasSize,
        LPDIRECT3DTEXTURE9 texture)
        :
        owner(&owner), canvasSize(canvasSize), texture(texture)
    {
        this->texture->GetSurfaceLevel(0, &backBuffer);
    }

    CanvasDataImplementation::~CanvasDataImplementation()
    {
        texture->Release();
    }

    void CanvasDataImplementation::Resize(ScreenSize canvasSize)
    {
        LPDIRECT3DTEXTURE9 newTexture;
        LogIfError(
            D3DXCreateTexture
            (
                owner->Device(),
                canvasSize.width,
                canvasSize.height,
                D3DX_DEFAULT,
                D3DUSAGE_RENDERTARGET,
                D3DFMT_X8R8G8B8,
                D3DPOOL_DEFAULT,
                &newTexture
            ),
            "Resizing of a canvas was attempted and failed.",
            Logging::Severity::SevereError);

        texture->Release();
        texture = newTexture;
    }

    void CanvasDataImplementation::StartPainting()
    {
        LogIfError(
            backBuffer->LockRect(&lockedRect, nullptr, 0),
            "A canvas' back buffer could not be locked.",
            Logging::Severity::LowError);
    }

    void CanvasDataImplementation::StopPainting()
    {
        LogIfError(
            backBuffer->UnlockRect(),
            "A canvas' back buffer could not be unlocked.",
            Logging::Severity::LowError);
    }

    void CanvasDataImplementation::PaintPixel(
        const ScreenPosition& position,
        const Color& color,
        ScreenPosition::Value height)
    {
        const auto row =
            reinterpret_cast<DWORD*>(reinterpret_cast<char*>(lockedRect.pBits)
                + lockedRect.Pitch * position.y);
        row[position.x] = ToDirectXColor(color);
    }

    void CanvasDataImplementation::Clear(const Color& color)
    {
        std::memset(lockedRect.pBits, ToDirectXColor(color), lockedRect.Pitch * canvasSize.height);
    }

    void CanvasDataImplementation::Release()
    {
        texture->Release();
    }

    void CanvasDataImplementation::Reset(ScreenSize size)
    {
        D3DXCreateTexture
        (
            owner->Device(),
            size.width,
            size.height,
            D3DX_DEFAULT,
            0,
            D3DFMT_X8R8G8B8,
            D3DPOOL_MANAGED,
            &texture
        );

        texture->GetSurfaceLevel(0, &backBuffer);
    }

    LPDIRECT3DTEXTURE9 CanvasDataImplementation::Texture() const
    {
        return texture;
    }
}