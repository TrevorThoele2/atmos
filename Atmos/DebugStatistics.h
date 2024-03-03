#pragma once

#include <optional>

#include <Arca/Relic.h>

#include "Point2D.h"
#include "GridPoint.h"
#include "FieldID.h"
#include "WindowBase.h"
#include "Stopwatch.h"

namespace Atmos::Debug
{
    class Statistics final
    {
    public:
        Statistics() = default;
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
        struct Profilers
        {
            Time::Stopwatch input;
            Time::Stopwatch logic;
            Time::Stopwatch render;
            Time::Stopwatch frameTime;
            Time::Stopwatch idle;
            Time::Stopwatch misc1;
            Time::Stopwatch misc2;
            Time::Stopwatch misc3;

            explicit Profilers();
        private:
            static Time::Stopwatch CreateProfiler();
        } profilers;
    };
}

namespace Arca
{
    template<>
    struct Traits<::Atmos::Debug::Statistics>
    {
        static const ObjectType objectType = ObjectType::Relic;
        static TypeName TypeName() { return "Atmos::Debug::Statistics"; }
        static const Locality locality = Locality::Global;
    };
}

namespace Inscription
{
    template<class Archive>
    struct ScribeTraits<Atmos::Debug::Statistics, Archive> final
    {
        using Category = ArcaNullScribeCategory<Atmos::Debug::Statistics>;
    };
}