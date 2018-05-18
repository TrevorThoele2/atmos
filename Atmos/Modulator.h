#pragma once

#include "ModulatorTrack.h"

#include "RandomAccessSequence.h"

namespace Atmos
{
    namespace Modulator
    {
        class ModulatorController;
        class ModulatorBase
        {
        public:
            typedef size_t ID;
            typedef std::unique_ptr<TrackBase> TrackPtr;
        private:
            friend ModulatorController;
        private:
            bool working;
        private:
            virtual ModulatorBase* CloneImpl() const = 0;

            virtual TrackBase* AddTrackImpl(TrackPtr &&track) = 0;
            virtual void RemoveTrackImpl(ID id) = 0;
            virtual TrackBase* FindTrackImpl(ID id) = 0;
            virtual const TrackBase* FindTrackImpl(ID id) const = 0;

            virtual void StopImpl() = 0;
            virtual bool WorkImpl() = 0;
            virtual TimeValue GetSumTimeTakenImpl() const = 0;
        protected:
            void StartBase();
        public:
            ModulatorBase();
            ModulatorBase(const ModulatorBase &arg) = default;
            ModulatorBase(ModulatorBase &&arg);
            ModulatorBase& operator=(const ModulatorBase &arg) = default;
            ModulatorBase& operator=(ModulatorBase &&arg);
            virtual ~ModulatorBase() = 0;

            ModulatorBase* Clone() const;

            TrackBase* AddTrack(TrackPtr &&track);
            void RemoveTrack(ID id);
            TrackBase* FindTrack(ID id);
            const TrackBase* FindTrack(ID id) const;

            void Stop();
            // Returns true if the modulator is done
            bool Work();
            bool IsWorking() const;
            TimeValue GetSumTimeTaken() const;
        };

        template<class Object>
        class Modulator : public ModulatorBase
        {
        public:
            typedef Object ObjectT;
            typedef Track<ObjectT> TrackT;
        private:
            typedef RandomAccessSequence<TrackT> TrackContainer;
        private:
            Modulator* CloneImpl() const override final;

            TrackBase* AddTrackImpl(TrackPtr &&track) override final;
            void RemoveTrackImpl(ID id) override final;
            TrackBase* FindTrackImpl(ID id) override final;
            const TrackBase* FindTrackImpl(ID id) const override final;

            void StopImpl() override final;
            bool WorkImpl() override final;
            TimeValue GetSumTimeTakenImpl() const override final;
        private:
            ObjectT *obj;
            TrackContainer tracks;
            std::vector<TrackT*> tracksWorking;
        public:
            Modulator();
            Modulator(const Modulator &arg);
            Modulator(Modulator &&arg);
            Modulator& operator=(const Modulator &arg);
            Modulator& operator=(Modulator &&arg);
            bool operator==(const Modulator &arg) const;
            bool operator!=(const Modulator &arg) const;

            void Start(ObjectT &obj);
        };

        template<class Object>
        Modulator<Object>* Modulator<Object>::CloneImpl() const
        {
            return new Modulator(*this);
        }

        template<class Object>
        TrackBase* Modulator<Object>::AddTrackImpl(TrackPtr &&track)
        {
            return &*tracks.AddBack(*static_cast<TrackT*>(track.get()));
        }

        template<class Object>
        void Modulator<Object>::RemoveTrackImpl(ID id)
        {
            tracks.Remove(id);
        }

        template<class Object>
        TrackBase* Modulator<Object>::FindTrackImpl(ID id)
        {
            auto found = tracks.Find(id);
            if (found == tracks.end())
                return nullptr;

            return &*found;
        }

        template<class Object>
        const TrackBase* Modulator<Object>::FindTrackImpl(ID id) const
        {
            auto found = tracks.Find(id);
            if (found == tracks.end())
                return nullptr;

            return &*found;
        }

        template<class Object>
        void Modulator<Object>::StopImpl()
        {
            tracksWorking.clear();
        }

        template<class Object>
        bool Modulator<Object>::WorkImpl()
        {
            if (tracksWorking.empty())
                return true;

            for (auto loop = tracksWorking.begin(); loop != tracksWorking.end();)
            {
                if ((*loop)->Work(*obj))
                    loop = tracksWorking.erase(loop);
                else
                    ++loop;
            }

            return tracksWorking.empty();
        }

        template<class Object>
        TimeValue Modulator<Object>::GetSumTimeTakenImpl() const
        {
            TimeValue ret;
            for (auto &loop : tracks)
            {
                TimeValue checkAgainst = loop.GetSumTimeTaken();
                if (ret < checkAgainst)
                    ret = checkAgainst;
            }

            return ret;
        }

        template<class Object>
        Modulator<Object>::Modulator() : obj(nullptr)
        {}

        template<class Object>
        Modulator<Object>::Modulator(const Modulator &arg) : ModulatorBase(arg), obj(arg.obj), tracks(arg.tracks)
        {}

        template<class Object>
        Modulator<Object>::Modulator(Modulator &&arg) : ModulatorBase(std::move(arg)), obj(std::move(arg.obj)), tracks(std::move(arg.tracks))
        {}

        template<class Object>
        Modulator<Object>& Modulator<Object>::operator=(const Modulator &arg)
        {
            ModulatorBase::operator=(arg);
            obj = arg.obj;
            tracks = arg.tracks;
            return *this;
        }

        template<class Object>
        Modulator<Object>& Modulator<Object>::operator=(Modulator &&arg)
        {
            ModulatorBase::operator=(std::move(arg));
            obj = std::move(arg.obj);
            tracks = std::move(arg.tracks);
            return *this;
        }

        template<class Object>
        bool Modulator<Object>::operator==(const Modulator &arg) const
        {
            return obj == arg.obj && tracks == arg.tracks;
        }

        template<class Object>
        bool Modulator<Object>::operator!=(const Modulator &arg) const
        {
            return !(*this == arg);
        }

        template<class Object>
        void Modulator<Object>::Start(ObjectT &obj)
        {
            if(tracks.empty())
                return;

            this->obj = &obj;
            for (auto &loop : tracks)
                tracksWorking.push_back(&loop);

            StartBase();
        }
    }
}