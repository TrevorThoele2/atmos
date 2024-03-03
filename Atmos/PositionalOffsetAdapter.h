#pragma once

#include "Object.h"
#include "ObjectReference.h"
#include "ObjectException.h"

#include "AxisAlignedObject.h"

#include "StoredProperty.h"

#include "ScopedEventConnection.h"

namespace Atmos
{
    class PositionalOffsetAdapterSourceNotSetException : public ObjectException
    {
    public:
        PositionalOffsetAdapterSourceNotSetException();
    };

    template<class T>
    class PositionalOffsetAdapter;

    template<class T>
    class PositionalOffsetAdapterPosition
    {
    public:
        typedef StoredProperty<Position3D::Value> Property;
    public:
        Property xOffset;
        Property yOffset;
        Property zOffset;
    public:
        operator Position3D() const;

        Position3D Get() const;
    private:
        Position3D underlying;
    private:
        PositionalOffsetAdapterPosition();
        PositionalOffsetAdapterPosition(const PositionalOffsetAdapterPosition& arg);
        PositionalOffsetAdapterPosition(PositionalOffsetAdapterPosition&& arg);

        PositionalOffsetAdapterPosition& operator=(const PositionalOffsetAdapterPosition& arg);
        PositionalOffsetAdapterPosition& operator=(PositionalOffsetAdapterPosition&& arg);

        void Calculate();
    private:
        void SubscribeToProperties();
        void OnPropertyChanged(Position3D::Value value);
    private:
        PositionalOffsetAdapter<T>* owner;
    private:
        using SourceReference = TypedObjectReference<T>;
        SourceReference source;
        ScopedEventConnection sourceConnection;

        void SetSource();
        void SetSource(SourceReference set);
        void OnSourcePositionChanged(Position3D previous);
    private:
        template<class T>
        friend class PositionalOffsetAdapter;
    private:
        INSCRIPTION_ACCESS;
    };

    template<class T>
    PositionalOffsetAdapterPosition<T>::operator Position3D() const
    {
        return Get();
    }

    template<class T>
    Position3D PositionalOffsetAdapterPosition<T>::Get() const
    {
        if (!source)
            throw PositionalOffsetAdapterSourceNotSetException();

        return underlying;
    }

    template<class T>
    PositionalOffsetAdapterPosition<T>::PositionalOffsetAdapterPosition() :
        xOffset(0), yOffset(0), zOffset(0), owner(nullptr)
    {
        SubscribeToProperties();
    }

    template<class T>
    PositionalOffsetAdapterPosition<T>::PositionalOffsetAdapterPosition(const PositionalOffsetAdapterPosition& arg) :
        source(arg.source), xOffset(arg.xOffset), yOffset(arg.yOffset), zOffset(arg.zOffset), owner(nullptr)
    {
        SubscribeToProperties();
    }

    template<class T>
    PositionalOffsetAdapterPosition<T>::PositionalOffsetAdapterPosition(PositionalOffsetAdapterPosition&& arg) :
        source(std::move(arg.source)), xOffset(std::move(arg.xOffset)), yOffset(std::move(arg.yOffset)), zOffset(std::move(arg.zOffset)), owner(nullptr)
    {
        SubscribeToProperties();
    }

    template<class T>
    typename PositionalOffsetAdapterPosition<T>& PositionalOffsetAdapterPosition<T>::operator=(const PositionalOffsetAdapterPosition& arg)
    {
        source = arg.source;
        xOffset = arg.xOffset;
        yOffset = arg.yOffset;
        zOffset = arg.zOffset;
        Calculate();
        return *this;
    }

    template<class T>
    typename PositionalOffsetAdapterPosition<T>& PositionalOffsetAdapterPosition<T>::operator=(PositionalOffsetAdapterPosition&& arg)
    {
        source = std::move(arg.source);
        xOffset = std::move(arg.xOffset);
        yOffset = std::move(arg.yOffset);
        zOffset = std::move(arg.zOffset);
        Calculate();
        return *this;
    }

    template<class T>
    void PositionalOffsetAdapterPosition<T>::Calculate()
    {
        if (!source)
            return;

        underlying.x = source->position.x + xOffset;
        underlying.y = source->position.y + yOffset;
        underlying.z = source->position.z + zOffset;
        owner->PositionHasChanged(underlying);
    }

    template<class T>
    void PositionalOffsetAdapterPosition<T>::SubscribeToProperties()
    {
        xOffset.onValueChanged.Subscribe(&PositionalOffsetAdapterPosition::OnPropertyChanged, *this);
        yOffset.onValueChanged.Subscribe(&PositionalOffsetAdapterPosition::OnPropertyChanged, *this);
        zOffset.onValueChanged.Subscribe(&PositionalOffsetAdapterPosition::OnPropertyChanged, *this);
    }

    template<class T>
    void PositionalOffsetAdapterPosition<T>::OnPropertyChanged(Position3D::Value value)
    {
        Calculate();
    }

    template<class T>
    void PositionalOffsetAdapterPosition<T>::SetSource()
    {
        source = SourceReference();
        sourceConnection.Sever();
    }

