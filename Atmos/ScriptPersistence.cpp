
#include <unordered_map>

#include "ScriptPersistence.h"

#include "Optional.h"

namespace Atmos
{
    namespace Scripting
    {
        Persistence::Persistence()
        {}

        Persistence::Persistence(const Persistence& arg)
        {}

        Persistence::~Persistence()
        {}

        Persistence& Persistence::operator=(const Persistence& arg)
        {
            return *this;
        }

        Persistence::SizeT Persistence::Size() const
        {
            return 0;
        }

        bool Persistence::IsEmpty() const
        {
            return true;
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Persistence)
        {

        }
    }
}