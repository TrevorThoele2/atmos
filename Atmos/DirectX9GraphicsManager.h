#pragma once

#include "GraphicsManager.h"
#include "DirectX9Renderer.h"
#include "DirectX9Includes.h"
#undef CreateFont

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

        [[nodiscard]] bool IsOk() const override;

        void SetFullscreen(bool set) override;

        void ChangeVerticalSync(bool set) override;

        [[nodiscard]] LPDIRECT3DDEVICE9 Device() const;
    protected:
        void InitializeImpl() override;

        [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
            const Buffer& buffer, const Name& name, const Size2D& size) override;
        [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
            const Buffer& buffer, const Name& name, const String& entryPoint) override;
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateMainSurfaceDataImpl(
            void* window) override;
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateSurfaceDataImpl(
            void* window) override;

        [[nodiscard]] bool ShouldReconstructInternals() const override;
        void ReconstructInternals(GraphicsReconstructionObjects objects) override;
    private:
        HWND hwnd;
        LPDIRECT3D9 d3d;
        D3DPRESENT_PARAMETERS presentationParameters {};
    private:
        LPDIRECT3DDEVICE9 device;
        [[nodiscard]] LPDIRECT3DDEVICE9 CreateDevice();
        void SetupPresentationParameters();
        void SetRenderStates() const;

        [[nodiscard]] bool IsDeviceOk() const;
        [[nodiscard]] bool IsDeviceLost() const;
        [[nodiscard]] bool IsDeviceNotReset() const;
    private:
        Arca::Index<Asset::ShaderAsset> defaultTexturedMaterialShader{};
    private:
        static std::optional<D3DRENDERSTATETYPE> RenderStateToD3D(RenderState renderState);
    };
}