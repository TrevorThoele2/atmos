#pragma once

#include <memory>
#include <vector>

#include "ObjectManager.h"
#include "ObjectTypeGraph.h"

#include <Inscription/Scribe.h>

namespace Atmos
{
    class ObjectRegistration
    {
    private:
        class Impl;
    public:
        ObjectRegistration();

        void RegisterGlobal(ObjectManager& manager);
        void RegisterLocal(ObjectManager& manager);

        void Register(ObjectTypeGraph& graph);

        void Register(::Inscription::Scribe& scribe);
    private:
        void StoreAllGlobals();
        void StoreAllLocals();
    private:
        typedef std::unique_ptr<Impl> ImplPtr;
        static ImplPtr impl;

        static void EnsureImplCreated();
    };
}