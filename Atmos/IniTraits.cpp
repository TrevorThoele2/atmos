
#include "IniTraits.h"

#include "Environment.h"

namespace Atmos
{
    String ToString(const ::Agui::Resolution::Size &arg)
    {
        return String(ToString(arg.width) + "x" + ToString(arg.height));
    }

    String ToString(const Input::KeyID &arg)
    {
        return Environment::GetInput()->GetKey(arg)->displayName;
    }

    template<> ::Agui::Resolution::Size FromString(const String &arg)
    {
        const ::Agui::Resolution::Size invalidReturn(1024, 768);

        // First, get past all of the spaces
        size_t count = 0;
        auto loop = arg.begin();
        while (loop != arg.end() && *loop == ' ')
            ++loop, ++count;

        // If we're already at the end, then leave
        if (loop == arg.end())
            return invalidReturn;

        // Find where the x is
        size_t xPosition = arg.find_first_of('x', count);
        if (xPosition == arg.npos)
            return invalidReturn;

        // Now, we need to get the first dimension
        ::Agui::Resolution::Size::ValueT width = FromString<::Agui::Resolution::Size::ValueT>(arg.substr(count, xPosition - count));
        // Now, get the second dimension
        ::Agui::Resolution::Size::ValueT height = FromString<::Agui::Resolution::Size::ValueT>(arg.substr(xPosition + 1));

        return ::Agui::Resolution::Size(width, height);
    }

    template<> Input::KeyID FromString(const String &arg)
    {
        return Environment::GetInput()->GetInputFromDisplayName(arg)->AsKey()->id;
    }
}