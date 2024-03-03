
#include "GeneralComponent.h"

#include <Inscription/UnorderedSet.h>

namespace Atmos
{
    namespace Ent
    {
        GeneralComponent::Storage::Storage() : name(nullptr)
        {}

        GeneralComponent::Storage::Storage(const Name &name, const Variant &set) : name(name), variant(set)
        {}

        GeneralComponent::Storage::Storage(const Name &name, Variant &&set) : name(name), variant(std::move(set))
        {}

        bool GeneralComponent::Storage::operator==(const Storage &arg) const
        {
            return name == arg.name && variant == arg.variant;
        }

        bool GeneralComponent::Storage::operator!=(const Storage &arg) const
        {
            return !(*this == arg);
        }

        Variant* GeneralComponent::Storage::operator->()
        {
            return &variant;
        }

        const Variant* GeneralComponent::Storage::operator->() const
        {
            return &variant;
        }

        const Name& GeneralComponent::Storage::GetName() const
        {
            return name;
        }

        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(GeneralComponent)
        {
            if (scribe.IsOutput())
            {
                scribe.Save(name);
                scribe.Save(niceName);
                scribe.Save(position);
                scribe.Save(direction);
                scribe.Save(solid);

                // Storage
                {
                    ::Inscription::ContainerSize size(storage.size());
                    scribe.Save(size);

                    for (auto &loop : storage)
                    {
                        scribe.Save(::Inscription::RemoveConst(loop.first));
                        scribe.Save(loop.second.variant);
                    }
                }

                scribe.Save(tags);
            }
            else // INPUT
            {
                scribe.Load(name);
                scribe.Load(niceName);
                scribe.Load(position);
                scribe.Load(direction);
                scribe.Load(solid);

                // Storage
                {
                    ::Inscription::ContainerSize size;
                    scribe.Load(size);

                    while (size-- > 0)
                    {
                        Name name;
                        scribe.Load(name);

                        Variant variant;
                        scribe.Load(variant);

                        storage.emplace(std::move(name), Storage(name, std::move(variant)));
                    }
                }

                scribe.Load(tags);
            }
        }

        GeneralComponent::GeneralComponent() : solid(false)
        {}

        GeneralComponent::GeneralComponent(const Name &name, const PositionT &position, const DirectionT &direction, Manager *manager) : name(name), niceName(name), position(position), direction(direction), handler(manager), solid(false)
        {}

        GeneralComponent::GeneralComponent(GeneralComponent &&arg) : Component(std::move(arg)), name(std::move(arg.name)), niceName(std::move(arg.niceName)), position(std::move(arg.position)), direction(std::move(arg.direction)), solid(std::move(arg.solid)), tags(std::move(arg.tags)), handler(std::move(arg.handler))
        {}

        GeneralComponent& GeneralComponent::operator=(GeneralComponent &&arg)
        {
            Component::operator=(std::move(arg));
            name = std::move(arg.name);
            niceName = std::move(arg.niceName);
            position = std::move(arg.position);
            direction = std::move(arg.direction);
            solid = std::move(arg.solid);
            tags = std::move(arg.tags);
            handler = std::move(arg.handler);
            return *this;
        }

        bool GeneralComponent::operator==(const GeneralComponent &arg) const
        {
            return name == arg.name && niceName == arg.niceName && position == arg.position && direction == arg.direction && solid == arg.solid && storage == arg.storage && tags == arg.tags && handler == arg.handler;
        }

        bool GeneralComponent::operator!=(const GeneralComponent &arg) const
        {
            return !(*this == arg);
        }

        void GeneralComponent::SetPosition(const PositionT &set)
        {
            position = set;
        }

        GridPosition GeneralComponent::GetPositionInFront() const
        {
            GridPosition posInFront(position);

            switch (direction.Get())
            {
            case Direction::ValueT::UP:
                --posInFront.y;
                break;
            case Direction::ValueT::DOWN:
                ++posInFront.y;
                break;
            case Direction::ValueT::LEFT:
                --posInFront.x;
                break;
            case Direction::ValueT::RIGHT:
                ++posInFront.x;
                break;
            }

            return posInFront;
        }

        void GeneralComponent::SetSolid(bool set)
        {
            solid = set;
        }

        bool GeneralComponent::IsSolid() const
        {
            return solid;
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Name &name, bool set)
        {
            return AddStorage(Storage(name, Variant(set)));
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Name &name, std::uint8_t set)
        {
            return AddStorage(Storage(name, Variant(set)));
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Name &name, std::uint16_t set)
        {
            return AddStorage(Storage(name, Variant(set)));
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Name &name, std::uint32_t set)
        {
            return AddStorage(Storage(name, Variant(set)));
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Name &name, std::uint64_t set)
        {
            return AddStorage(Storage(name, Variant(set)));
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Name &name, std::int8_t set)
        {
            return AddStorage(Storage(name, Variant(set)));
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Name &name, std::int16_t set)
        {
            return AddStorage(Storage(name, Variant(set)));
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Name &name, std::int32_t set)
        {
            return AddStorage(Storage(name, Variant(set)));
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Name &name, std::int64_t set)
        {
            return AddStorage(Storage(name, Variant(set)));
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Name &name, const GridPosition &set)
        {
            return AddStorage(Storage(name, Variant(set)));
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Name &name, const Variant &set)
        {
            return AddStorage(Storage(name, set));
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(const Storage &add)
        {
            return &storage.emplace(add.GetName(), add).first->second;
        }

        GeneralComponent::Storage* GeneralComponent::AddStorage(Storage &&add)
        {
            return &storage.emplace(add.GetName(), std::move(add)).first->second;
        }

        void GeneralComponent::RemoveStorage(const Name &remove)
        {
            storage.erase(remove);
        }

        void GeneralComponent::RemoveStorage(const Storage &remove)
        {
            RemoveStorage(remove.GetName());
        }

        GeneralComponent::Storage* GeneralComponent::FindStorage(const Name &find)
        {
            auto found = storage.find(find);
            if (found == storage.end())
                return nullptr;

            return &found->second;
        }

        GeneralComponent::Storage* GeneralComponent::FindStorage(const Storage &find)
        {
            return FindStorage(find.GetName());
        }

        const GeneralComponent::Storage* GeneralComponent::FindStorage(const Name &find) const
        {
            auto found = storage.find(find);
            if (found == storage.end())
                return nullptr;

            return &found->second;
        }

        const GeneralComponent::Storage* GeneralComponent::FindStorage(const Storage &find) const
        {
            return FindStorage(find.GetName());
        }

        bool GeneralComponent::HasStorage(const Name &check) const
        {
            return storage.find(check) != storage.end();
        }

        bool GeneralComponent::HasStorage(const Storage &check) const
        {
            return HasStorage(check.GetName());
        }

        void GeneralComponent::TagAs(const Tag &add)
        {
            tags.emplace(add);
        }

        void GeneralComponent::RemoveTag(const Tag &remove)
        {
            tags.erase(remove);
        }

        bool GeneralComponent::HasTag(const Tag &check) const
        {
            return tags.find(check) != tags.end();
        }
    }
}