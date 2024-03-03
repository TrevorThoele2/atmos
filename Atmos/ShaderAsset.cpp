
#include "ShaderAsset.h"
#include <fstream>

namespace Atmos
{
    ShaderAsset* ShaderAsset::Data::GetOwner() const
    {
        return owner;
    }

    void ShaderAsset::SetData(Data *set)
    {
        data.reset(set);
        data->owner = this;
    }

    void ShaderAsset::SetData(std::unique_ptr<Data> &&set)
    {
        data = std::move(set);
        data->owner = this;
    }

    String ShaderAsset::GetStringImpl() const
    {
        return name.GetValue();
    }

    ShaderAsset::ShaderAsset(Data *data, const FileName &name) : name(name)
    {
        SetData(data);
    }

    ShaderAsset::ShaderAsset(ShaderAsset &&arg) : name(std::move(arg.name))
    {
        SetData(std::move(arg.data));
    }

    ShaderAsset& ShaderAsset::operator=(ShaderAsset &&arg)
    {
        SetData(std::move(arg.data));
        name = std::move(arg.name);
        return *this;
    }

    bool ShaderAsset::operator==(const ShaderAsset &arg) const
    {
        return Asset::operator==(arg);
    }

    bool ShaderAsset::operator!=(const ShaderAsset &arg) const
    {
        return !(*this == arg);
    }

    ShaderAsset::Data* ShaderAsset::GetData() const
    {
        return data.get();
    }

    void ShaderAsset::Reset()
    {
        data->Reset();
    }

    void ShaderAsset::Release()
    {
        data->Release();
    }

    const FileName& ShaderAsset::GetFileName() const
    {
        return name;
    }

    ShaderAsset::PassT ShaderAsset::Begin() const
    {
        return data->Begin();
    }

    void ShaderAsset::End() const
    {
        data->End();
    }

    void ShaderAsset::BeginNextPass(PassT pass) const
    {
        data->BeginNextPass(pass);
    }

    void ShaderAsset::EndPass() const
    {
        data->EndPass();
    }
}