
#include "Object.h"

#include "ObjectManager.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    Object::Object() : manager(nullptr)
    {}

    Object::Object(const ::Inscription::Table<Object>& table) : INSCRIPTION_TABLE_GET_MEM(id), INSCRIPTION_TABLE_GET_MEM(manager)
    {}

    Object::~Object()
    {}

    ObjectID Object::ID() const
    {
        return id;
    }

    ObjectManager* Object::Manager()
    {
        return manager;
    }

    const ObjectManager* Object::Manager() const
    {
        return manager;
    }
}

namespace Inscription
{
    INSCRIPTION_INSCRIPTER_DEFINE_TABLE(::Atmos::Object)
    {
        INSCRIPTION_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD(id);
        INSCRIPTION_TABLE_ADD_UNOWNING_POINTER(manager);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }
}