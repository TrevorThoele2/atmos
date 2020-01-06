#pragma once

#include "DirectX9Includes.h"
#include "DirectX9GraphicsManager.h"

#include "ImageAsset.h"
#include "ShaderAsset.h"
#include "Surface.h"

namespace Atmos::Render::DirectX9
{
    class ImageAssetDataImplementation final : public Asset::ImageAssetData
    {
    public:
        LPDIRECT3DTEXTURE9 tex;
    public:
        explicit ImageAssetDataImplementation(LPDIRECT3DTEXTURE9 tex);
        ~ImageAssetDataImplementation();

        [[nodiscard]] std::unique_ptr<ImageAssetData> Clone() const override;
    };

    class ShaderAssetDataImplementation final : public Asset::ShaderAssetData
    {
    public:
        LPD3DXEFFECT effect;
    public:
        explicit ShaderAssetDataImplementation(LPD3DXEFFECT effect);
        ~ShaderAssetDataImplementation();

        [[nodiscard]] std::unique_ptr<ShaderAssetData> Clone() const override;

        void Reset() override;
        void Release() override;

        PassCount Begin() const override;
        void End() const override;

        void BeginNextPass(PassCount pass) const override;
        void EndPass() const override;
    };

    class SurfaceData final : public Surface::Data
    {
    public:
        SurfaceData
        (
            GraphicsManager& owner,
            Arca::Reliquary& reliquary,
            LPDIRECT3DSURFACE9 backBuffer
        );

        SurfaceData
        (
            GraphicsManager& owner,
            Arca::Reliquary& reliquary,
            LPDIRECT3DSWAPCHAIN9 swapChain,
            LPDIRECT3DSURFACE9 backBuffer
        );

        ~SurfaceData();

        void SetAsRenderTarget() override;
        void Present() override;

        void Reset() override;
        void Release() override;

        [[nodiscard]] ScreenDimensions Size() override;

        [[nodiscard]] D3DPRESENT_PARAMETERS GetPresentationParameters() const;
    private:
        GraphicsManager* owner = nullptr;
        Arca::Reliquary* reliquary = nullptr;
    private:
        D3DPRESENT_PARAMETERS presentationParameters{};
        LPDIRECT3DSWAPCHAIN9 swapChain = nullptr;
        LPDIRECT3DSURFACE9 backBuffer = nullptr;
    private:
        void LogIfError(
            HRESULT hr,
            const String& message,
            Logging::Severity severity,
            const std::optional<Logging::Details>& details = {}) const;
    };

    class CanvasData final : public Canvas::Data
    {
    public:
        GraphicsManager* owner = nullptr;
        Arca::Reliquary* reliquary = nullptr;

        LPDIRECT3DTEXTURE9 tex = nullptr;
        LPDIRECT3DSURFACE9 backBuffer = nullptr;

        D3DLOCKED_RECT lockedRect{};

        CanvasData(GraphicsManager& owner, Arca::Reliquary& reliquary, LPDIRECT3DTEXTURE9 tex);
        ~CanvasData();

        void StartPainting() override;
        void StopPainting() override;

        void PaintPixel(
            const Canvas::Position& position,
            const Color& color,
            Canvas::DimensionValue height) override;

        void Clear(const Color& color) override;

        void Release() override;
        void Reset(Canvas::DimensionValue width, Canvas::DimensionValue height) override;
    private:
        void LogIfError(
            HRESULT hr,
            const String& message,
            Logging::Severity severity,
            const std::optional<Logging::Details>& details = {}) const;
    };
}