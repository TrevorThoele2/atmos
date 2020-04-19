#pragma once

#include "ImageAsset.h"
#include "ShaderAsset.h"
#include "SurfaceData.h"

#include "ScreenSize.h"

#include "ImageRender.h"
#include "LineRender.h"

#include "GraphicsReconstructionObjects.h"

#include "Buffer.h"

namespace Atmos::Render
{
    class MainSurface;
    class AncillarySurface;

    class GraphicsManager
    {
    public:
        using Dimension = unsigned int;

        enum class RenderState
        {
            Stencil
        };
    public:
        virtual ~GraphicsManager() = 0;

        void Initialize(Arca::Reliquary& reliquary, void* mainWindow);

        [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageData(
            const Buffer& buffer, const Name& name, const Size2D& size);
        [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderData(
            const Buffer& buffer, const Name& name, const String& entryPoint);
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateMainSurfaceData(
            void* window);
        [[nodiscard]] std::unique_ptr<SurfaceData> CreateSurfaceData(
            void* window);

        void Reconstruct(GraphicsReconstructionObjects objects);
        [[nodiscard]] bool ShouldReconstruct() const;

        [[nodiscard]] virtual bool IsOk() const = 0;

        virtual void SetFullscreen(bool set) = 0;

        virtual void ChangeVerticalSync(bool set) = 0;
    protected:
        GraphicsManager() = default;

        [[nodiscard]] Arca::Reliquary& Reliquary();
        [[nodiscard]] const Arca::Reliquary& Reliquary() const;
    protected:
        virtual void InitializeImpl() = 0;

        [[nodiscard]] virtual std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
            const Buffer& buffer, const Name& name, const Size2D& size) = 0;
        [[nodiscard]] virtual std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
            const Buffer& buffer, const Name& name, const String& entryPoint) = 0;
        [[nodiscard]] virtual std::unique_ptr<SurfaceData> CreateMainSurfaceDataImpl(
            void* window) = 0;
        [[nodiscard]] virtual std::unique_ptr<SurfaceData> CreateSurfaceDataImpl(
            void* window) = 0;

        [[nodiscard]] virtual bool ShouldReconstructInternals() const = 0;
        virtual void ReconstructInternals(GraphicsReconstructionObjects objects) = 0;
    private:
        Arca::Reliquary* reliquary;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Render::GraphicsManager>
    {
        static inline const TypeName typeName = "GraphicsManager";
    };
}