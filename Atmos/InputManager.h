#pragma once

#include <vector>

#include "InputID.h"
#include "Input.h"
#include "ScreenPosition.h"

#include <Arca/Traits.h>

namespace Atmos::Input
{
    class Manager
    {
    public:
        Manager() = default;
        virtual ~Manager() = 0;
    public:
        struct CreatedInput
        {
            InputID id;
            Input input;
        };

        [[nodiscard]] virtual std::vector<CreatedInput> CreateAllInputs() const = 0;
        [[nodiscard]] virtual ScreenPosition CurrentMousePosition() const = 0;
        [[nodiscard]] virtual bool IsInputDown(Input& input) const = 0;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Input::Manager>
    {
        static inline const TypeName typeName = "InputManager";
    };
}