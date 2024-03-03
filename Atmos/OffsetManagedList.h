#pragma once

#include <unordered_map>

#include "ObjectOffsetHandle.h"
#include "IDManager.h"
#include "Position3D.h"

#include "Assert.h"

#include "Serialization.h"
#include <Inscription/Scribe.h>
#include <Inscription/List.h>

namespace Atmos
{
    template<class T>
    class OffsetManagedList
    {
    private:
        typedef IDManager<std::unordered_map<IDManagerBase::ID, ObjectOffsetHandle<T, Position3D>>> ManagerT;
    public:
        typedef typename ManagerT::Object HandleT;
        typedef typename ManagerT::ID ID;
        typedef typename ManagerT::Object::ObjectT ObjectT;
        typedef typename ManagerT::Object::OffsetT OffsetT;
        typedef typename ManagerT::Object::OffsetValueT OffsetValueT;

        typedef typename ManagerT::iterator iterator;
        typedef typename ManagerT::const_iterator const_iterator;
        typedef typename ManagerT::size_type size_type;

        static const ID nullID = ManagerT::nullID;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    private:
        ManagerT manager;
        const Position3D *ownerPosition;

        std::pair<bool, iterator> AddImpl(const HandleT &handle);
        std::pair<bool, iterator> AddImpl(ID id, const HandleT &handle);
        std::pair<bool, iterator> AddImpl(HandleT &&handle);
        std::pair<bool, iterator> AddImpl(ID id, HandleT &&handle);

        void SetManagerOwnerPosition();
    public:
        OffsetManagedList();
        OffsetManagedList(const Position3D *ownerPosition);
        OffsetManagedList(const OffsetManagedList &arg);
        OffsetManagedList(const OffsetManagedList &arg, const Position3D *ownerPosition);
        OffsetManagedList& operator=(const OffsetManagedList &arg);
        OffsetManagedList(OffsetManagedList &&arg);
        OffsetManagedList(OffsetManagedList &&arg, const Position3D *ownerPosition);
        OffsetManagedList& operator=(OffsetManagedList &&arg);
        bool operator==(const OffsetManagedList &arg) const;
        bool operator!=(const OffsetManagedList &arg) const;

        // This must be called whenever this is serialized. It is not saved
        void SetOwnerPosition(const Position3D *set);

        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(const ObjectT &obj);
        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(const ObjectT &obj, OffsetValueT xOffset, OffsetValueT yOffset, OffsetValueT zOffset);
        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(ID id, const ObjectT &obj);
        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(ID id, const ObjectT &obj, OffsetValueT xOffset, OffsetValueT yOffset, OffsetValueT zOffset);
        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(ObjectT &&obj);
        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(ObjectT &&obj, OffsetValueT xOffset, OffsetValueT yOffset, OffsetValueT zOffset);
        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(ID id, ObjectT &&obj);
        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(ID id, ObjectT &&obj, OffsetValueT xOffset, OffsetValueT yOffset, OffsetValueT zOffset);
        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(const HandleT &handle);
        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(ID id, const HandleT &handle);
        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(HandleT &&handle);
        // Will return false if the object was not added
        // Guaranteed to return a valid iterator
        // The owner position must be set before this is called
        std::pair<bool, iterator> Add(ID id, HandleT &&handle);

        void Remove(HandleT &remove);
        void Remove(ObjectT &remove);
        void Remove(ID id);
        // Will return the iterator after the given one
        iterator Remove(iterator remove);
        const_iterator Remove(const_iterator remove);

        iterator Find(ID id);
        const_iterator Find(ID id) const;

        ID GetNextAutoID() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        size_type size() const;
        bool empty() const;
    };

