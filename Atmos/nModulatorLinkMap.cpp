
#include "nModulatorLinkMap.h"

namespace Atmos
{
    namespace Modulator
    {
        Link LinkMap::Create(LinkID id, ObjectReference from)
        {
            return bases.find(from->TypeDescription().typeName)->second->Create(id, from);
        }
    }
}