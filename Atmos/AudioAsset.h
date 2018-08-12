#pragma once

#include <cstdint>
#include <memory>

#include "Asset.h"

#include "FilePath.h"
#include "Serialization.h"

namespace Atmos
{
    class AudioAsset : public Asset
    {
    public:
        class Instance
        {
        public:
            typedef float Volume;
        private:
            void Serialize(::Inscription::Scribe &scribe);
            INSCRIPTION_ACCESS;
        public:
            class Data
            {
            private:
                Instance *owner;
                friend Instance;
            public:
                virtual ~Data() = 0 {}
                Instance* GetOwner() const;
                virtual Data* Clone() const = 0;
                virtual void Start() = 0;
                virtual void Stop() = 0;
                virtual void SetVolume(Volume set) = 0;
                virtual void Loop(bool set) = 0;
                virtual bool Loop() const = 0;
                virtual bool IsPlaying() const = 0;
                virtual void Resubmit() = 0;
            };
        private:
            std::unique_ptr<Data> data;
            const AudioAsset *asset;

            void SetData(Data *set);
            void SetData(std::unique_ptr<Data> &&set);
        public:
            Instance();
            Instance(const AudioAsset &asset, Data *data);
            Instance(const Instance &arg);
            Instance(Instance &&arg);
            Instance& operator=(const Instance &arg);
            Instance& operator=(Instance &&arg);
            void Start();
            void Stop();

            void SetVolume(Volume set);
            void Loop(bool set);
            bool Loop() const;

            bool IsPlaying() const;

            void Resubmit();

            const AudioAsset& GetOriginal() const;
        };
    public:
        class Data
        {
        private:
            AudioAsset *owner;
            friend AudioAsset;
        public:
            virtual ~Data() = 0 {}
            AudioAsset* GetOwner() const;
            virtual Instance MakeInstance(const AudioAsset &asset) const = 0;
        };
    private:
        std::unique_ptr<Data> data;
        FileName name;

        void SetData(Data *set);
        void SetData(std::unique_ptr<Data> &&set);

        String GetStringImpl() const override final;
    public:
        AudioAsset(Data *data, const FileName &name);
        AudioAsset(AudioAsset &&arg);
        AudioAsset& operator=(AudioAsset &&arg);

        bool operator==(const AudioAsset &arg) const;
        bool operator!=(const AudioAsset &arg) const;

        Instance MakeInstance() const;
        const FileName& GetFileName() const;
    };
}