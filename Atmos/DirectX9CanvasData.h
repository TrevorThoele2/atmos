#pragma once

#include "Canvas.h"
#include "DirectX9Includes.h"
#include "DirectX9GraphicsManager.h"

namespace Atmos::Render::DirectX9
{
    class CanvasDataImplementation final : public CanvasData
    {
    public:
        CanvasDataImplementation(
            GraphicsManager& owner,
            ScreenSize canvasSize,
            LPDIRECT3DTEXTURE9 texture);
        ~CanvasDataImplementation();

        void Resize(ScreenSize canvasSize) override;

        void StartPainting() override;
        void StopPainting() override;
        void PaintPixel(
            const ScreenPosition& position,
            const Color& color,
            ScreenPosition::Value height) override;
        void Clear(const Color& color) override;

        void Release() override;
        void Reset(ScreenSize size) override;

        LPDIRECT3DTEXTURE9 Texture() const;
    private:
        GraphicsManager* owner = nullptr;

        ScreenSize canvasSize;
    private:
        LPDIRECT3DTEXTURE9 texture = nullptr;
        LPDIRECT3DSURFACE9 backBuffer = nullptr;
        D3DLOCKED_RECT lockedRect{};
    };
}