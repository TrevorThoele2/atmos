#include "MainSurface.h"

namespace Atmos::Render
{
    MainSurface::MainSurface(Init init, DataPtr&& data) : Surface(init, std::move(data))
    {}
}