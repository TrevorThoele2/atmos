
#include "Sense.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    nSense::nSense() : enabled(true)
    {}

    nSense::nSense(const ::Inscription::Table<nSense>& table) : INSCRIPTION_TABLE_GET_BASE(PositionalObject)
    {}

    nSense::~nSense()
    {}

    ObjectTypeDescription nSense::TypeDescription() const
    {
        return ObjectTraits<nSense>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<nSense>::typeName = "Sense";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::nSense)
    {

    }
}