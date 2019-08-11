#include "Object.h"

#include "ObjectManager.h"

namespace Atmos
{
    Object::Object(ObjectManager& manager) : manager(&manager)
    {}

    Object::Object(const ::Inscription::BinaryTableData<Object>& data) :
        id(data.id), manager(data.manager)
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
    Scribe<::Atmos::Object, BinaryArchive>::Table::Table()
    {
        MergeDataEntries({
            DataEntry::Auto(&ObjectT::id, &DataT::id),
            DataEntry::Auto(&ObjectT::manager, &DataT::manager) });
    }

    void Scribe<::Atmos::Object, BinaryArchive>::Table::ConstructImplementation(ObjectT* storage, ArchiveT& archive)
    {
        DoBasicConstruction(storage, archive);
    }
}