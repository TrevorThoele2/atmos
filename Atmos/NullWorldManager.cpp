#include "NullWorldManager.h"

namespace Atmos::World
{
    void NullManager::LockIn(
        std::unique_ptr<Arca::Reliquary>&& reliquary, Inscription::LoadAssetsUserContext& loadAssetsUserContext)
    {}

    void NullManager::Request(FieldID id)
    {}

    void NullManager::Request(const FieldDestination& request)
    {}

    void NullManager::UseWorld(const File::Path& path)
    {}

    void NullManager::UseField(Field&& field, std::vector<Property>&& properties)
    {}

    void NullManager::UseStasis(const File::Path& path)
    {}

    File::Path NullManager::WorldPath() const
    {
        return {};
    }

    Field* NullManager::CurrentField()
    {
        return nullptr;
    }

    const Field* NullManager::CurrentField() const
    {
        return nullptr;
    }

    void NullManager::SetWorldProperties(const std::vector<Property>& set)
    {}

    std::vector<Property> NullManager::CurrentWorldProperties()
    {
        return {};
    }
}