#pragma once

#include <vector>
#include <unordered_map>
#include "IDSequencer.h"

#include "Serialization.h"
#include <Inscription\ContainerSize.h>
#include <Inscription\Vector.h>

namespace Atmos
{
    class IDManagerBase
    {
    public:
        typedef std::int32_t ID;
    };

    template<class ContainerT>
    struct IDManagerTraits;

    template<class T, class Alloc>
    struct IDManagerTraits<std::vector<T, Alloc>>
    {
        typedef std::vector<T, Alloc> ContainerT;
        typedef T ObjectT;

        static typename ContainerT::iterator Add(ContainerT &cont, IDManagerBase::ID id, ObjectT &&add)
        {
            cont.push_back(std::move(add));
            return --cont.end();
        }

        static ObjectT* Unpack(ContainerT &cont, typename ContainerT::iterator itr)
        {
            if (itr == cont.end())
                return nullptr;
            else
                return &*itr;
        }
    };

    template<class T, class Hasher, class KeyEq, class Alloc>
    struct IDManagerTraits<std::unordered_map<IDManagerBase::ID, T, Hasher, KeyEq, Alloc>>
    {
        typedef std::unordered_map<IDManagerBase::ID, T, Hasher, KeyEq, Alloc> ContainerT;
        typedef T ObjectT;

        static typename ContainerT::iterator Add(ContainerT &cont, IDManagerBase::ID id, T &&add)
        {
            return cont.emplace(id, std::move(add)).first;
        }

        static ObjectT* Unpack(ContainerT &cont, typename ContainerT::iterator itr)
        {
            if (itr == cont.end())
                return nullptr;
            else
                return &itr->second;
        }
    };

    template<class Container>
    class IDManager : public IDManagerBase
    {
    public:
        typedef Container ContainerType;
    private:
        typedef IDManagerTraits<ContainerType> Traits;
    public:
        typedef typename Traits::ObjectT Object;
        typedef typename ContainerType::iterator iterator;
        typedef typename ContainerType::const_iterator const_iterator;
        typedef typename ContainerType::size_type size_type;

        typedef IDManagerBase::ID ID;
        static const ID nullID = 0;
    private:
        typedef std::vector<iterator> IDs;

        typedef long long WideID;
    private:
        IDs positive;
        IDs negative;
        IDSequencerStatic<ID, -1, std::numeric_limits<ID>::min()> idSequencer;

        ContainerType container;

        ID AddImpl(Object &&obj);
        ID AddImpl(ID id, Object &&obj);
        // Returns (if removed, iterator after removed iterator)
        std::pair<bool, iterator> RemoveImpl(iterator remove, typename IDs::iterator idItr, IDs &ids);
        // Returns (if removed, iterator after removed iterator)
        std::pair<bool, iterator> RemoveImpl(iterator remove, size_t position, IDs &ids);
        // Returns (if removed, iterator after removed iterator)
        std::pair<bool, const_iterator> RemoveImpl(const_iterator remove, typename IDs::iterator idItr, IDs &ids);
        // Returns (if removed, iterator after removed iterator)
        std::pair<bool, const_iterator> RemoveImpl(const_iterator remove, size_t position, IDs &ids);

        template<class LeftT, class RightT>
        inline bool CompareGreater(LeftT left, RightT right) const;
        template<class LeftT, class RightT>
        inline bool CompareGreaterEqual(LeftT left, RightT right) const;
        template<class LeftT, class RightT>
        inline bool CompareLesser(LeftT left, RightT right) const;
        template<class LeftT, class RightT>
        inline bool CompareLesserEqual(LeftT left, RightT right) const;
    public:
        IDManager() = default;
        IDManager(const IDManager &arg) = default;
        IDManager& operator=(const IDManager &arg) = default;
        IDManager(IDManager &&arg);
        IDManager& operator=(IDManager &&arg);
        bool operator==(const IDManager &arg) const;
        bool operator!=(const IDManager &arg) const;

        // Will return nullID if the subcomponent was not created
        ID Add(const Object &add);
        // Will return nullID if the subcomponent was not created
        ID Add(Object &&add);
        // Will return nullID if the subcomponent was not created
        ID Add(ID id, const Object &add);
        // Will return nullID if the subcomponent was not created
        ID Add(ID id, Object &&add);
        // Returns if the subcomponent was destroyed
        bool Remove(ID id);
        // If you supply true to this, then it will be assumed that the object has a positive ID. If false, a negative one.
        // Returns the iterator after the removed
        iterator Remove(iterator remove, bool hint = true);
        // If you supply true to this, then it will be assumed that the object has a positive ID. If false, a negative one.
        // Returns the iterator after the removed
        const_iterator Remove(const_iterator remove, bool hint = true);
        void Clear();
        iterator Find(ID id);
        const_iterator Find(ID id) const;

