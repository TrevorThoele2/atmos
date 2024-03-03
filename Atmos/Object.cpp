#include "Object.h"

#include "ObjectManager.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    Object::Object(ObjectManager& manager) : manager(&manager)
    {}

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(Object) : INSCRIPTION_TABLE_GET_MEM(id), INSCRIPTION_TABLE_GET_MEM(manager)
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
    INSCRIPTION_BINARY_INSCRIPTER_DEFINE_TABLE(::Atmos::Object)
    {
        INSCRIPTION_BINARY_INSCRIPTER_CREATE_TABLE;

        INSCRIPTION_TABLE_ADD(id);
        INSCRIPTION_TABLE_ADD_UNOWNING_POINTER(manager);

        INSCRIPTION_INSCRIPTER_RETURN_TABLE;
    }
}