#pragma once

#include <memory>

#include "ScreenPosition.h"
#include "ScreenDimensions.h"
#include "Color.h"

namespace Atmos::Asset
{
    class ShaderAsset;
}

namespace Atmos::Render
{
    class GraphicsManager;
    
    class Canvas
    {
    public:
        using Position = ScreenPosition;
        using Dimensions = ScreenDimensions;
        using DimensionValue = Dimensions::Dimension;
    public:
        class Data
        {
        public:
            virtual ~Data() = 0;

            [[nodiscard]] Canvas* Owner() const;

            virtual void StartPainting() = 0;
            virtual void StopPainting() = 0;
            virtual void PaintPixel(const Position& position, const Color& color, DimensionValue height) = 0;
            virtual void Clear(const Color& color) = 0;
            virtual void Release() = 0;
            virtual void Reset(DimensionValue width, DimensionValue height) = 0;
        private:
            Canvas* owner = nullptr;
        private:
            friend Canvas;
        };

        using DataPtr = std::unique_ptr<Data>;
    public:
        Canvas(DataPtr&& data, DimensionValue width, DimensionValue height);
        Canvas(Canvas&& arg) noexcept;
        Canvas& operator=(Canvas&& arg) noexcept;

        [[nodiscard]] Data* GetData() const;
        template<class DataT>
        [[nodiscard]] DataT* GetData() const;

        // Must be called before starting to paint to it
        void StartPainting();
        // Call when done painting
        void StopPainting();
        [[nodiscard]] bool IsPainting() const;
        
        // Will not work unless this is in manual mode
        void PaintPixel(const Position& position, const Color& color);

        // This must be set as the texture for this to work
        void Clear(const Color& color);

        [[nodiscard]] DimensionValue Width() const;
        [[nodiscard]] DimensionValue Height() const;

        void Release();
        void Reset();
    private:
        bool isPainting = false;

        DimensionValue width;
        DimensionValue height;
        std::unique_ptr<Data> data;
    private:
        void SetData(DataPtr&& set);
    private:
        friend GraphicsManager;
    };

    template<class DataT>
    DataT* Canvas::GetData() const
    {
        return static_cast<DataT*>(data.get());
    }
}