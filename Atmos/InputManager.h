#pragma once

#include "KeyStates.h"
#include "ScreenPoint.h"
#include "Event.h"
#include "String.h"

namespace Atmos::Input
{
    class Manager
    {
    public:
        struct State
        {
            KeyStates keyStates;
            Spatial::ScreenPoint mousePosition;
            std::vector<String> enteredText;
        };
    public:
        Manager() = default;
        virtual ~Manager() = 0;

        [[nodiscard]] virtual State ReadState() const = 0;

        [[nodiscard]] virtual String TypeName() const = 0;
    };
}