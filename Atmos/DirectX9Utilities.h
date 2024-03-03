#pragma once

#include "DirectX9Includes.h"

#include <optional>

#include "Color.h"

#include "LoggingSeverity.h"
#include "LoggingDetails.h"

#include "String.h"

#include <Arca/Reliquary.h>

namespace Atmos::Render::DirectX9
{
    D3DCOLOR ColorToD3D(const Color& col);

    void LogIfError
    (
        HRESULT hr,
        Arca::Reliquary& reliquary,
        const String& message,
        Logging::Severity severity,
        const std::optional<Logging::Details>& details = {}
    );
}