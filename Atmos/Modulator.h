#pragma once

#include "ModulatorTrack.h"
#include "IDManager.h"

namespace Atmos
{
    namespace Modulator
    {
        class ModulatorBase
        {
        public:
            typedef IDManagerBase::ID TrackID;
            typedef std::unique_ptr<TrackBase> TrackPtr;
        private:
            bool working;
            const Name *generatorName;
        private:
            virtual ModulatorBase* CloneImpl() const = 0;

            virtual TrackID AddTrackImpl(TrackPtr &&track) = 0;
            virtual void RemoveTrackImpl(TrackID id) = 0;
            virtual TrackBase* FindTrackImpl(TrackID id) = 0;
            virtual const TrackBase* FindTrackImpl(TrackID id) const = 0;

            virtual void StopImpl() = 0;
            virtual bool WorkImpl() = 0;
            virtual TimeValue GetSumTimeTakenImpl() const = 0;

            virtual bool IsWorkingObjectImpl(void *obj) const = 0;
            virtual std::type_index GetObjectTypeImpl() const = 0;
        protected:
            void StartBase();
        public:
            ModulatorBase(const Name &generatorName);
            ModulatorBase(const ModulatorBase &arg) = default;
            ModulatorBase(ModulatorBase &&arg);
            ModulatorBase& operator=(const ModulatorBase &arg) = default;
            ModulatorBase& operator=(ModulatorBase &&arg);
            virtual ~ModulatorBase() = 0;

            ModulatorBase* Clone() const;

            TrackID AddTrack(TrackPtr &&track);
            void RemoveTrack(TrackID id);
            TrackBase* FindTrack(TrackID id);
            const TrackBase* FindTrack(TrackID id) const;

            void Stop();
            // Returns true if the modulator is done
            bool Work();
            bool IsWorking() const;
            TimeValue GetSumTimeTaken() const;

            const Name& GetGeneratorName() const;

            bool IsWorkingObject(void *obj) const;
            std::type_index GetObjectType() const;
        };

        template<class Object>
        class Modulator : public ModulatorBase
        {
        public:
            typedef Object ObjectT;
            typedef Track<ObjectT> TrackT;
        private:
            typedef IDManager<std::unordered_map<TrackID, TrackT>> TrackContainer;
        private:
            Modulator* CloneImpl() const override final;

            TrackID AddTrackImpl(TrackPtr &&track) override final;
            void RemoveTrackImpl(TrackID id) override final;
            TrackBase* FindTrackImpl(TrackID id) override final;
            const TrackBase* FindTrackImpl(TrackID id) const override final;

            void StopImpl() override final;
            bool WorkImpl() override final;
            TimeValue GetSumTimeTakenImpl() const override final;

            bool IsWorkingObjectImpl(void *obj) const override final;
            std::type_index GetObjectTypeImpl() const override final;
        private:
            ObjectT *obj;
            TrackContainer tracks;
            std::vector<TrackT*> tracksWorking;
        public:
            Modulator(const Name &generatorName);
            Modulator(const Modulator &arg);
            Modulator(Modulator &&arg);
            ~Modulator();
            Modulator& operator=(const Modulator &arg);
            Modulator& operator=(Modulator &&arg);
            bool operator==(const Modulator &arg) const;
            bool operator!=(const Modulator &arg) const;

            void Start(ObjectT &obj);

            ObjectT* GetObject();
            const ObjectT* GetObject() const;
        };

        template<class Object>
        Modulator<Object>* Modulator<Object>::CloneImpl() const
        {
            return new Modulator(*this);
        }

        template<class Object>
        typename Modulator<Object>::TrackID Modulator<Object>::AddTrackImpl(TrackPtr &&track)
        {
            return tracks.Add(*static_cast<TrackT*>(track.get()));
        }

        template<class Object>
        void Modulator<Object>::RemoveTrackImpl(TrackID id)
        {
            tracks.Remove(id);
        }

        template<class Object>
        TrackBase* Modulator<Object>::FindTrackImpl(TrackID id)
        {
            auto found = tracks.Find(id);
            if (found == tracks.end())
                return nullptr;

            return &found->second;
        }

        template<class Object>
        const TrackBase* Modulator<Object>::FindTrackImpl(TrackID id) const
        {
            auto found = tracks.Find(id);
            if (found == tracks.end())
                return nullptr;

            return &found->second;
        }

        template<class Object>
        void Modulator<Object>::StopImpl()
        {
            obj = nullptr;
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
                TimeValue checkAgainst = loop.second.GetSumTimeTaken();
                if (ret < checkAgainst)
                    ret = checkAgainst;
            }

            return ret;
        }

        template<class Object>
        bool Modulator<Object>::IsWorkingObjectImpl(void *obj) const
        {
            return this->obj == obj;
        }

        template<class Object>
        std::type_index Modulator<Object>::GetObjectTypeImpl() const
        {
            return std::type_index(typeid(Object));
        }

        template<class Object>
        Modulator<Object>::Modulator(const Name &generatorName) : ModulatorBase(generatorName), obj(nullptr)
        {}

        template<class Object>
        Modulator<Object>::Modulator(const Modulator &arg) : ModulatorBase(arg), obj(arg.obj), tracks(arg.tracks)
        {}

        template<class Object>
        Modulator<Object>::Modulator(Modulator &&arg) : ModulatorBase(std::move(arg)), obj(std::move(arg.obj)), tracks(std::move(arg.tracks))
        {}

        template<class Object>
        Modulator<Object>::~Modulator()
        {
            StopImpl();
        }

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
            if(IsWorking() || tracks.empty())
                return;

            this->obj = &obj;
            for (auto &loop : tracks)
            {
                tracksWorking.push_back(&loop.second);
                loop.second.Start(obj);
            }

            StartBase();
        }

        template<class Object>
        typename Modulator<Object>::ObjectT* Modulator<Object>::GetObject()
        {
            return obj;
        }

        template<class Object>
        typename const Modulator<Object>::ObjectT* Modulator<Object>::GetObject() const
        {
            return obj;
        }
    }
}