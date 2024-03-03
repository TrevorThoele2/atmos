#pragma once

#include <optional>

#include <Arca/ClosedTypedRelic.h>

#include "RealStopwatch.h"
#include "Position2D.h"
#include "GridPosition.h"
#include "FieldID.h"
#include "WindowBase.h"

namespace Atmos::Debug
{
    class Statistics final : public Arca::ClosedTypedRelic<Statistics>
    {
    public:
        explicit Statistics(Init init);
    public:
        struct Memory
        {
            size_t audioAssetSize = 0;
            size_t imageAssetSize = 0;
            size_t materialAssetSize = 0;
            size_t scriptAssetSize = 0;
            size_t shaderAssetSize = 0;

            size_t tileSize = 0;
            size_t entitySize = 0;

            size_t workingScriptSize = 0;
        } memory;
    public:
        struct Game
        {
            Grid::Position::Value playerColumn = 0;
            Grid::Position::Value playerRow = 0;

            std::optional<World::FieldID> fieldID;
        } game;
    public:
        struct Window
        {
            ScreenSize::Dimension windowWidth = 0;
            ScreenSize::Dimension windowHeight = 0;
            Atmos::Window::WindowBase::Size::Dimension clientWidth = 0;
            Atmos::Window::WindowBase::Size::Dimension clientHeight = 0;
            ScreenSize::Dimension startX = 0;
            ScreenSize::Dimension startY = 0;

            Position2D::Value viewOriginX = 0;
            Position2D::Value viewOriginY = 0;
        } window;
    public:
        using Profiler = Arca::RelicIndex<Time::RealStopwatch>;

        struct Profilers
        {
            Profiler input;
            Profiler logic;
            Profiler render;
            Profiler frameTime;
            Profiler idle;
            Profiler misc1;
            Profiler misc2;
            Profiler misc3;

            explicit Profilers(Arca::Reliquary& owner);
        private:
            static Profiler CreateProfiler(Arca::Reliquary& owner);
        } profilers;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Debug::Statistics>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static inline const TypeName typeName = "DebugStatistics";
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<>
    class Scribe<Atmos::Debug::Statistics, BinaryArchive> final
        : public ArcaCompositeScribe<Atmos::Debug::Statistics, BinaryArchive>
    {
    protected:
        void ScrivenImplementation(ObjectT& object, ArchiveT& archive) override;
    };
}