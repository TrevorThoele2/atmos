#include "Input.h"

namespace Atmos::Input
{
    Input::Input(String displayName, DataPtr&& data)
        : displayName(std::move(displayName)), data(std::move(data))
    {}

    auto Input::Data() -> DataT*
    {
        return data.get();
    }

    auto Input::Data() const -> const DataT*
    {
        return data.get();
    }

    InputData::~InputData()
    {}
}