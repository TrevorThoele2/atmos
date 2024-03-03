
#include "ScriptVariant.h"

#include <Chroma/Iterate.h>

namespace Atmos
{
    class ToStringImplementation
    {
    public:
        template<class T>
        static String DoReturn(T& t)
        {
            return ToString(t);
        }
    };

    String ToString(const Scripting::VariantT& arg)
    {
        return ::Chroma::VisitReturn<ToStringImplementation, String>(arg);
    }
}

namespace Chroma
{
    template Variant<
        bool,
        std::int8_t,
        std::int16_t,
        std::int32_t,
        std::int64_t,
        std::uint8_t,
        std::uint16_t,
        std::uint32_t,
        std::uint64_t,
        float,
        double>;
}