#pragma once

#include <memory>

#include "Color.h"
#include "String.h"

#include "Join.h"

namespace Atmos
{
    class Position2D;

    class GraphicsManager;
    class ShaderAsset;
    class Canvas
    {
    public:
        typedef unsigned int Dimension;
        typedef Join2<Dimension> PositionT;
    public:
        class Data
        {
        private:
            Canvas *owner;
            friend Canvas;
        public:
            virtual ~Data() = 0 {}
            Canvas* GetOwner() const;
            virtual void StartPainting() = 0;
            virtual void StopPainting() = 0;
            virtual void PaintPixel(const PositionT &position, const Color &color, Dimension height) = 0;
            virtual void Clear(const Color &color) = 0;
            virtual void Release() = 0;
            virtual void Reset(Dimension width, Dimension height) = 0;
        };
    private:
        bool painting;

        Dimension width;
        Dimension height;
        std::unique_ptr<Data> data;

        friend GraphicsManager;

        void SetData(Data *set);
        void SetData(std::unique_ptr<Data> &&set);
    public:
        Canvas(Data *data, Dimension width, Dimension height);
        Canvas(Canvas &&arg);
        Canvas& operator=(Canvas &&arg);

        Data* GetData() const;
        template<class DataT>
        DataT* GetData() const;

        // Must be called before starting to paint to it
        void StartPainting();
        // Call when done painting
        void StopPainting();
        bool IsPainting() const;
        
        // Will not work unless this is in manual mode
        void PaintPixel(const PositionT &position, const Color &color);

        // This must be set as the texture for this to work
        void Clear(const Color &color);

        Dimension GetWidth() const;
        Dimension GetHeight() const;

        void Release();
        void Reset();
    };

    template<class DataT>
    DataT* Canvas::GetData() const
    {
        return static_cast<DataT*>(data.get());
    }
}