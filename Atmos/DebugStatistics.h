#pragma once

#include <optional>

#include <Arca/ClosedTypedRelic.h>

#include "RealStopwatch.h"
#include "Point2D.h"
#include "GridPoint.h"
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
            Spatial::Grid::Point::Value playerColumn = 0;
            Spatial::Grid::Point::Value playerRow = 0;

            std::optional<World::FieldID> fieldID;
        } game;
    public:
        struct Window
        {
            Spatial::ScreenSize::Dimension windowWidth = 0;
            Spatial::ScreenSize::Dimension windowHeight = 0;
            Atmos::Window::WindowBase::Size::Dimension clientWidth = 0;
            Atmos::Window::WindowBase::Size::Dimension clientHeight = 0;
            Spatial::ScreenSize::Dimension startX = 0;
            Spatial::ScreenSize::Dimension startY = 0;

            Spatial::Point2D::Value cameraCenterX = 0;
            Spatial::Point2D::Value cameraCenterY = 0;
        } window;
    public:
        using Profiler = Arca::Index<Time::RealStopwatch>;

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
        static inline const TypeName typeName = "Atmos::Debug::Statistics";
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