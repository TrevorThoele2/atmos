#pragma once

#include <Atmos/WorldManager.h>

class MockWorldManager final : public Atmos::World::Manager
{
public:
    std::vector<Atmos::World::FieldID> requestCalls;

    std::vector<Atmos::Property> properties;
public:
    void LockIn() override;
    [[nodiscard]] bool WillLockIn() const override;

    void Request(Atmos::World::FieldID id) override;
    void Request(const Atmos::World::FieldDestination& request) override;

    void UseWorld(const Atmos::File::Path& path) override;
    void UseField(Atmos::World::Field&& field, std::vector<Atmos::Property>&& properties) override;
    void UseStasis(const Atmos::File::Path& path) override;

    [[nodiscard]] Atmos::File::Path WorldPath() const override;

    [[nodiscard]] Atmos::World::Field* CurrentField() override;
    [[nodiscard]] const Atmos::World::Field* CurrentField() const override;

    void SetWorldProperties(const std::vector<Atmos::Property>& set) override;
    [[nodiscard]] std::vector<Atmos::Property> CurrentWorldProperties() override;
};