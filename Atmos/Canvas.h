#pragma once

#include <Arca/ClosedTypedRelic.h>
#include "CanvasData.h"

#include <memory>

namespace Atmos::Asset
{
    class ShaderAsset;
}

namespace Atmos::Render
{
    class Canvas : public Arca::ClosedTypedRelic<Canvas>
    {
    public:
        using DataT = CanvasData;
        using DataPtr = std::unique_ptr<DataT>;
    public:
        Canvas(Init init, ScreenSize size, DataPtr&& data);
        Canvas(Canvas&& arg) noexcept;

        Canvas& operator=(Canvas&& arg) noexcept;

        [[nodiscard]] DataT* Data() const;
        template<class DataT>
        [[nodiscard]] DataT* Data() const;

        // Must be called before starting to paint to it
        void StartPainting();
        // Call when done painting
        void StopPainting();
        [[nodiscard]] bool IsPainting() const;
        
        // Will not work unless this is in manual mode
        void PaintPixel(const ScreenPosition& position, const Color& color, ScreenPosition::Value height);

        // This must be set as the texture for this to work
        void Clear(const Color& color);

        [[nodiscard]] ScreenSize Size() const;

        void Release();
        void Reset();
    private:
        bool isPainting = false;

        ScreenSize size;
        std::unique_ptr<DataT> data;
    };

    template<class DataT>
    DataT* Canvas::Data() const
    {
        return static_cast<DataT*>(data.get());
    }
}

namespace Arca
{
    template<>
    struct Traits<Atmos::Render::Canvas>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "Canvas";
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Render::Canvas, BinaryArchive> final :
        public ArcaNullScribe<Atmos::Render::Canvas, BinaryArchive>
    {};
}