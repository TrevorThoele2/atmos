
#pragma once

#include <memory>

#include "Asset.h"

#include "FilePath.h"

namespace Atmos
{
    class ShaderAsset : public Asset
    {
    public: typedef unsigned int PassT;
    public:
        class Data
        {
        public:
            typedef ShaderAsset::PassT PassT;
        private:
            ShaderAsset *owner;
            friend ShaderAsset;
        public:
            virtual ~Data() = 0 {}
            ShaderAsset* GetOwner() const;
            virtual void Reset() = 0;
            virtual void Release() = 0;
            virtual PassT Begin() const = 0;
            virtual void End() const = 0;
            virtual void BeginNextPass(PassT pass) const = 0;
            virtual void EndPass() const = 0;
        };
    private:
        FileName name;
        std::unique_ptr<Data> data;
        void SetData(Data *set);
        void SetData(std::unique_ptr<Data> &&set);

        String GetStringImpl() const override final;
    public:
        ShaderAsset(Data *data, const FileName &name);
        ShaderAsset(ShaderAsset &&arg);
        ShaderAsset& operator=(ShaderAsset &&arg);

        bool operator==(const ShaderAsset &arg) const;
        bool operator!=(const ShaderAsset &arg) const;

        Data* GetData() const;
        template<class DataT>
        DataT* GetData() const;

        void Reset();
        void Release();

        const FileName& GetFileName() const;

        PassT Begin() const;
        void End() const;
        void BeginNextPass(PassT pass) const;
        void EndPass() const;
    };

    template<class DataT>
    DataT* ShaderAsset::GetData() const
    {
        return static_cast<DataT*>(data.get());
    }
}