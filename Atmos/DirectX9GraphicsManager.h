#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#undef CreateFont

#include "GraphicsManager.h"

#include "ShaderAsset.h"
#include "ObjectReference.h"

#include "Size2D.h"
#include "AxisAlignedBox2D.h"
#include "Angle.h"

#include "FilePath.h"

#include <AGUI/Resolution.h>

namespace Atmos
{
    class LoggingSystem;
}

namespace Atmos::Time
{
    class FpsSystem;
}

namespace Atmos::Render
{
    class DirectX9GraphicsManager : public GraphicsManager
    {
    private:
        class Renderer2D;
    public:
        DirectX9GraphicsManager(
            ObjectManager& objectManager,
            HWND hwnd,
            const ScreenDimensions& backbuffer,
            bool fullscreen);
        ~DirectX9GraphicsManager();

        // Setting
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

        // Stencil
        void StartStencil() override;
        void StopStencil() override;

        // Retrieval
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
        void RenderObject(
            LPDIRECT3DTEXTURE9 tex,
            TypedObjectReference<Asset::ShaderAsset> shader,
            float X,
            float Y,
            float Z,
            const AxisAlignedBox2D& imageBounds,
            const Size2D& size,
            const Position2D& center,
            const Scalings& scalings,
            const Angle& rotation,
            const Color& color);
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
        DirectX9GraphicsManager(const DirectX9GraphicsManager& arg) = delete;
        DirectX9GraphicsManager& operator=(const DirectX9GraphicsManager& arg) = delete;

        void ReinitializeImpl() override;
        void SetMainDimensionsImpl(const ScreenDimensions& dimensions) override;
        ScreenDimensions GetMainDimensionsImpl() const override;

        std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
            const File::Path& path) override;
        std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
            void* buffer, std::int32_t size, const File::Name& name) override;
        std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
            const File::Path& path) override;
        std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
            void* buffer, std::int32_t size, const File::Name& name) override;
        Surface CreateSurfaceImpl(
            void* window) override;
        Canvas CreateCanvasImpl(
            const ScreenDimensions& dimensions) override;

        bool CanMakeImageImpl(const File::Path& path) const override;
        bool CanMakeImageImpl(void* buffer, std::int32_t size) const override;

        void ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions) override;

        void SetRenderTargetImpl(Surface& set) override;
        void SetRenderTargetToMainImpl() override;
        void ReleaseMainRenderTarget() override;
        void ResetMainRenderTarget() override;

        void PresentImpl() override;
        void PresentImpl(void* windowOverride) override;

        void RenderSpriteImpl(SpriteReference sprite, float X, float Y) override;
        void RenderCanvasViewImpl(CanvasViewReference view, float X, float Y) override;
        void RenderUnknownFragmentImpl(RenderFragmentReference fragment, float X, float Y) override;
        void RenderLineImpl(const Line& line) override;

        void SetupPresentationParameters();
        void SetRenderStates();
        void SetProjectionMatrix();
        void OnResolutionChanged(const Agui::Resolution& arg);
    private:
        Time::FpsSystem* FindFpsSystem() const;
        LoggingSystem* FindLoggingSystem() const;
    };
}