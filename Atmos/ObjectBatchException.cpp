
#include "ObjectBatchException.h"

namespace Atmos
{
    ObjectBatchInvalidatedException::ObjectBatchInvalidatedException() : runtime_error("This batch has been invalidated")
    {}
}