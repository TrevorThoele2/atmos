
#include "IniTraits.h"

#include "Environment.h"

namespace Atmos
{
    String ToString(const ::agui::Resolution::Size &arg)
    {
        return String(ToString(arg.width) + "x" + ToString(arg.height));
    }

    String ToString(const Input::KeyID &arg)
    {
        return Environment::GetInput()->GetKey(arg)->displayName;
    }

    template<> ::agui::Resolution::Size FromString(const String &arg)
    {
        const ::agui::Resolution::Size invalidReturn(1024, 768);

        // First, get past all of the spaces
        size_t count = 0;
        auto loop = arg.begin();
        while (loop != arg.end(), *loop == ' ')
            ++loop, ++count;

        // If we're already at the end, then leave
        if (loop == arg.end())
            return invalidReturn;

        // Find where the x is
        size_t xPosition = arg.find_first_of('x', count);
        if (xPosition == arg.npos)
            return invalidReturn;

        // Now, we need to get the first dimension
        ::agui::Resolution::Size::ValueT width = FromString<::agui::Resolution::Size::ValueT>(arg.substr(count, xPosition - count));
        // Now, get the second dimension
        ::agui::Resolution::Size::ValueT height = FromString<::agui::Resolution::Size::ValueT>(arg.substr(xPosition + 1));

        return ::agui::Resolution::Size(width, height);
    }

    template<> Input::KeyID FromString(const String &arg)
    {
        return Environment::GetInput()->GetInputFromDisplayName(arg)->AsKey()->id;
    }
}