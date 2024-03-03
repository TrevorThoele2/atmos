#include "ScriptVariant.h"

#include <Chroma/Iterate.h>

namespace Atmos
{
    class ToStringImplementation
    {
    public:
        template<class T>
        String operator()(T& t)
        {
            return ToString(t);
        }
    };

    String ToString(const Script::Variant& arg)
    {
        return std::visit(ToStringImplementation{}, arg);
    }
}

namespace Chroma
{
    template std::variant<
        bool,
        std::int8_t,
        std::int16_t,
        std::int32_t,
        std::int64_t,
        std::uint8_t,
        std::uint16_t,
        uint32_t,
        std::uint64_t,
        float,
        double
    >;
}