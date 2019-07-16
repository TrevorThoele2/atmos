#include "Sense.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    Sense::Sense(ObjectManager& manager) :
        AxisAlignedObject(manager), enabled(true)
    {}

    Sense::Sense(const ::Inscription::BinaryTableData<Sense>& data) :
        AxisAlignedObject(std::get<0>(data.bases))
    {}

    Sense::~Sense()
    {}

    ObjectTypeDescription Sense::TypeDescription() const
    {
        return ObjectTraits<Sense>::TypeDescription();
    }

    const ObjectTypeName ObjectTraits<Sense>::typeName = "Sense";
}