#pragma once

#include <memory>

#include "ScreenPosition.h"
#include "ScreenDimensions.h"
#include "Color.h"
#include "String.h"

namespace Atmos
{
    class ShaderAsset;
}

namespace Atmos::Render
{
    class GraphicsManager;
    
    class Canvas
    {
    public:
        typedef ScreenPosition Position;
        typedef ScreenDimensions Dimensions;
        typedef Dimensions::Dimension DimensionValue;
    public:
        class Data
        {
        public:
            virtual ~Data() = 0 {}

            Canvas* GetOwner() const;

            virtual void StartPainting() = 0;
            virtual void StopPainting() = 0;
            virtual void PaintPixel(const Position& position, const Color& color, DimensionValue height) = 0;
            virtual void Clear(const Color& color) = 0;
            virtual void Release() = 0;
            virtual void Reset(DimensionValue width, DimensionValue height) = 0;
        private:
            Canvas *owner;
        private:
            friend Canvas;
        };

        typedef std::unique_ptr<Data> DataPtr;
    public:
        Canvas(DataPtr&& data, DimensionValue width, DimensionValue height);
        Canvas(Canvas&& arg);
        Canvas& operator=(Canvas&& arg);

        Data* GetData() const;
        template<class DataT>
        DataT* GetData() const;

        // Must be called before starting to paint to it
        void StartPainting();
        // Call when done painting
        void StopPainting();
        bool IsPainting() const;
        
        // Will not work unless this is in manual mode
        void PaintPixel(const Position& position, const Color& color);

        // This must be set as the texture for this to work
        void Clear(const Color& color);

        DimensionValue GetWidth() const;
        DimensionValue GetHeight() const;

        void Release();
        void Reset();
    private:
        bool isPainting;

        DimensionValue width;
        DimensionValue height;
        std::unique_ptr<Data> data;
    private:
        void SetData(std::unique_ptr<Data>&& set);
    private:
        friend GraphicsManager;
    };

    template<class DataT>
    DataT* Canvas::GetData() const
    {
        return static_cast<DataT*>(data.get());
    }
}