    template<class T>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(OffsetManagedList<T>)
    {
        scribe(manager);
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::AddImpl(const HandleT &handle)
    {
        typedef std::pair<bool, iterator> RetT;
        auto size = manager.size();
        auto added = manager.Add(handle);
        if (size != manager.size())
            return RetT(true, manager.Find(added));
        else
            return RetT(false, manager.Find(added));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::AddImpl(ID id, const HandleT &handle)
    {
        typedef std::pair<bool, iterator> RetT;
        auto size = manager.size();
        auto added = manager.Add(id, handle);
        if (size != manager.size())
            return RetT(true, manager.Find(added));
        else
            return RetT(false, manager.Find(added));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::AddImpl(HandleT &&handle)
    {
        typedef std::pair<bool, iterator> RetT;
        auto size = manager.size();
        auto added = manager.Add(std::move(handle));
        if (size != manager.size())
            return RetT(true, manager.Find(added));
        else
            return RetT(false, manager.Find(added));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::AddImpl(ID id, HandleT &&handle)
    {
        typedef std::pair<bool, iterator> RetT;
        auto size = manager.size();
        auto added = manager.Add(id, std::move(handle));
        if (size != manager.size())
            return RetT(true, manager.Find(added));
        else
            return RetT(false, manager.Find(added));
    }

    template<class T>
    void OffsetManagedList<T>::SetManagerOwnerPosition()
    {
        for (auto &loop : manager)
            loop.second.SetOwnerPosition(ownerPosition);
    }

    template<class T>
    OffsetManagedList<T>::OffsetManagedList() : ownerPosition(nullptr)
    {}

    template<class T>
    OffsetManagedList<T>::OffsetManagedList(const Position3D *position) : ownerPosition(position)
    {}

    template<class T>
    OffsetManagedList<T>::OffsetManagedList(const OffsetManagedList &arg) : manager(arg.manager), ownerPosition(nullptr)
    {
        SetManagerOwnerPosition();
    }

    template<class T>
    OffsetManagedList<T>::OffsetManagedList(const OffsetManagedList &arg, const Position3D *ownerPosition) : manager(arg.manager), ownerPosition(ownerPosition)
    {
        SetManagerOwnerPosition();
    }

    template<class T>
    OffsetManagedList<T>& OffsetManagedList<T>::operator=(const OffsetManagedList &arg)
    {
        manager = arg.manager;
        SetManagerOwnerPosition();
        return *this;
    }

    template<class T>
    OffsetManagedList<T>::OffsetManagedList(OffsetManagedList &&arg) : manager(std::move(arg.manager)), ownerPosition(arg.ownerPosition)
    {
        SetManagerOwnerPosition();
    }

    template<class T>
    OffsetManagedList<T>::OffsetManagedList(OffsetManagedList &&arg, const Position3D *ownerPosition) : manager(std::move(arg.manager)), ownerPosition(ownerPosition)
    {
        SetManagerOwnerPosition();
    }

    template<class T>
    OffsetManagedList<T>& OffsetManagedList<T>::operator=(OffsetManagedList &&arg)
    {
        manager = std::move(arg.manager);
        SetManagerOwnerPosition();
        return *this;
    }

    template<class T>
    bool OffsetManagedList<T>::operator==(const OffsetManagedList &arg) const
    {
        return manager == arg.manager && ownerPosition == arg.ownerPosition;
    }

    template<class T>
    bool OffsetManagedList<T>::operator!=(const OffsetManagedList &arg) const
    {
        return !(*this == arg);
    }

    template<class T>
    void OffsetManagedList<T>::SetOwnerPosition(const Position3D *set)
    {
ownerPosition = set;
SetManagerOwnerPosition();
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(const ObjectT &obj)
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(HandleT(obj, 0, 0, 0, ownerPosition));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(const ObjectT &obj, OffsetValueT xOffset, OffsetValueT yOffset, OffsetValueT zOffset)
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(HandleT(obj, xOffset, yOffset, zOffset, ownerPosition));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(ID id, const ObjectT &obj)
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(id, HandleT(obj, ownerPosition));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(ID id, const ObjectT &obj, OffsetValueT xOffset, OffsetValueT yOffset, OffsetValueT zOffset)
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(id, HandleT(obj, xOffset, yOffset, zOffset, ownerPosition));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(ObjectT &&obj)
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(HandleT(std::move(obj), 0, 0, 0, ownerPosition));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(ObjectT &&obj, OffsetValueT xOffset, OffsetValueT yOffset, OffsetValueT zOffset)
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(HandleT(std::move(obj), xOffset, yOffset, zOffset, ownerPosition));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(ID id, ObjectT &&obj)
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(id, HandleT(std::move(obj), 0, 0, 0, ownerPosition));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(ID id, ObjectT &&obj, OffsetValueT xOffset, OffsetValueT yOffset, OffsetValueT zOffset)
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(id, HandleT(std::move(obj), xOffset, yOffset, zOffset, ownerPosition));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(const HandleT &handle)
    {
        typedef std::pair<bool, iterator> RetT;
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(handle);
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(ID id, const HandleT &handle)
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(id, handle);
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(HandleT &&handle)
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(std::move(handle));
    }

    template<class T>
    std::pair<bool, typename OffsetManagedList<T>::iterator> OffsetManagedList<T>::Add(ID id, HandleT &&handle)
    {
        ATMOS_ASSERT_MESSAGE(ownerPosition, "The owner position MUST be set before this is called.");
        return AddImpl(id, std::move(handle));
    }

    template<class T>
    void OffsetManagedList<T>::Remove(HandleT &remove)
    {
        for (auto loop = manager.begin(); loop != manager.end(); ++loop)
        {
            if (loop->second == remove)
            {
                Remove(loop);
                return;
            }
        }
    }

    template<class T>
    void OffsetManagedList<T>::Remove(ObjectT &remove)
    {
        for (auto loop = manager.begin(); loop != manager.end(); ++loop)
        {
            if (*loop->second == remove)
            {
                Remove(loop);
                return;
            }
        }
    }

    template<class T>
    void OffsetManagedList<T>::Remove(ID id)
    {
        manager.Remove(id);
    }

    template<class T>
    typename OffsetManagedList<T>::iterator OffsetManagedList<T>::Remove(iterator remove)
    {
        return manager.Remove(remove);
    }

    template<class T>
    typename OffsetManagedList<T>::const_iterator OffsetManagedList<T>::Remove(const_iterator remove)
    {
        return manager.Remove(remove);
    }

    template<class T>
    typename OffsetManagedList<T>::iterator OffsetManagedList<T>::Find(ID id)
    {
        return manager.Find(id);
    }

    template<class T>
    typename OffsetManagedList<T>::const_iterator OffsetManagedList<T>::Find(ID id) const
    {
        return manager.Find(id);
    }

    template<class T>
    typename OffsetManagedList<T>::ID OffsetManagedList<T>::GetNextAutoID() const
    {
        return manager.GetNextAutoID();
    }

    template<class T>
    typename OffsetManagedList<T>::iterator OffsetManagedList<T>::begin()
    {
        return manager.begin();
    }

    template<class T>
    typename OffsetManagedList<T>::iterator OffsetManagedList<T>::end()
    {
        return manager.end();
    }

    template<class T>
    typename OffsetManagedList<T>::const_iterator OffsetManagedList<T>::begin() const
    {
        return manager.begin();
    }

    template<class T>
    typename OffsetManagedList<T>::const_iterator OffsetManagedList<T>::end() const
    {
        return manager.end();
    }

    template<class T>
    typename OffsetManagedList<T>::size_type OffsetManagedList<T>::size() const
    {
        return manager.Size();
    }

    template<class T>
    bool OffsetManagedList<T>::empty() const
    {
        return manager.IsEmpty();
    }
}