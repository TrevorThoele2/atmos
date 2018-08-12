
#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#undef CreateFont

#include "GraphicsHandlerBase.h"

#include "AssetReference.h"
#include "ShaderAsset.h"

#include "Size2D.h"
#include "AxisBoundingBox2D.h"
#include "FilePath.h"
#include "Join.h"
#include <AGUI\Resolution.h>

namespace Atmos
{
    class DX9GraphicsHandler : public GraphicsHandlerBase
    {
    private:
        class Renderer2D;
    private:
        HWND hwnd;
        LPDIRECT3DDEVICE9 device;
        LPDIRECT3D9 d3d;
        D3DPRESENT_PARAMETERS presentationParameters;

        std::unique_ptr<Renderer2D> renderer2D;
        LPD3DXLINE lineInterface;

        D3DXMATRIX projection;

        LPDIRECT3DSURFACE9 mainSurface;

        DX9GraphicsHandler(const DX9GraphicsHandler &arg) = delete;
        DX9GraphicsHandler& operator=(const DX9GraphicsHandler &arg) = delete;

        void ReinitializeImpl() override;
        void SetMainDimensionsImpl(const ScreenDimensions &dimensions) override;
        ScreenDimensions GetMainDimensionsImpl() const override;

        ImageAsset CreateImageImpl(const FilePath &path, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows) override;
        ImageAsset CreateImageImpl(void *buffer, std::int32_t size, const FileName &name, ImageAsset::GridDimension cols, ImageAsset::GridDimension rows) override;
        ShaderAsset CreateShaderImpl(const FilePath &path) override;
        ShaderAsset CreateShaderImpl(void *buffer, std::int32_t size, const FileName &name) override;
        RenderSurface CreateRenderSurfaceImpl(void *window) override;
        Canvas CreateCanvasImpl(const ScreenDimensions &dimensions) override;

        bool CanMakeImageImpl(const FilePath &path) const override;
        bool CanMakeImageImpl(void *buffer, std::int32_t size) const override;

        void ResizeCanvasImpl(Canvas &canvas, const ScreenDimensions &dimensions) override;

        void SetRenderTargetImpl(RenderSurface &set) override;
        void SetRenderTargetToMainImpl() override;
        void ReleaseMainRenderTarget() override;
        void ResetMainRenderTarget() override;

        void PresentImpl() override;
        void PresentImpl(void *windowOverride) override;

        void RenderSpriteImpl(const Sprite &sprite, float X, float Y) override;
        void RenderCanvasViewImpl(const CanvasView &view, float X, float Y) override;
        void RenderUnknownFragmentImpl(const RenderFragment &fragment, float X, float Y) override;
        void RenderLineImpl(const LineRender &line) override;

        void SetupPresentationParameters();
        void SetRenderStates();
        void SetProjectionMatrix();
        void OnResolutionChanged(const Agui::Resolution &arg);
    protected:
        void RenderObject(LPDIRECT3DTEXTURE9 tex, const AssetReference<ShaderAsset> &shader, float X, float Y, float Z, const AxisBoundingBox2D &imageBounds, const Size2D &size, const Position2D &center, const Join2<float> &scaling, const Angle &rotation, const Color &color);
    public:
        DX9GraphicsHandler(HWND hwnd, const Join2<UINT> &backbuffer, bool fullscreen, bool subscribeEvents = true);
        ~DX9GraphicsHandler();

        // Setting
        void SetFullscreen(bool set) override;

        void ClearTarget(const Flags<Target> &target) override;
        void ClearTarget(const Flags<Target> &target, const Color &color) override;
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
    };
}