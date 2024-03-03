
#pragma once

#include <Inscription\Scribe.h>

namespace Atmos
{
    namespace Detail
    {
        template<class Scribe>
        class SkipHelperBase
        {
        public:
            typedef Scribe ScribeT;
        protected:
            typedef typename Scribe::StreamPos Pos;
        protected:
            Scribe &scribe;
            Pos pos;
        public:
            SkipHelperBase(ScribeT &scribe);
            ScribeT& GetScribe();
            const ScribeT& GetScribe() const;
        };

        template<class Scribe>
        SkipHelperBase<Scribe>::SkipHelperBase(ScribeT &scribe) : scribe(scribe)
        {}

        template<class Scribe>
        typename SkipHelperBase<Scribe>::ScribeT& SkipHelperBase<Scribe>::GetScribe()
        {
            return scribe;
        }

        template<class Scribe>
        typename const SkipHelperBase<Scribe>::ScribeT& SkipHelperBase<Scribe>::GetScribe() const
        {
            return scribe;
        }
    }

    template<class Scribe>
    class SkipSaver : public Detail::SkipHelperBase<Scribe>
    {
    private:
        typedef Detail::SkipHelperBase<Scribe> BaseT;
        typedef typename BaseT::Pos Pos;
    private:
        virtual void OnBeforeObjectSave();
        virtual void OnAfterObjectSave();

        virtual void SavePlaceholderExtra();
        virtual void SaveExtra();

        void OverwritePlaceholder(Pos overwrite);
    public:
        SkipSaver(ScribeT &scribe);
        void SavePlaceholder();
        // Will take the current position and overwrite the placeholder with it still
        void SaveNothing();
        template<class T>
        void SaveObject(T &obj);
        void SaveBuffer(const inscription::Buffer &buffer);
    };

    template<class Scribe>
    void SkipSaver<Scribe>::OnBeforeObjectSave()
    {}

    template<class Scribe>
    void SkipSaver<Scribe>::OnAfterObjectSave()
    {}

    template<class Scribe>
    void SkipSaver<Scribe>::SavePlaceholderExtra()
    {}

    template<class Scribe>
    void SkipSaver<Scribe>::SaveExtra()
    {}

    template<class Scribe>
    void SkipSaver<Scribe>::OverwritePlaceholder(Pos overwrite)
    {
        Pos currentPos = scribe.TellStream();
        scribe.SeekStream(pos);
        scribe.Save(overwrite);
        SaveExtra();
        scribe.SeekStream(currentPos);
    }

    template<class Scribe>
    SkipSaver<Scribe>::SkipSaver(ScribeT &scribe) : SkipHelperBase(scribe)
    {}

    template<class Scribe>
    void SkipSaver<Scribe>::SavePlaceholder()
    {
        pos = scribe.TellStream();
        scribe.Save(Pos());
        SavePlaceholderExtra();
    }

    template<class Scribe>
    void SkipSaver<Scribe>::SaveNothing()
    {
        Pos currentPos = scribe.TellStream();

        OnBeforeObjectSave();
        OnAfterObjectSave();

        OverwritePlaceholder(currentPos);
    }

    template<class Scribe>
    template<class T>
    void SkipSaver<Scribe>::SaveObject(T &obj)
    {
        Pos currentPos = scribe.TellStream();

        OnBeforeObjectSave();
        scribe.Save(obj);
        OnAfterObjectSave();

        OverwritePlaceholder(currentPos);
    }

    template<class Scribe>
    void SkipSaver<Scribe>::SaveBuffer(const inscription::Buffer &buffer)
    {
        Pos currentPos = scribe.TellStream();

        OnBeforeObjectSave();
        scribe.WriteBuffer(buffer);
        OnAfterObjectSave();

        OverwritePlaceholder(currentPos);
    }

    template<class Scribe>
    class SkipHandle : public Detail::SkipHelperBase<Scribe>
    {
    private:
        typedef Detail::SkipHelperBase<Scribe> BaseT;
    public:
        typedef typename BaseT::Pos Pos;
    private:
        virtual void OnBeforeObjectLoad();
        virtual void OnAfterObjectLoad();

        virtual void LoadExtra();
    public:
        SkipHandle(ScribeT &scribe);
        Pos LoadPosition();
        template<class T>
        void LoadObject(T &obj);
        inscription::Buffer LoadBuffer(inscription::Buffer::SizeT size);

        Pos GetSkipPosition() const;
    };

    template<class Scribe>
    void SkipHandle<Scribe>::OnBeforeObjectLoad()
    {}

