#include "InvalidSignature.h"

namespace Atmos
{
    InvalidSignature::InvalidSignature() :
        Exception("The signature in the file was not correct. The file is either corrupt or not the correct file type.")
    {}
}