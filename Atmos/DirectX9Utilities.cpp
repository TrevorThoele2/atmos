#include "DirectX9Utilities.h"

namespace Atmos::Render::DirectX9
{
    D3DCOLOR ToDirectXColor(const Color& color)
    {
        return D3DCOLOR_ARGB(color.alpha, color.red, color.green, color.blue);
    }

    bool IsSuccess(HRESULT hr)
    {
        return SUCCEEDED(hr);
    }

    bool IsError(HRESULT hr)
    {
        return FAILED(hr);
    }
}