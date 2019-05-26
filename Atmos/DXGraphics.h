
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
    class FpsSystem;

    class DX9GraphicsManager : public GraphicsManager
    {
    private:
        class Renderer2D;
    public:
        DX9GraphicsManager(ObjectManager& objectManager, HWND hwnd, const ScreenDimensions& backbuffer, bool fullscreen);
        ~DX9GraphicsManager();

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
            TypedObjectReference<ShaderAsset> shader,
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
        DX9GraphicsManager(const DX9GraphicsManager& arg) = delete;
        DX9GraphicsManager& operator=(const DX9GraphicsManager& arg) = delete;

        void ReinitializeImpl() override;
        void SetMainDimensionsImpl(const ScreenDimensions& dimensions) override;
        ScreenDimensions GetMainDimensionsImpl() const override;

        std::unique_ptr<ImageAssetData> CreateImageDataImpl(const FilePath& path) override;
        std::unique_ptr<ImageAssetData> CreateImageDataImpl(void* buffer, std::int32_t size, const FileName& name) override;
        std::unique_ptr<ShaderAssetData> CreateShaderDataImpl(const FilePath& path) override;
        std::unique_ptr<ShaderAssetData> CreateShaderDataImpl(void* buffer, std::int32_t size, const FileName& name) override;
        RenderSurface CreateRenderSurfaceImpl(void* window) override;
        Canvas CreateCanvasImpl(const ScreenDimensions& dimensions) override;

        bool CanMakeImageImpl(const FilePath& path) const override;
        bool CanMakeImageImpl(void* buffer, std::int32_t size) const override;

        void ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions) override;

        void SetRenderTargetImpl(RenderSurface& set) override;
        void SetRenderTargetToMainImpl() override;
        void ReleaseMainRenderTarget() override;
        void ResetMainRenderTarget() override;

        void PresentImpl() override;
        void PresentImpl(void* windowOverride) override;

        void RenderSpriteImpl(SpriteReference sprite, float X, float Y) override;
        void RenderCanvasViewImpl(CanvasViewReference view, float X, float Y) override;
        void RenderUnknownFragmentImpl(RenderFragmentReference fragment, float X, float Y) override;
        void RenderLineImpl(const LineRender& line) override;

        void SetupPresentationParameters();
        void SetRenderStates();
        void SetProjectionMatrix();
        void OnResolutionChanged(const Agui::Resolution& arg);
    private:
        FpsSystem* FindFpsSystem() const;
        LoggingSystem* FindLoggingSystem() const;
    };
}