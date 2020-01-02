#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#undef CreateFont

#include "GraphicsManager.h"

#include "ShaderAsset.h"

#include "Size2D.h"
#include "AxisAlignedBox2D.h"
#include "Angle.h"

#include "FilePath.h"

#include <AGUI/Resolution.h>

namespace Atmos::File
{
    class ManagerProvider;
}

namespace Atmos::Time
{
    class Settings;
}

namespace Atmos::Render
{
    class DirectX9GraphicsManager final : public GraphicsManager
    {
    private:
        class Renderer2D;
    public:
        DirectX9GraphicsManager
        (
            Arca::Reliquary& reliquary,
            HWND hwnd,
            const ScreenDimensions& backbuffer,
            bool fullscreen
        );
        DirectX9GraphicsManager(const DirectX9GraphicsManager& arg) = delete;
        DirectX9GraphicsManager& operator=(const DirectX9GraphicsManager& arg) = delete;
        ~DirectX9GraphicsManager();

        void SetFullscreen(bool set) override;

        void ClearTarget(const Flags<Target>& target) override;
        void ClearTarget(const Flags<Target>& target, const Color& color) override;
        void Flush() override;
        void SetRenderState(RenderState state, bool set) override;

        bool Start() override;
        void End() override;

        void StartSprites(size_t spriteCount = 0) override;
        void EndSprites() override;
        void StartLines() override;
        void EndLines() override;

        void StartStencil() override;
        void StopStencil() override;

        LPDIRECT3DDEVICE9& GetDevice();
    protected:
        struct Scalings
        {
            float x;
            float y;

            Scalings();
            Scalings(float x, float y);
        };
    protected:
        void RenderObject
        (
            LPDIRECT3DTEXTURE9 tex,
            Asset::ShaderAsset* shader,
            float X,
            float Y,
            float Z,
            const AxisAlignedBox2D& imageBounds,
            const Size2D& size,
            const Position2D& center,
            const Scalings& scalings,
            const Angle& rotation,
            const Color& color
        );
    private:
        HWND hwnd;
        LPDIRECT3DDEVICE9 device;
        LPDIRECT3D9 d3d;
        D3DPRESENT_PARAMETERS presentationParameters;

        std::unique_ptr<Renderer2D> renderer2D;
        LPD3DXLINE lineInterface;

        D3DXMATRIX projection;

        LPDIRECT3DSURFACE9 mainSurface;
    private:
        void ReconstructInternals() override;
        void SetMainDimensionsImpl(const ScreenDimensions& dimensions) override;
        [[nodiscard]] ScreenDimensions MainDimensionsImpl() const override;

        [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
            const File::Path& path) override;
        [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
            void* buffer, std::int32_t size, const File::Name& name) override;
        [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
            const File::Path& path) override;
        [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
            void* buffer, std::int32_t size, const File::Name& name) override;
        [[nodiscard]] Surface CreateSurfaceImpl(
            void* window) override;
        [[nodiscard]] Canvas CreateCanvasImpl(
            const ScreenDimensions& dimensions) override;

        [[nodiscard]] bool CanMakeImageImpl(const File::Path& path) const override;
        [[nodiscard]] bool CanMakeImageImpl(void* buffer, std::int32_t size) const override;

        void ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions) override;

        void SetRenderTargetImpl(Surface& set) override;
        void SetRenderTargetToMainImpl() override;
        void ReleaseMainRenderTarget() override;
        void ResetMainRenderTarget() override;

        void PresentImpl() override;
        void PresentImpl(void* windowOverride) override;

        void RenderMaterialViewImpl(MaterialRender& materialRender) override;
        void RenderCanvasViewImpl(CanvasRender& canvasRender) override;
        void RenderLineImpl(const Line& line) override;

        void SetupPresentationParameters();
        void SetRenderStates();
        void SetProjectionMatrix();
        void OnResolutionChanged(const Agui::Resolution& arg);
    private:
        Time::Settings* timeSettings;
    };
}