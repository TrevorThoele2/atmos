#pragma once

namespace Atmos
{
    template<class T>
    class PropertyCore
    {
    protected:
        virtual T GetInternal() const = 0;
        virtual void SetInternal(T set) = 0;
    };
}