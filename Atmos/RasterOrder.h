#pragma once

namespace Atmos::Render::Raster
{
    struct Order
    {
        int space = 0;
        float z = 0;
        
        bool operator==(const Order& arg) const;
        bool operator!=(const Order& arg) const;
        bool operator<(const Order& arg) const;
    };
}