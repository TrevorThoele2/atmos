#pragma once

#include <vector>

#include "FieldComponent.h"

#include "Action.h"
#include "FrameTimer.h"

#include "Serialization.h"

namespace Atmos
{
    class Scheduler : public FieldComponent
    {
    private:
        struct Entry
        {
            Action action;
            FrameTimer timer;
            Entry(Action &&action, TimeValue timeTaken);
            Entry(Action &&action, FrameTimer &&timer);
        };
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        std::vector<Entry> entries;
    public:
        Scheduler(Field &field);
        Scheduler(Field &field, Scheduler &&arg);
        Scheduler& operator=(Scheduler &&arg);
        void Schedule(Action &&action, TimeValue timeTaken);
        void Work();
    };
}