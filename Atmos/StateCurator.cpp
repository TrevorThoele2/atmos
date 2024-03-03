#include "StateCurator.h"

#include "cassert"

#include <Arca/Reliquary.h>

namespace Atmos::State
{
    void StateCurator::Handle(const Request& command)
    {
        switch (command.value.index())
        {
        case 0:
            Push(*std::get<0>(command.value));
            break;
        case 1:
            Pop();
            break;
        default:
            assert(false);
        }
    }

    void StateCurator::Push(const GameState& state)
    {
        stack.push_back(&state);
    }

    void StateCurator::Pop()
    {
        if (stack.empty())
            return;

        stack.pop_back();
    }
}