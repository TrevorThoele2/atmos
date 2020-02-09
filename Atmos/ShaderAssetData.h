#pragma once

#include <memory>

namespace Atmos::Asset
{
    class ShaderAssetData
    {
    public:
        using PassCount = unsigned int;
    public:
        virtual ~ShaderAssetData() = 0;

        [[nodiscard]] virtual std::unique_ptr<ShaderAssetData> Clone() const = 0;

        virtual void Reset() = 0;
        virtual void Release() = 0;

        virtual PassCount Begin() const = 0;
        virtual void End() const = 0;

        virtual void BeginNextPass(PassCount pass) const = 0;
        virtual void EndPass() const = 0;
    };
}