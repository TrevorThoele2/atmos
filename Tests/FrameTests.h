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
        static constexpr ObjectType objectType = ObjectType::Curator;
        static const inline TypeName typeName = "FrameTestsFixture::WaitCurator";
        using HandledCommands = HandledCommands<Atmos::Work>;
    };
}

namespace Inscription
{
    template<class Format>
    struct ScribeTraits<FrameTestsFixture::WaitCurator, Format> final
    {
        using Category = ArcaNullScribeCategory<FrameTestsFixture::WaitCurator>;
    };
}