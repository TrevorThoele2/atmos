#pragma once

#include <unordered_map>
#include <Inscription/UnorderedMapScribe.h>

#include "IDManagerIterator.h"
#include "IntervalList.h"

#include "Serialization.h"

namespace Atmos
{
    template<class ID, class T>
    class IDManager
    {
    public:
        typedef ID IdentifierT;
        typedef T ValueT;
        typedef size_t SizeT;

        static constexpr IdentifierT nullID = 0;
    private:
        typedef std::unordered_map<IdentifierT, ValueT> ValueMap;
        typedef typename ValueMap::iterator BasicIterator;
        typedef typename ValueMap::const_iterator ConstBasicIterator;
    public:
        typedef IDManagerIteratorBase<ValueT, typename ValueMap::iterator> iterator;
        typedef IDManagerIteratorBase<const ValueT, typename ValueMap::const_iterator> const_iterator;
    public:
        bool operator==(const IDManager& arg) const;
        bool operator!=(const IDManager& arg) const;

        iterator Add(const T& add);
        iterator Add(T&& add);
        iterator Add(IdentifierT id, const T& add);
        iterator Add(IdentifierT id, T&& add);
        iterator Remove(IdentifierT id);
        iterator Remove(iterator itr);
        iterator Remove(const_iterator itr);
        void Clear();
        iterator Find(IdentifierT id);
        const_iterator Find(IdentifierT id) const;

        SizeT Size() const;
        bool IsEmpty() const;

        iterator begin();
        const_iterator begin() const;
        iterator end();
        const_iterator end() const;
    private:
        IntervalList<IdentifierT> occupiedIDs;
        ValueMap values;
    private:
        iterator AddAutomatic(const ValueT& add);
        iterator AddAutomatic(ValueT&& add);
        iterator AddManual(IdentifierT id, const ValueT& add);
        iterator AddManual(IdentifierT id, ValueT&& add);
        iterator AddCommon(IdentifierT id, const ValueT& add);
        iterator AddCommon(IdentifierT id, ValueT&& add);
    private:
        BasicIterator RemoveByID(IdentifierT id);
        BasicIterator RemoveByIterator(BasicIterator itr);
    private:
        IdentifierT BaseAutomaticID() const;
        IdentifierT NextAutomaticID() const;
    private:
        iterator Wrap(BasicIterator itr);
        const_iterator Wrap(ConstBasicIterator itr) const;
        BasicIterator Unwrap(iterator itr);
        ConstBasicIterator Unwrap(const_iterator itr) const;
    private:
        INSCRIPTION_ACCESS;
    private:
        static_assert(std::is_signed<IdentifierT>::value, "IDs are required to be signed in an IDManager");
    };

    template<class ID, class T>
    bool IDManager<ID, T>::operator==(const IDManager& arg) const
    {
        return values == arg.values && occupiedIDs == arg.occupiedIDs;
    }

