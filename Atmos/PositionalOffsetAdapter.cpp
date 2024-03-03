
#include "PositionalOffsetAdapter.h"

namespace Atmos
{
    PositionalOffsetAdapterSourceNotSetException::PositionalOffsetAdapterSourceNotSetException() :
        ObjectException("The positional offset adapter requires a source to be set, but it isn't.")
    {}
}