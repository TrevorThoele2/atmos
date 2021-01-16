#pragma once

#include "Field.h"
#include "FieldDestination.h"

#include "FilePath.h"
#include "Property.h"

#include "InputFieldArchiveInterface.h"
#include "LoadAssetsUserContext.h"

namespace Atmos::World
{
    class Manager
    {
    public:
        virtual ~Manager() = 0;

        virtual void LockIn(
            std::unique_ptr<Arca::Reliquary>&& reliquary, Inscription::LoadAssetsUserContext& loadAssetsUserContext) = 0;

        virtual void Request(FieldID id) = 0;
        virtual void Request(const FieldDestination& request) = 0;

        virtual void UseWorld(const File::Path& path) = 0;
        virtual void UseField(Field&& field, std::vector<Property>&& worldProperties) = 0;
        virtual void UseStasis(const File::Path& path) = 0;

        [[nodiscard]] virtual File::Path WorldPath() const = 0;

        [[nodiscard]] virtual Field* CurrentField() = 0;
        [[nodiscard]] virtual const Field* CurrentField() const = 0;

        virtual void SetWorldProperties(const std::vector<Property>& set) = 0;
        [[nodiscard]] virtual std::vector<Property> CurrentWorldProperties() = 0;
    protected:
        Manager() = default;
    };
}