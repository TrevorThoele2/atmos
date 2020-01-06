#include "DirectX9Utilities.h"

#include <DxErr.h>
#include "Log.h"

namespace Atmos::Render::DirectX9
{
    D3DCOLOR ColorToD3D(const Color& col)
    {
        return D3DCOLOR_ARGB(col.alpha, col.red, col.green, col.blue);
    }

    void LogIfError
    (
        HRESULT hr,
        Arca::Reliquary& reliquary,
        const String& message,
        Logging::Severity severity,
        const std::optional<Logging::Details>& details
    ) {
        if (hr == S_OK)
            return;

        auto useDetails = details
            ? *details
            : Logging::Details{};

        useDetails.insert(useDetails.begin(), { "Description", String(DXGetErrorDescriptionA(hr)) });

        reliquary.Raise<Logging::Log>(message, severity, useDetails);
    }
}