        bool CanCreate() const;
        bool CanCreate(ID id) const;

        ID GetUpperLimit() const;
        ID GetLowerLimit() const;
        // Returns the ID that will be used if letting the manager select the ID
        ID GetNextAutoID() const;

        bool IsEmpty() const;
        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;
        size_type size() const;
        size_type Size() const;
        bool empty() const;
        void clear();
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    template<class Container>
    typename IDManager<Container>::ID IDManager<Container>::AddImpl(Object &&obj)
    {
        ID id = idSequencer.GetNextID();
        iterator added = Traits::Add(container, id, std::move(obj));
        ID negateID = (-1) * id;
        (CompareGreater(negateID, negative.size())) ? negative.push_back(added) : negative[negateID - 1] = added;

        idSequencer.ReserveNext();
        return id;
    }

    template<class Container>
    typename IDManager<Container>::ID IDManager<Container>::AddImpl(ID id, Object &&obj)
    {
        // If the ID is less than or equal to null ID, then just add through the automatic ID adder
        if (id <= nullID)
            return AddImpl(std::move(obj));

        iterator added = Traits::Add(container, id, std::move(obj));
        // Check if ID is greater than the current size
        if (CompareLesser(positive.size(), id))
        {
            positive.resize(id);
            positive[id - 1] = added;
            return id;
        }

        iterator &object = positive[id - 1];

        // If the object at the ID the user wants to create at is already occupied
        if (object != container.end())
            // Don't create anything
            return nullID;

        object = added;
        return id;
    }

    template<class Container>
    std::pair<bool, typename IDManager<Container>::iterator> IDManager<Container>::RemoveImpl(iterator remove, typename IDs::iterator idItr, IDs &ids)
    {
        return RemoveImpl(remove, idItr - ids.begin(), ids);
    }

    template<class Container>
    std::pair<bool, typename IDManager<Container>::iterator> IDManager<Container>::RemoveImpl(iterator remove, size_t position, IDs &ids)
    {
        typedef std::pair<bool, iterator> Ret;
        if (remove == container.end())
            return Ret(false, container.end());

        if (CompareGreaterEqual(position, ids.size()))
            return Ret(false, container.end());

        if ((position + 1) == ids.size())
            ids.pop_back();
        else
        {
            iterator &focused = ids[position];
            if (focused == container.end())
                return Ret(false, ++remove);

            ids[position] = container.end();
        }

        if (&ids == &negative)
            idSequencer.IDRemoved(-1 * (position + 1));

        return Ret(true, container.erase(remove));
    }

    template<class Container>
    std::pair<bool, typename IDManager<Container>::const_iterator> IDManager<Container>::RemoveImpl(const_iterator remove, typename IDs::iterator idItr, IDs &ids)
    {
        return RemoveImpl(remove, idItr - ids.begin(), ids);
    }

    template<class Container>
    std::pair<bool, typename IDManager<Container>::const_iterator> IDManager<Container>::RemoveImpl(const_iterator remove, size_t position, IDs &ids)
    {
        typedef std::pair<bool, const_iterator> Ret;
        if (remove == container.end())
            return Ret(false, container.end());

        if (CompareGreaterEqual(position, ids.size()))
            return Ret(false, container.end());

        if ((position + 1) == ids.size())
            ids.pop_back();
        else
        {
            iterator &focused = ids[position];
            if (focused == container.end())
                return Ret(false, ++remove);

            ids[position] = container.end();
        }

        if (&ids == &negative)
            idSequencer.IDRemoved(-1 * (position + 1));

        return Ret(true, container.erase(remove));
    }

    template<class Container>
    template<class LeftT, class RightT>
    bool IDManager<Container>::CompareGreater(LeftT left, RightT right) const
    {
        WideID useLeft = left;
        WideID useRight = right;
        return useLeft > useRight;
    }

    template<class Container>
    template<class LeftT, class RightT>
    bool IDManager<Container>::CompareGreaterEqual(LeftT left, RightT right) const
    {
        WideID useLeft = left;
        WideID useRight = right;
        return useLeft >= useRight;
    }

    template<class Container>
    template<class LeftT, class RightT>
    bool IDManager<Container>::CompareLesser(LeftT left, RightT right) const
    {
        WideID useLeft = left;
        WideID useRight = right;
        return useLeft < useRight;
    }

