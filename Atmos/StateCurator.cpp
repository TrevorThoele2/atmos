#include "StateCurator.h"

#include "cassert"

#include <Arca/Reliquary.h>

namespace Atmos::State
{
    void StateCurator::InitializeImplementation()
    {
        requests = Owner().Batch<Request>();
    }

    void StateCurator::WorkImplementation(Stage& stage)
    {
        if (!requests.IsEmpty())
        {
            auto& request = *--requests.end();
            switch (request.value.index())
            {
            case 0:
                Push(*std::get<0>(request.value));
                break;
            case 1:
                Pop();
                break;
            default:
                assert(false);
            }
        }
    }

    void StateCurator::Push(GameState& state)
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