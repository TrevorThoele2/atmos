#pragma once

#include <cstdint>
#include <list>

#include "ImageAsset.h"
#include "ShaderAsset.h"
#include "Surface.h"
#include "Canvas.h"

#include "ScreenDimensions.h"

#include "MaterialRender.h"
#include "CanvasRender.h"

#include "FilePath.h"
#include "Flags.h"

namespace Atmos::Render
{
    class Color;
    class Line;

    class GraphicsManager
    {
    public:
        using Dimension = unsigned int;

        enum class Target
        {
            Main = 1 << 0,
            Stencil = 1 << 1
        };

        enum class RenderState
        {
            Stencil
        };
    public:
        virtual ~GraphicsManager() = 0;

        void ReconstructAll();

        [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageData(
            const File::Path& path);
        [[nodiscard]] std::unique_ptr<Asset::ImageAssetData> CreateImageData(
            void* buffer, std::int32_t size, const File::Name& name);
        [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderData(
            const File::Path& path);
        [[nodiscard]] std::unique_ptr<Asset::ShaderAssetData> CreateShaderData(
            void* buffer, std::int32_t size, const File::Name& name);
        [[nodiscard]] Surface& CreateSurface(
            void* window);
        [[nodiscard]] Canvas& CreateCanvas(
            const ScreenDimensions& dimensions);

        void DestroySurface(Surface& destroy);

        [[nodiscard]] bool CanMakeImage(const File::Path& path) const;
        [[nodiscard]] bool CanMakeImage(void* buffer, std::int32_t size) const;

        void ResizeCanvas(Canvas& canvas, const ScreenDimensions& dimensions);

        void SetRenderTarget(Surface& set);
        void SetRenderTargetToMain();
        [[nodiscard]] const Surface* GetCurrentRenderTarget() const;

        void RenderMaterialView(MaterialRender& materialRender);
        void RenderCanvasView(CanvasRender& canvasRender);
        void RenderLine(const Line& line);

        virtual void SetFullscreen(bool set) = 0;
        // This only has an effect if the main render target is being used
        void SetMainDimensions(const ScreenDimensions& set);
        [[nodiscard]] ScreenDimensions MainDimensions() const;
        [[nodiscard]] ScreenDimensions CurrentDimensions() const;

        virtual void ClearTarget(const Flags<Target>& target) = 0;
        virtual void ClearTarget(const Flags<Target>& target, const Color& color) = 0;

        virtual void Flush() = 0;
        virtual void SetRenderState(RenderState state, bool value) = 0;

        virtual bool Start() = 0;
        virtual void End() = 0;
        void Present();
        void Present(void* windowOverride);

        virtual void StartSprites(size_t spriteCount = 0) = 0;
        virtual void EndSprites() = 0;
        virtual void StartLines() = 0;
        virtual void EndLines() = 0;

        virtual void StartStencil() = 0;
        virtual void StopStencil() = 0;
    protected:
        Arca::Reliquary* const reliquary;
    protected:
        explicit GraphicsManager(Arca::Reliquary& reliquary);

        [[nodiscard]] bool IsUsingNonMainSurface() const;
    private:
        using SurfaceList = std::list<Surface>;
        SurfaceList surfaces;
        SurfaceList::iterator currentSurface;

        using CanvasList = std::list<Canvas>;
        CanvasList canvasList;

        ScreenDimensions dimensions;
    private:
        virtual void ReconstructInternals() = 0;
        virtual void SetMainDimensionsImpl(const ScreenDimensions& dimensions) = 0;
        [[nodiscard]] virtual ScreenDimensions MainDimensionsImpl() const = 0;

        [[nodiscard]] virtual std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
            const File::Path& path) = 0;
        [[nodiscard]] virtual std::unique_ptr<Asset::ImageAssetData> CreateImageDataImpl(
            void* buffer, std::int32_t size, const File::Name& name) = 0;
        [[nodiscard]] virtual std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
            const File::Path& path) = 0;
        [[nodiscard]] virtual std::unique_ptr<Asset::ShaderAssetData> CreateShaderDataImpl(
            void* buffer, std::int32_t size, const File::Name& name) = 0;
        [[nodiscard]] virtual Surface CreateSurfaceImpl(
            void* window) = 0;
        [[nodiscard]] virtual Canvas CreateCanvasImpl(
            const ScreenDimensions& dimensions) = 0;

        [[nodiscard]] virtual bool CanMakeImageImpl(const File::Path& path) const = 0;
        [[nodiscard]] virtual bool CanMakeImageImpl(void* buffer, std::int32_t size) const = 0;

        virtual void ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions) = 0;

        virtual void SetRenderTargetImpl(Surface& set) = 0;
        virtual void SetRenderTargetToMainImpl() = 0;
        virtual void ReleaseMainRenderTarget() = 0;
        virtual void ResetMainRenderTarget() = 0;

        virtual void PresentImpl() = 0;
        virtual void PresentImpl(void* windowOverride) = 0;

        virtual void RenderMaterialViewImpl(MaterialRender& materialRender) = 0;
        virtual void RenderCanvasViewImpl(CanvasRender& canvasRender) = 0;
        virtual void RenderLineImpl(const Line& line) = 0;

        SurfaceList::iterator FindSurface(Surface& surface);
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