
#include "ObjectBatchSource.h"

namespace Atmos
{
    ObjectBatchSourceBase::~ObjectBatchSourceBase()
    {}
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::ObjectBatchSourceBase)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;
        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }
}