#include "DirectX9Utilities.h"

#include <DxErr.h>
#include "Logger.h"

namespace Atmos::Render::DirectX9
{
    D3DCOLOR ColorToD3D(const Color& color)
    {
        return D3DCOLOR_ARGB(color.alpha, color.red, color.green, color.blue);
    }

    void LogIfError
    (
        HRESULT hr,
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

        Logging::logger.Log(Logging::Log(message, severity, useDetails));
    }
}