#pragma once

#include "KeyStates.h"
#include "Point2D.h"
#include "Event.h"
#include "String.h"
#include "Logger.h"

namespace Atmos::Input
{
    class Manager
    {
    public:
        struct State
        {
            KeyStates keyStates;
            Spatial::Point2D mousePosition;
            std::vector<String> enteredText;
        };
    public:
        Manager(Logging::Logger& logger, String typeName);
        virtual ~Manager() = 0;

        [[nodiscard]] virtual State ReadState() const = 0;
    private:
        String typeName;
        Logging::Logger* logger;
    };
}