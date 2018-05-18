
#include "OrphanScripts.h"

namespace Atmos
{
    OrphanScripts::OrphanScripts(Field &field) : FieldComponent(field)
    {}

    OrphanScripts::OrphanScripts(Field &field, OrphanScripts &&arg) : FieldComponent(field), held(std::move(arg.held))
    {}

    OrphanScripts& OrphanScripts::operator=(OrphanScripts &&arg)
    {
        held = std::move(arg.held);
        return *this;
    }

    Script::Instance& OrphanScripts::Add(Script::Instance &&script)
    {
        held.push_back(std::move(script));
        return held.back();
    }

    void OrphanScripts::Work()
    {
        for (auto loop = held.begin(); loop != held.end(); ++loop)
        {
            if (!loop->IsExecuting())
                loop = held.erase(loop);
            else
                ++loop;
        }
    }
}