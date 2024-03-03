#pragma once

#include "DirectX9Includes.h"
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#undef CreateFont

#include "GraphicsManager.h"

#include "ShaderAsset.h"

#include "Size2D.h"
#include "AxisAlignedBox2D.h"
#include "Angle.h"
#include "Scalers2D.h"

#include "FilePath.h"

#include <AGUI/Resolution.h>

#include "LoggingSeverity.h"
#include "LoggingDetails.h"

namespace Atmos::File
{
    class ManagerProvider;
}

namespace Atmos::Time
{
    class Settings;
}

namespace Atmos::Render::DirectX9
{
    class Renderer2D;

    class GraphicsManager final : public Render::GraphicsManager
    {
    public:
        GraphicsManager
        (
            Arca::Reliquary& reliquary,
            HWND hwnd,
            const ScreenDimensions& backbuffer,
            bool fullscreen
        );
        GraphicsManager(const GraphicsManager& arg) = delete;
        GraphicsManager& operator=(const GraphicsManager& arg) = delete;
        ~GraphicsManager();

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
            const Scalers2D& scalers,
            const Angle& rotation,
            const Color& color
        ) const;
    private:
        HWND hwnd;
        LPDIRECT3D9 d3d;
        D3DPRESENT_PARAMETERS presentationParameters;

        std::unique_ptr<Renderer2D> renderer2D;
        LPD3DXLINE lineInterface;

        D3DXMATRIX projection;

        LPDIRECT3DSURFACE9 mainSurface;
    private:
        LPDIRECT3DDEVICE9 device;
        void CreateDevice();
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
        void SetRenderStates() const;
        void SetProjectionMatrix();
        void OnResolutionChanged(const Agui::Resolution& arg);
    private:
        static std::optional<D3DRENDERSTATETYPE> RenderStateToD3D(RenderState renderState);
    private:
        void LogIfError(
            HRESULT hr,
            const String& message,
            Logging::Severity severity,
            const std::optional<Logging::Details>& details = {}) const;
    private:
        Arca::GlobalPtr<Time::Settings> timeSettings;
    };
}