    template<class T>
    void PositionalOffsetAdapterPosition<T>::SetSource(SourceReference set)
    {
        source = set;
        sourceConnection.Sever();
        if (set.IsOccupied())
            sourceConnection.Set(source->onPositionChanged.Subscribe(&PositionalOffsetAdapterPosition::OnSourcePositionChanged, *this));
    }

    template<class T>
    void PositionalOffsetAdapterPosition<T>::OnSourcePositionChanged(Position3D previous)
    {
        Calculate();
    }

    template<class T>
    class PositionalOffsetAdapter
    {
    public:
        typedef TypedObjectReference<T> SourceReference;
    public:
        ::Chroma::Event<Position3D> onPositionChanged;
    public:
        using Position = PositionalOffsetAdapterPosition<T>;
        Position position;
    public:
        typedef StoredProperty<SourceReference> SourceProperty;
        SourceProperty source;
    public:
        PositionalOffsetAdapter();
        PositionalOffsetAdapter(SourceReference source);
        PositionalOffsetAdapter(const PositionalOffsetAdapter& arg);
        PositionalOffsetAdapter(PositionalOffsetAdapter&& arg);

        PositionalOffsetAdapter& operator=(const PositionalOffsetAdapter& arg);
        PositionalOffsetAdapter& operator=(PositionalOffsetAdapter&& arg);
    private:
        void SubscribeToProperties();
    private:
        void LinkPosition();
        void CalculatePosition();
        void PositionHasChanged(Position3D dispatch);
    private:
        void OnSourceChanged(SourceReference newValue);
    private:
        friend Position;
    private:
        INSCRIPTION_ACCESS;
    private:
        static_assert(std::is_base_of<AxisAlignedObject, T>::value, "This must be derived from AxisAlignedObject");
    };

    template<class T>
    PositionalOffsetAdapter<T>::PositionalOffsetAdapter()
    {
        LinkPosition();
        CalculatePosition();
        SubscribeToProperties();
    }

    template<class T>
    PositionalOffsetAdapter<T>::PositionalOffsetAdapter(SourceReference source) :
        source(source)
    {
        LinkPosition();
        CalculatePosition();
        SubscribeToProperties();
    }

    template<class T>
    PositionalOffsetAdapter<T>::PositionalOffsetAdapter(const PositionalOffsetAdapter& arg) :
        position(arg.position), source(arg.source)
    {
        LinkPosition();
        CalculatePosition();
        SubscribeToProperties();
    }

    template<class T>
    PositionalOffsetAdapter<T>::PositionalOffsetAdapter(PositionalOffsetAdapter&& arg) :
        position(std::move(arg.position)), source(std::move(arg.source))
    {
        LinkPosition();
        CalculatePosition();
        SubscribeToProperties();
    }

    template<class T>
    PositionalOffsetAdapter<T>& PositionalOffsetAdapter<T>::operator=(const PositionalOffsetAdapter& arg)
    {
        position = arg.position;
        source = arg.source;
        return *this;
    }

    template<class T>
    PositionalOffsetAdapter<T>& PositionalOffsetAdapter<T>::operator=(PositionalOffsetAdapter&& arg)
    {
        position = std::move(arg.position);
        source = std::move(arg.source);
        return *this;
    }

    template<class T>
    void PositionalOffsetAdapter<T>::SubscribeToProperties()
    {
        source.onValueChanged.Subscribe(&PositionalOffsetAdapter::OnSourceChanged, *this);
    }

    template<class T>
    void PositionalOffsetAdapter<T>::LinkPosition()
    {
        position.owner = this;
    }

    template<class T>
    void PositionalOffsetAdapter<T>::CalculatePosition()
    {
        position.Calculate();
    }

    template<class T>
    void PositionalOffsetAdapter<T>::PositionHasChanged(Position3D dispatch)
    {
        onPositionChanged(dispatch);
    }

    template<class T>
    void PositionalOffsetAdapter<T>::OnSourceChanged(SourceReference newValue)
    {
        position.SetSource(newValue);
    }
}

namespace Inscription
{
    template<class T>
    class Scribe<typename ::Atmos::PositionalOffsetAdapterPosition<T>, BinaryArchive> :
        public CompositeScribe<typename ::Atmos::PositionalOffsetAdapterPosition<T>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<typename ::Atmos::PositionalOffsetAdapterPosition<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;
    public:
        static void Scriven(ObjectT& object, ArchiveT& archive)
        {
            archive(object.xOffset);
            archive(object.yOffset);
            archive(object.zOffset);
            archive(object.source);
        }
    };

    template<class T>
    class Scribe<::Atmos::PositionalOffsetAdapter<T>, BinaryArchive> :
        public CompositeScribe<::Atmos::PositionalOffsetAdapter<T>, BinaryArchive>
    {
    private:
        using BaseT = CompositeScribe<typename ::Atmos::PositionalOffsetAdapter<T>, BinaryArchive>;
    public:
        using ObjectT = typename BaseT::ObjectT;
        using ArchiveT = typename BaseT::ArchiveT;

        using BaseT::Scriven;
        using BaseT::Construct;
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override
        {
            archive(object.source);
        }

        void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override
        {
            DoBasicConstruction(storage, archive);
        }

        using BaseT::DoBasicConstruction;
    };
}