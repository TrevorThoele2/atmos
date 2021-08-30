#include "MockWorldManager.h"

void MockWorldManager::LockIn()
{}

bool MockWorldManager::WillLockIn() const
{
    return false;
}

void MockWorldManager::Request(Atmos::World::FieldID id)
{
    requestCalls.push_back(id);
}

void MockWorldManager::Request(const Atmos::World::FieldDestination& request)
{}

void MockWorldManager::UseWorld(const Atmos::File::Path& path)
{}

void MockWorldManager::UseField(Atmos::World::Field&& field, std::vector<Atmos::Property>&& properties)
{}

void MockWorldManager::UseStasis(const Atmos::File::Path& path)
{}

Atmos::File::Path MockWorldManager::WorldPath() const
{
    return {};
}

Atmos::World::Field* MockWorldManager::CurrentField()
{
    return nullptr;
}

const Atmos::World::Field* MockWorldManager::CurrentField() const
{
    return nullptr;
}

void MockWorldManager::SetWorldProperties(const std::vector<Atmos::Property>& set)
{
    properties = set;
}

std::vector<Atmos::Property> MockWorldManager::CurrentWorldProperties()
{
    return properties;
}