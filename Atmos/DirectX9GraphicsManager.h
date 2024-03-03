#pragma once

#include "DirectX9Includes.h"
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#undef CreateFont

#include "GraphicsManager.h"

namespace Atmos::File
{
    class ManagerProvider;
}

namespace Atmos::Render::DirectX9
{
    class Renderer;

    class GraphicsManager final : public Render::GraphicsManager
    {
    public:
        GraphicsManager(HWND hwnd, bool fullscreen);
        GraphicsManager(const GraphicsManager& arg) = delete;
        GraphicsManager& operator=(const GraphicsManager& arg) = delete;
        ~GraphicsManager();

        void Initialize(Arca::Reliquary& reliquary) override;

        [[nodiscard]] bool IsOk() const override;

        [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageData(
            const Buffer& buffer, const Name& name) override;
        [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderData(
            const Buffer& buffer, const Name& name) override;
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateMainSurfaceData() override;
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateSurfaceData(
            void* window) override;
        [[nodiscard]] std::unique_ptr<CanvasData> CreateCanvasData(
            const ScreenSize& size) override;

        void SetFullscreen(bool set) override;

        void ClearStencil(const Color& color = Color()) override;
        void SetRenderState(RenderState state, bool set) override;

        void ChangeVerticalSync(bool set) override;

        [[nodiscard]] LPDIRECT3DDEVICE9 Device() const;
    protected:
        [[nodiscard]] bool ShouldReconstructInternals() const override;
        void ReconstructInternals(const ScreenSize& screenSize) override;
    private:
        HWND hwnd;
        LPDIRECT3D9 d3d;
        D3DPRESENT_PARAMETERS presentationParameters;
    private:
        LPDIRECT3DDEVICE9 device;
        [[nodiscard]] LPDIRECT3DDEVICE9 CreateDevice();
        void SetupPresentationParameters();
        void SetRenderStates() const;

        [[nodiscard]] bool IsDeviceOk() const;
        [[nodiscard]] bool IsDeviceLost() const;
        [[nodiscard]] bool IsDeviceNotReset() const;
    private:
        static std::optional<D3DRENDERSTATETYPE> RenderStateToD3D(RenderState renderState);
    };
}