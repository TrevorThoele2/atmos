#pragma once

namespace Atmos::Scripting
{
    class ReferenceCounted
    {
    public:
        ReferenceCounted();
        virtual ~ReferenceCounted() = 0;

        void AddRef();
        void Release();
    private:
        int refCount;
    };
}