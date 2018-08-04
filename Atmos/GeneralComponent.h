#pragma once

#include "EntityComponent.h"
#include "EntityManager.h"

#include "Name.h"
#include "GridPosition.h"
#include "ObjectHandle.h"
#include "Modulator.h"
#include "Tag.h"
#include "Variant.h"

#include "Serialization.h"

namespace Atmos
{
    namespace Ent
    {
        class GeneralComponent : public Component<GeneralComponent>
        {
        private:
            INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
            INSCRIPTION_ACCESS;
        public:
            class Storage
            {
            public:
                Name name;
                Variant variant;

                Storage();
                Storage(const Name &name, const Variant &set);
                Storage(const Name &name, Variant &&set);
                bool operator==(const Storage &arg) const;
                bool operator!=(const Storage &arg) const;
                Variant* operator->();
                const Variant* operator->() const;
                const Name& GetName() const;
            };
        public:
            // Name
            Name name;
            Name niceName;

            // Position
            typedef GridPosition PositionT;
            PositionT position;

            // Direction
            typedef Direction DirectionT;
            DirectionT direction;

            // Solid
            bool solid;

            // Persistent storage
            std::unordered_map<Name, Storage> storage;

            // Tags
            std::unordered_set<Tag> tags;

            // Handler
            ObjectHandle<Manager, GeneralComponent> handler;

            GeneralComponent();
            GeneralComponent(const Name &name, const PositionT &position, const DirectionT &direction, Manager *manager);
            GeneralComponent(const GeneralComponent &arg) = default;
            GeneralComponent(GeneralComponent &&arg);
            GeneralComponent& operator=(const GeneralComponent &arg) = default;
            GeneralComponent& operator=(GeneralComponent &&arg);

            bool operator==(const GeneralComponent &arg) const;
            bool operator!=(const GeneralComponent &arg) const;

            void SetPosition(const PositionT &set);
            GridPosition GetPositionInFront() const;

            void SetSolid(bool set);
            bool IsSolid() const;

            Storage* AddStorage(const Name &name, bool set);
            Storage* AddStorage(const Name &name, std::uint8_t set);
            Storage* AddStorage(const Name &name, std::uint16_t set);
            Storage* AddStorage(const Name &name, std::uint32_t set);
            Storage* AddStorage(const Name &name, std::uint64_t set);
            Storage* AddStorage(const Name &name, std::int8_t set);
            Storage* AddStorage(const Name &name, std::int16_t set);
            Storage* AddStorage(const Name &name, std::int32_t set);
            Storage* AddStorage(const Name &name, std::int64_t set);
            Storage* AddStorage(const Name &name, const GridPosition &set);
            Storage* AddStorage(const Name &name, const Variant &set);

            Storage* AddStorage(const Storage &add);
            Storage* AddStorage(Storage &&add);
            void RemoveStorage(const Name &remove);
            void RemoveStorage(const Storage &remove);

            Storage* FindStorage(const Name &find);
            Storage* FindStorage(const Storage &find);
            const Storage* FindStorage(const Name &find) const;
            const Storage* FindStorage(const Storage &find) const;
            bool HasStorage(const Name &check) const;
            bool HasStorage(const Storage &check) const;

            void TagAs(const Tag &add);
            void RemoveTag(const Tag &remove);
            bool HasTag(const Tag &check) const;
        };

        ENTITY_COMPONENT_MAP_DECLARE("General", GeneralComponent)
    }
}