    template<class ID, class T>
    bool IDManager<ID, T>::operator!=(const IDManager& arg) const
    {
        return !(*this == arg);
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::Add(const T& add)
    {
        return AddAutomatic(add);
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::Add(T&& add)
    {
        return AddAutomatic(std::move(add));
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::Add(IdentifierT id, const T& add)
    {
        return AddManual(id, add);
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::Add(IdentifierT id, T&& add)
    {
        return AddManual(id, std::move(add));
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::Remove(IdentifierT id)
    {
        return RemoveByID(id);
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::Remove(iterator itr)
    {
        return Wrap(RemoveByIterator(Unwrap(itr)));
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::Remove(const_iterator itr)
    {
        return Wrap(RemoveByIterator(Unwrap(itr)));
    }

    template<class ID, class T>
    void IDManager<ID, T>::Clear()
    {
        occupiedIDs.clear();
        values.clear();
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::Find(IdentifierT id)
    {
        return Wrap(values.find(id));
    }

    template<class ID, class T>
    typename IDManager<ID, T>::const_iterator IDManager<ID, T>::Find(IdentifierT id) const
    {
        return Wrap(values.find(id));
    }

    template<class ID, class T>
    typename IDManager<ID, T>::SizeT IDManager<ID, T>::Size() const
    {
        return values.size();
    }

    template<class ID, class T>
    bool IDManager<ID, T>::IsEmpty() const
    {
        return Size() == 0;
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::begin()
    {
        return Wrap(values.begin());
    }

    template<class ID, class T>
    typename IDManager<ID, T>::const_iterator IDManager<ID, T>::begin() const
    {
        return Wrap(values.begin());
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::end()
    {
        return Wrap(values.end());
    }

    template<class ID, class T>
    typename IDManager<ID, T>::const_iterator IDManager<ID, T>::end() const
    {
        return Wrap(values.end());
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::AddAutomatic(const ValueT& add)
    {
        return AddCommon(NextAutomaticID(), add);
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::AddAutomatic(ValueT&& add)
    {
        return AddCommon(NextAutomaticID(), std::move(add));
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::AddManual(IdentifierT id, const ValueT& add)
    {
        return AddCommon(id, add);
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::AddManual(IdentifierT id, ValueT&& add)
    {
        return AddCommon(id, std::move(add));
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::AddCommon(IdentifierT id, const ValueT& add)
    {
        occupiedIDs.Include(id);
        return Wrap(values.emplace(id, add).first);
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::AddCommon(IdentifierT id, ValueT&& add)
    {
        occupiedIDs.Include(id);
        return Wrap(values.emplace(id, std::move(add)).first);
    }

    template<class ID, class T>
    typename IDManager<ID, T>::BasicIterator IDManager<ID, T>::RemoveByID(IdentifierT id)
    {
        return RemoveByIterator(values.find(id));
    }

    template<class ID, class T>
    typename IDManager<ID, T>::BasicIterator IDManager<ID, T>::RemoveByIterator(BasicIterator itr)
    {
        occupiedIDs.Remove(itr->first);
        return values.erase(itr);
    }

    template<class ID, class T>
    typename IDManager<ID, T>::IdentifierT IDManager<ID, T>::BaseAutomaticID() const
    {
        return -1;
    }

    template<class ID, class T>
    typename IDManager<ID, T>::IdentifierT IDManager<ID, T>::NextAutomaticID() const
    {
        auto itr = occupiedIDs.begin();
        if (itr == occupiedIDs.end() || itr->Start() > BaseAutomaticID())
            return BaseAutomaticID();
        else
            return itr->Start() - 1;
    }

    template<class ID, class T>
    typename IDManager<ID, T>::iterator IDManager<ID, T>::Wrap(BasicIterator itr)
    {
        return iterator(itr);
    }

    template<class ID, class T>
    typename IDManager<ID, T>::const_iterator IDManager<ID, T>::Wrap(ConstBasicIterator itr) const
    {
        return const_iterator(itr);
    }

    template<class ID, class T>
    typename IDManager<ID, T>::BasicIterator IDManager<ID, T>::Unwrap(iterator itr)
    {
        return itr.wrapped;
    }

    template<class ID, class T>
    typename IDManager<ID, T>::ConstBasicIterator IDManager<ID, T>::Unwrap(const_iterator itr) const
    {
        return itr.wrapped;
    }
}

namespace Inscription
{
    template<class ID, class T>
    class Scribe<::Atmos::IDManager<ID, T>, BinaryArchive> :
        public CompositeScribe<::Atmos::IDManager<ID, T>, BinaryArchive>
    {
    private:
        using BaseT = typename CompositeScribe<::Atmos::IDManager<ID, T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using BaseT::Scriven;
        using BaseT::Construct;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.values);

            if (archive.IsInput())
            {
                object.occupiedIDs.Clear();
                for (auto& loop : object.values)
                    object.occupiedIDs.Include(loop.first);
            }
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }

        using BaseT::DoBasicConstruction;
    };
}