
#include "IniTraits.h"

#include "Environment.h"

namespace Atmos
{
    namespace Ini
    {
        String ToIniString(const ::Agui::Resolution::Size &arg)
        {
            return String(ToString(arg.width) + "x" + ToString(arg.height));
        }

        String ToIniString(const Input::KeyID &arg)
        {
            return Environment::GetInput()->GetKey(arg)->displayName;
        }

        template<>
        Optional<::Agui::Resolution::Size> FromIniString(const String &arg)
        {
            typedef Optional<::Agui::Resolution::Size> RetT;

            // First, get past all of the spaces
            size_t count = 0;
            auto loop = arg.begin();
            while (loop != arg.end() && *loop == ' ')
                ++loop, ++count;

            // If we're already at the end, then leave
            if (loop == arg.end())
                return RetT();

            // Find where the x is
            size_t xPosition = arg.find_first_of('x', count);
            if (xPosition == arg.npos)
                return RetT();

            // Now, we need to get the first dimension
            ::Agui::Resolution::Size::ValueT width = FromString<::Agui::Resolution::Size::ValueT>(arg.substr(count, xPosition - count));
            // Now, get the second dimension
            ::Agui::Resolution::Size::ValueT height = FromString<::Agui::Resolution::Size::ValueT>(arg.substr(xPosition + 1));

            return RetT(::Agui::Resolution::Size(width, height));
        }

        template<>
        Optional<Input::KeyID> FromIniString(const String &arg)
        {
            typedef Optional<Input::KeyID> RetT;

            auto signal = Environment::GetInput()->GetInputFromDisplayName(arg);
            if (!signal || !signal->AsKey())
                return RetT();

            return RetT(signal->AsKey()->id);
        }
    }
}