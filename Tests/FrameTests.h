#pragma once

#include "GeneralFixture.h"

#include <Arca/Curator.h>
#include <Atmos/Work.h>
#include <Atmos/FrameInformation.h>

using namespace Atmos;
using namespace Frame;

class FrameTestsFixture : public GeneralFixture
{
public:
    class WaitCurator final : public Arca::Curator
    {
    public:
        using Curator::Curator;

        void Handle(const Atmos::Work& command);
    };
};

namespace Arca
{
    template<>
    struct Traits<FrameTestsFixture::WaitCurator>
    {
        static const ObjectType objectType = ObjectType::Curator;
        static inline const TypeName typeName = "FrameTestsFixture::WaitCurator";
        using HandledCommands = HandledCommands<Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<FrameTestsFixture::WaitCurator, Archive> final
    {
        using Category = ArcaNullScribeCategory<FrameTestsFixture::WaitCurator>;
    };
}