#pragma once

#include "WorldManager.h"

namespace Atmos::World
{
    class NullManager final : public Manager
    {
    public:
        void LockIn() override;
        [[nodiscard]] bool WillLockIn() const override;

        void Request(FieldID id) override;
        void Request(const FieldDestination& request) override;

        void UseWorld(const File::Path& path) override;
        void UseField(Field&& field, std::vector<Property>&& properties) override;
        void UseStasis(const File::Path& path) override;

        [[nodiscard]] File::Path WorldPath() const override;

        [[nodiscard]] Field* CurrentField() override;
        [[nodiscard]] const Field* CurrentField() const override;

        void SetWorldProperties(const std::vector<Property>& set) override;
        [[nodiscard]] std::vector<Property> CurrentWorldProperties() override;
    };
}