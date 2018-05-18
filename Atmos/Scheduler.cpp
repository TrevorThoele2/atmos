
#include "Scheduler.h"

namespace Atmos
{
    Scheduler::Entry::Entry(Action &&action, TimeValue timeTaken) : action(std::move(action)), timer(timeTaken)
    {}

    Scheduler::Entry::Entry(Action &&action, FrameTimer &&timer) : action(std::move(action)), timer(std::move(timer))
    {}

    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Scheduler)
    {
        if (scribe.IsOutput())
        {
            ::inscription::ContainerSize size(entries.size());
            scribe.Save(size);

            for (auto &loop : entries)
            {
                scribe.Save(loop.action);
                scribe.Save(loop.timer);
            }
        }
        else // INPUT
        {
            ::inscription::ContainerSize size;
            scribe.Load(size);

            while (size-- > 0)
            {
                Action action;
                scribe.Load(action);

                FrameTimer timer;
                scribe.Load(timer);

                entries.push_back(Entry(std::move(action), std::move(timer)));
            }
        }
    }

    Scheduler::Scheduler(Field &field) : FieldComponent(field)
    {}

    Scheduler::Scheduler(Field &field, Scheduler &&arg) : FieldComponent(field), entries(std::move(arg.entries))
    {}

    Scheduler& Scheduler::operator=(Scheduler &&arg)
    {
        entries = std::move(arg.entries);
        return *this;
    }

    void Scheduler::Schedule(Action &&action, TimeValue timeTaken)
    {
        entries.push_back(Entry(std::move(action), timeTaken));
        entries.back().timer.Start();
    }

    void Scheduler::Work()
    {
        for (auto loop = entries.begin(); loop != entries.end();)
        {
            if (loop->timer.HasReachedGoal())
            {
                loop->action.Execute();
                loop = entries.erase(loop);
            }
            else
                ++loop;
        }
    }
}