    template<class Scribe>
    void SkipHandle<Scribe>::OnAfterObjectLoad()
    {}

    template<class Scribe>
    void SkipHandle<Scribe>::LoadExtra()
    {}

    template<class Scribe>
    SkipHandle<Scribe>::SkipHandle(ScribeT &scribe) : SkipHelperBase(scribe)
    {}

    template<class Scribe>
    typename SkipHandle<Scribe>::Pos SkipHandle<Scribe>::LoadPosition()
    {
        scribe.Load(pos);
        LoadExtra();
        return pos;
    }

    template<class Scribe>
    template<class T>
    void SkipHandle<Scribe>::LoadObject(T &obj)
    {
        auto currentPos = scribe.TellStream();
        scribe.SeekStream(pos);

        OnBeforeObjectLoad();
        scribe.Load(obj);
        OnAfterObjectLoad();

        scribe.SeekStream(currentPos);
    }

    template<class Scribe>
    inscription::Buffer SkipHandle<Scribe>::LoadBuffer(inscription::Buffer::SizeT size)
    {
        auto currentPos = scribe.TellStream();
        scribe.SeekStream(pos);

        OnBeforeObjectLoad();
        inscription::Buffer buffer(size);
        scribe.ReadBuffer(buffer);
        OnAfterObjectLoad();

        scribe.SeekStream(currentPos);

        return buffer;
    }

    template<class Scribe>
    typename SkipHandle<Scribe>::Pos SkipHandle<Scribe>::GetSkipPosition() const
    {
        return pos;
    }

    /*
    template<class Scribe, class ID>
    class SkipSaverExtended : public detail::SkipHelperBase<Scribe>
    {
    public:
        typedef ID IDType;
    private:
        virtual void SavePlaceholderExtra();
        virtual void SaveExtra();
    public:
        SkipSaverExtended(ScribeT &scribe);
        void SavePlaceholder();
        template<class T>
        void SaveObject(ID id, T &obj);
    };

    template<class Scribe, class ID>
    void SkipSaverExtended<Scribe, ID>::SavePlaceholderExtra()
    {}
    
    template<class Scribe, class ID>
    void SkipSaverExtended<Scribe, ID>::SaveExtra()
    {}

    template<class Scribe, class ID>
    SkipSaverExtended<Scribe, ID>::SkipSaverExtended(ScribeT &scribe) : SkipHelperBase(scribe)
    {}

    template<class Scribe, class ID>
    void SkipSaverExtended<Scribe, ID>::SavePlaceholder()
    {
        pos = scribe.TellStream();
        scribe(Pos());
        scribe(ID());
    }

    template<class Scribe, class ID>
    template<class T>
    void SkipSaverExtended<Scribe, ID>::SaveObject(ID id, T &obj)
    {
        Pos currentPos = scribe.TellStream();

        scribe.SeekStream(pos);
        scribe.Save(currentPos);
        scribe.Save(id);

        scribe.SeekStream(currentPos);
        scribe.Save(obj);
    }
    // SkipSaverExtended

    template<class Scribe, class ID>
    class SkipHandleExtended : public detail::SkipHelperBase<Scribe>
    {
    public:
        typedef ID IDType;
    public:
        SkipHandleExtended(ScribeT &scribe);
        void LoadPlaceholder(ID &id);
        template<class T>
        void LoadObject(T &obj);
        inscription::Buffer LoadBuffer(inscription::Buffer::SizeT size);
    };

    template<class Scribe, class ID>
    SkipHandleExtended<Scribe, ID>::SkipHandleExtended(ScribeT &scribe) : SkipHelperBase(scribe)
    {}

    template<class Scribe, class ID>
    void SkipHandleExtended<Scribe, ID>::LoadPlaceholder(ID &id)
    {
        scribe.Load(pos);
        scribe.Load(id);
    }

    template<class Scribe, class ID>
    template<class T>
    void SkipHandleExtended<Scribe, ID>::LoadObject(T &obj)
    {
        auto currentPos = scribe.TellStream();
        scribe.SeekStream(pos);
        scribe.Load(obj);
        scribe.SeekStream(currentPos);
    }

    template<class Scribe, class ID>
    inscription::Buffer SkipHandleExtended<Scribe, ID>::LoadBuffer(inscription::Buffer::SizeT size)
    {
        auto currentPos = scribe.TellStream();
        scribe.SeekStream(pos);

        inscription::Buffer buffer;
        scribe.ReadBuffer(buffer);
        scribe.SeekStream(currentPos);

        return buffer;
    }
    // SkipHandleExtended
    */
}