    template<class Container>
    template<class LeftT, class RightT>
    bool IDManager<Container>::CompareLesserEqual(LeftT left, RightT right) const
    {
        WideID useLeft = left;
        WideID useRight = right;
        return useLeft <= useRight;
    }

    template<class Container>
    IDManager<Container>::IDManager(IDManager &&arg) : positive(std::move(arg.positive)), negative(std::move(arg.negative)), idSequencer(std::move(arg.idSequencer)), container(std::move(arg.container))
    {}

    template<class Container>
    IDManager<Container>& IDManager<Container>::operator=(IDManager &&arg)
    {
        positive = std::move(arg.positive);
        negative = std::move(arg.negative);
        idSequencer = std::move(arg.idSequencer);
        container = std::move(arg.container);
        return *this;
    }

    template<class Container>
    bool IDManager<Container>::operator==(const IDManager &arg) const
    {
        return positive == arg.positive && negative == arg.negative && idSequencer == arg.idSequencer && container == arg.container;
    }

    template<class Container>
    bool IDManager<Container>::operator!=(const IDManager &arg) const
    {
        return !(*this == arg);
    }

    template<class Container>
    typename IDManager<Container>::ID IDManager<Container>::Add(const Object &add)
    {
        if (!idSequencer.CanMakeAnother())
            return nullID;

        return AddImpl(Object(add));
    }

    template<class Container>
    typename IDManager<Container>::ID IDManager<Container>::Add(Object &&add)
    {
        if (!idSequencer.CanMakeAnother())
            return nullID;

        return AddImpl(std::move(add));
    }

    template<class Container>
    typename IDManager<Container>::ID IDManager<Container>::Add(ID id, const Object &add)
    {
        return AddImpl(id, Object(add));
    }

    template<class Container>
    typename IDManager<Container>::ID IDManager<Container>::Add(ID id, Object &&add)
    {
        return AddImpl(id, std::move(add));
    }

    template<class Container>
    bool IDManager<Container>::Remove(ID id)
    {
        if (id > 0)
            return RemoveImpl(positive[id - 1], positive.begin() + id, positive).first;
        else
            return RemoveImpl(negative[id - 1], negative.begin() + (-id), negative).first;
    }

    template<class Container>
    typename IDManager<Container>::iterator IDManager<Container>::Remove(iterator remove, bool hint)
    {
        auto destroyer = [&](IDs &vector)
        {
            for (auto loop = vector.begin(); loop != vector.end(); ++loop)
            {
                if (remove == *loop)
                    return RemoveImpl(remove, loop, vector).second;
            }

            return container.end();
        };

        if (hint)
        {
            auto destroyed = destroyer(positive);
            if (destroyed == container.end())
                return destroyer(negative);
            else
                return destroyed;
        }
        else
        {
            auto destroyed = destroyer(negative);
            if (destroyed == container.end())
                return destroyer(positive);
            else
                return destroyed;
        }
    }

    template<class Container>
    typename IDManager<Container>::const_iterator IDManager<Container>::Remove(const_iterator remove, bool hint)
    {
        auto destroyer = [&](IDs &vector)
        {
            for (auto loop = vector.begin(); loop != vector.end(); ++loop)
            {
                if (remove == *loop)
                    return RemoveImpl(remove, loop, vector).second;
            }

            return const_iterator(container.end());
        };

        if (hint)
        {
            auto destroyed = destroyer(positive);
            if (destroyed == container.end())
                return destroyer(negative);
            else
                return destroyed;
        }
        else
        {
            auto destroyed = destroyer(negative);
            if (destroyed == container.end())
                return destroyer(positive);
            else
                return destroyed;
        }
    }

    template<class Container>
    void IDManager<Container>::Clear()
    {
        positive.clear();
        negative.clear();
        idSequencer.Reset();
        container.clear();
    }

    template<class Container>
    typename IDManager<Container>::iterator IDManager<Container>::Find(ID id)
    {
        if (id == nullID)
            return container.end();

        auto finder = [&](IDs &vector, ID useID) -> iterator
        {
            if (CompareLesser(vector.size(), useID))
                return container.end();

            return vector[useID - 1];
        };

        if (id > 0)
            return finder(positive, id);
        else
            return finder(negative, -id);
    }

    template<class Container>
    typename IDManager<Container>::const_iterator IDManager<Container>::Find(ID id) const
    {
        if (id == nullID)
            return container.end();

        auto finder = [&](const IDs &vector, ID useID) -> const_iterator
        {
            if (CompareLesser(vector.size(), useID))
                return container.end();

            return vector[useID - 1];
        };

        if (id > 0)
            return finder(positive, id);
        else
            return finder(negative, -id);
    }

