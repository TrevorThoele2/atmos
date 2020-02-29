#pragma once

#include "DirectX9Includes.h"
#include <DxErr.h>

#include <optional>

#include "Color.h"

#include "LoggingSeverity.h"
#include "LoggingDetails.h"
#include "Logger.h"

#include "String.h"


namespace Atmos::Render::DirectX9
{
    D3DCOLOR ToDirectXColor(const Color& color);

    bool IsSuccess(HRESULT hr);

    bool IsError(HRESULT hr);

    template<class Function>
    void IfError(HRESULT hr, Function function)
    {
        if (!IsError(hr))
            return;

        function();
    }

    template<class Function>
    void LogIfError(HRESULT hr, Function function)
    {
        IfError(
            hr,
            [hr, function]()
            {
                auto log = function();

                auto useDetails = log.details
                    ? *log.details
                    : Logging::Details{};

                useDetails.insert(useDetails.begin(), { "Description", String(DXGetErrorDescriptionA(hr)) });

                Logging::logger.Log(Logging::Log(log.message, log.severity, useDetails));
            });
    }
}