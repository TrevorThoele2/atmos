#include "Variant.h"
#include "StringUtility.h"

namespace Atmos
{
    struct ToStringVisitor
    {
        template<class T>
        String operator()(T& t)
        {
            return ToString(t);
        }
    };

    String ToString(const Variant& arg)
    {
        return std::visit(ToStringVisitor{}, arg);
    }
}