    template<class Container>
    bool IDManager<Container>::CanCreate() const
    {
        return idSequencer.CanMakeAnother();
    }

    template<class Container>
    bool IDManager<Container>::CanCreate(ID id) const
    {
        if (id == nullID)
            return false;

        if (id < positive.size())
            return positive[id - 1];

        return true;
    }

    template<class Container>
    typename IDManager<Container>::ID IDManager<Container>::GetUpperLimit() const
    {
        return std::numeric_limits<ID>::max();
    }

    template<class Container>
    typename IDManager<Container>::ID IDManager<Container>::GetLowerLimit() const
    {
        return idSequencer.GetLowerLimit();
    }

    template<class Container>
    typename IDManager<Container>::ID IDManager<Container>::GetNextAutoID() const
    {
        return idSequencer.GetNextID();
    }

    template<class Container>
    bool IDManager<Container>::IsEmpty() const
    {
        return empty();
    }

    template<class Container>
    typename IDManager<Container>::iterator IDManager<Container>::begin()
    {
        return container.begin();
    }

    template<class Container>
    typename IDManager<Container>::iterator IDManager<Container>::end()
    {
        return container.end();
    }

    template<class Container>
    typename IDManager<Container>::const_iterator IDManager<Container>::begin() const
    {
        return container.begin();
    }

    template<class Container>
    typename IDManager<Container>::const_iterator IDManager<Container>::end() const
    {
        return container.end();
    }

    template<class Container>
    typename IDManager<Container>::size_type IDManager<Container>::size() const
    {
        return container.size();
    }

    template<class Container>
    typename IDManager<Container>::size_type IDManager<Container>::Size() const
    {
        return size();
    }

    template<class Container>
    bool IDManager<Container>::empty() const
    {
        return container.empty();
    }

    template<class Container>
    void IDManager<Container>::clear()
    {
        Clear();
    }

    template<class Container>
    INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(IDManager<Container>)
    {
        typedef size_t Position;
        const Position nullPosition = 0;
        struct Hasher
        {
            size_t operator()(const iterator &i) const
            {
                return std::hash<typename iterator::pointer>()(&*i);
            }
        };

        // Handle IDs and sequencer
        if (scribe.IsOutput())
        {
            std::unordered_map<iterator, Position, Hasher> positionMap;

            // Container
            {
                ::Inscription::ContainerSize size(container.size());
                scribe.Save(size);

                Position position = nullPosition + 1;
                for (auto loop = container.begin(); loop != container.end(); ++loop)
                {
                    scribe.Save(*Traits::Unpack(container, loop));
                    positionMap.emplace(loop, position);
                    ++position;
                }
            }

            auto saver = [&](IDs &ids)
            {
                ::Inscription::ContainerSize size(ids.size());
                scribe.Save(size);

                for (auto &loop : ids)
                {
                    if (loop != container.end())
                        scribe.WriteNumeric(positionMap.find(loop)->second);
                    else
                        scribe.WriteNumeric(nullPosition);
                }
            };

            saver(positive);
            saver(negative);
        }
        else
        {
            std::unordered_map<Position, iterator> positionMap;

            // Container
            {
                container.clear();

                ::Inscription::ContainerSize size;
                scribe.Load(size);

                Position curPosition = nullPosition + 1;
                while (size-- > 0)
                {
                    ::Inscription::StackConstructor<Object> constructor(scribe);
                    auto itr = Traits::Add(container, curPosition, std::move(constructor.GetMove()));
                    positionMap.emplace(curPosition, itr);
                    scribe.ReplaceTrackedObject(*constructor.Get(), *Traits::Unpack(container, itr));
                    ++curPosition;
                }
            }

            // Positive
            {
                positive.clear();

                ::Inscription::ContainerSize size;
                scribe.Load(size);

                for(ID loop = 0; loop != size.Get(); ++loop)
                {
                    size_t pos = nullPosition;
                    scribe.ReadNumeric(pos);
                    (pos != nullPosition) ? positive.push_back(positionMap.find(pos)->second) : positive.push_back(container.end());
                }
            }

            // Negative
            {
                negative.clear();

                ::Inscription::ContainerSize size;
                scribe.Load(size);

                for (ID loop = 0; loop != size.Get(); ++loop)
                {
                    Position pos = nullPosition;
                    scribe.ReadNumeric(pos);
                    if (pos != nullPosition)
                    {
                        negative.push_back(positionMap.find(pos)->second);
                        idSequencer.Reserve(-(loop + 1));
                    }
                    else
                        negative.push_back(container.end());
                }
            }
        }
    }
}