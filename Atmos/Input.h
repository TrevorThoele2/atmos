#pragma once

#include <memory>

#include "String.h"

namespace Atmos::Input
{
    class InputData;

    class Input
    {
    public:
        const String displayName;

        bool isActive = false;
    public:
        using DataT = InputData;
        using DataPtr = std::unique_ptr<DataT>;
    public:
        explicit Input(String displayName, DataPtr&& data);

        [[nodiscard]] DataT* Data();
        [[nodiscard]] const DataT* Data() const;
        template<class RealDataT>
        [[nodiscard]] RealDataT* DataAs();
        template<class RealDataT>
        [[nodiscard]] const RealDataT* DataAs() const;
    private:
        DataPtr data;
    };

    template<class RealDataT>
    RealDataT* Input::DataAs()
    {
        return static_cast<RealDataT*>(data.get());
    }

    template<class RealDataT>
    const RealDataT* Input::DataAs() const
    {
        return static_cast<RealDataT*>(data.get());
    }

    class InputData
    {
    public:
        virtual ~InputData() = 0;
        [[nodiscard]] virtual std::unique_ptr<InputData> Clone() const = 0;
    };
}
