
#include "Variant.h"

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

    String ToString(const VariantT& arg)
    {
        return ::Chroma::VisitReturn<ToStringImplementation, String>(arg);
    }
}