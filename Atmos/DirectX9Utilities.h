#pragma once

#include "DirectX9Includes.h"

#include <optional>

#include "Color.h"

#include "LoggingSeverity.h"
#include "LoggingDetails.h"

#include "String.h"

namespace Atmos::Render::DirectX9
{
    D3DCOLOR ToDirectXColor(const Color& color);

    void LogIfError
    (
        HRESULT hr,
        const String& message,
        Logging::Severity severity,
        const std::optional<Logging::Details>& details = {}
    );
}