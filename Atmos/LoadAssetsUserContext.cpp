#include "LoadAssetsUserContext.h"

namespace Inscription
{
    LoadAssetsUserContext::Extracted::Extracted(const Atmos::Name& name, Atmos::Buffer&& memory) :
        name(name), memory(std::move(memory))
    {}

    LoadAssetsUserContext::~LoadAssetsUserContext() = default;
}