#include <catch.hpp>

#include "RenderTests.h"

#include <Atmos/StaticMaterialView.h>

#include <Atmos/Map.h>

#include <Atmos/OutputWorldArchiveInterface.h>
#include <Atmos/WorldFileExtension.h>

#include <Atmos/NullWindow.h>
#include <Atmos/NullInputManager.h>
#include <Atmos/NullAudioManager.h>

auto RenderTestsFixture::DerivedEngine::CreateInitializationProperties(Arca::Reliquary& reliquary)
    -> InitializationProperties
{
    this->reliquary = &reliquary;

    using Properties = InitializationProperties;

    auto window = std::make_unique<MockWindow>();
    auto input = std::make_unique<Input::NullManager>();
    auto graphics = std::make_unique<MockGraphicsManager>(reliquary);
    auto audio = std::make_unique<Audio::NullAudioManager>();

    mockWindow = window.get();
    mockGraphicsManager = graphics.get();

    Properties properties;
    properties.window = std::move(window);
    properties.inputManager = std::move(input);
    properties.graphicsManager = std::move(graphics);
    properties.audioManager = std::move(audio);
    return properties;
}

void RenderTestsFixture::DerivedEngine::DoExit()
{
    
}

void RenderTestsFixture::MockWindow::Show()
{}

void RenderTestsFixture::MockWindow::Exit()
{}

bool RenderTestsFixture::MockWindow::IsCurrentlyFocused() const
{
    return true;
}

void RenderTestsFixture::MockWindow::Suspend(const Time::Value& time)
{}

bool RenderTestsFixture::MockWindow::OnStartFrame()
{
    static auto hasRan = false;
    if (hasRan)
        return false;
    hasRan = true;
    return true;
}

void RenderTestsFixture::MockWindow::SetupImpl()
{}

AxisAlignedBox2D RenderTestsFixture::MockWindow::AdjustWindowDimensions()
{
    return {};
}

void RenderTestsFixture::MockWindow::OnSetWindowDimensions()
{}

auto RenderTestsFixture::MockWindow::GetDefaultWindowPosition() -> Position
{
    return {};
}

void RenderTestsFixture::MockWindow::OnSetFullscreen()
{}

class ImageAssetDataImplementation final : public Asset::ImageAssetData
{
public:
    ImageAssetDataImplementation() = default;

    [[nodiscard]] std::unique_ptr<ImageAssetData> Clone() const override
    {
        return std::make_unique<ImageAssetDataImplementation>(*this);
    }
};

class ShaderAssetDataImplementation final : public Asset::ShaderAssetData
{
public:
    ShaderAssetDataImplementation() = default;

    [[nodiscard]] std::unique_ptr<ShaderAssetData> Clone() const override
    {
        return std::make_unique<ShaderAssetDataImplementation>(*this);
    }

    void Reset() override
    {}

    void Release() override
    {}

    PassCount Begin() const override
    {
        return 0;
    }

    void End() const override
    {}

    void BeginNextPass(PassCount pass) const override
    {}

    void EndPass() const override
    {}
};

class SurfaceData final : public Surface::Data
{
public:
    SurfaceData() = default;

    void SetAsRenderTarget() override
    {}

    void Present() override
    {}

    void Reset() override
    {}

    void Release() override
    {}

    ScreenDimensions Size() override
    {
        return { 0, 0 };
    }
};

class CanvasData final : public Canvas::Data
{
public:
    CanvasData() = default;

    void StartPainting() override
    {}

    void StopPainting() override
    {}

    void PaintPixel(
        const Canvas::Position& position,
        const Color& color,
        Canvas::DimensionValue height) override
    {}

    void Clear(const Color& color) override
    {}

    void Release() override
    {}

    void Reset(Canvas::DimensionValue width, Canvas::DimensionValue height) override
    {}
};

RenderTestsFixture::MockGraphicsManager::MockGraphicsManager(Arca::Reliquary& reliquary) :
    GraphicsManager(reliquary)
{}

void RenderTestsFixture::MockGraphicsManager::SetFullscreen(bool set)
{}

void RenderTestsFixture::MockGraphicsManager::ClearTarget(const Chroma::Flags<Target>& target)
{}

void RenderTestsFixture::MockGraphicsManager::ClearTarget(const Chroma::Flags<Target>& target, const Color& color)
{}

void RenderTestsFixture::MockGraphicsManager::Flush()
{}

void RenderTestsFixture::MockGraphicsManager::SetRenderState(RenderState state, bool set)
{}

bool RenderTestsFixture::MockGraphicsManager::Start()
{
    return false;
}

void RenderTestsFixture::MockGraphicsManager::End()
{}

void RenderTestsFixture::MockGraphicsManager::StartSprites(size_t spriteCount)
{}

void RenderTestsFixture::MockGraphicsManager::EndSprites()
{}

void RenderTestsFixture::MockGraphicsManager::StartLines()
{}

void RenderTestsFixture::MockGraphicsManager::EndLines()
{}

void RenderTestsFixture::MockGraphicsManager::StartStencil()
{}

void RenderTestsFixture::MockGraphicsManager::StopStencil()
{}

void RenderTestsFixture::MockGraphicsManager::ReconstructInternals()
{}

void RenderTestsFixture::MockGraphicsManager::SetMainDimensionsImpl(const ScreenDimensions& dimensions)
{}

ScreenDimensions RenderTestsFixture::MockGraphicsManager::MainDimensionsImpl() const
{
    return { 0, 0 };
}

std::unique_ptr<Asset::ImageAssetData> RenderTestsFixture::MockGraphicsManager::CreateImageDataImpl(
    const File::Path& path)
{
    return std::make_unique<ImageAssetDataImplementation>();
}

std::unique_ptr<Asset::ImageAssetData> RenderTestsFixture::MockGraphicsManager::CreateImageDataImpl(void* buffer,
    std::int32_t size, const File::Name& name)
{
    return std::make_unique<ImageAssetDataImplementation>();
}

std::unique_ptr<Asset::ShaderAssetData> RenderTestsFixture::MockGraphicsManager::CreateShaderDataImpl(
    const File::Path& path)
{
    return std::make_unique<ShaderAssetDataImplementation>();
}

std::unique_ptr<Asset::ShaderAssetData> RenderTestsFixture::MockGraphicsManager::CreateShaderDataImpl(void* buffer,
    std::int32_t size, const File::Name& name)
{
    return std::make_unique<ShaderAssetDataImplementation>();
}

Surface RenderTestsFixture::MockGraphicsManager::CreateSurfaceImpl(void* window)
{
    return Surface(std::make_unique<SurfaceData>());
}

Canvas RenderTestsFixture::MockGraphicsManager::CreateCanvasImpl(const ScreenDimensions& dimensions)
{
    return Canvas(std::make_unique<CanvasData>(), dimensions.width, dimensions.height);
}

bool RenderTestsFixture::MockGraphicsManager::CanMakeImageImpl(const File::Path& path) const
{
    return false;
}

bool RenderTestsFixture::MockGraphicsManager::CanMakeImageImpl(void* buffer, std::int32_t size) const
{
    return false;
}

void RenderTestsFixture::MockGraphicsManager::ResizeCanvasImpl(Canvas& canvas, const ScreenDimensions& dimensions)
{}

void RenderTestsFixture::MockGraphicsManager::SetRenderTargetImpl(Surface& set)
{}

void RenderTestsFixture::MockGraphicsManager::SetRenderTargetToMainImpl()
{}

void RenderTestsFixture::MockGraphicsManager::ReleaseMainRenderTarget()
{}

void RenderTestsFixture::MockGraphicsManager::ResetMainRenderTarget()
{}

void RenderTestsFixture::MockGraphicsManager::PresentImpl()
{}

void RenderTestsFixture::MockGraphicsManager::PresentImpl(void* windowOverride)
{}

void RenderTestsFixture::MockGraphicsManager::RenderMaterialViewImpl(MaterialRender& materialRender, float x, float y)
{
    materialViewRenders.emplace_back(materialRender, x, y);
}

void RenderTestsFixture::MockGraphicsManager::RenderCanvasViewImpl(CanvasRender& canvasRender, float x, float y)
{}

void RenderTestsFixture::MockGraphicsManager::RenderLineImpl(const Line& line)
{}

SCENARIO_METHOD(RenderTestsFixture, "rendering material views")
{
    GIVEN("created static material views inside map")
    {
        DerivedEngine engine;
        engine.Setup();

        std::vector<World::Field> savedFields;
        savedFields.emplace_back(0, *engine.reliquary);
        auto& field = savedFields.back();
        auto positions = dataGeneration.RandomStackGroup<Position3D, Position3D::Value, Position3D::Value, Position3D::Value>(3);

        auto materialView1 = field.reliquary->Create<StaticMaterialView>(positions[0]);
        auto materialView2 = field.reliquary->Create<StaticMaterialView>(positions[1]);
        auto materialView3 = field.reliquary->Create<StaticMaterialView>(positions[2]);

        const auto filePath = File::Path("Test" + World::Serialization::worldFileExtension);

        {
            World::Serialization::OutputWorldArchiveInterface outputWorld(filePath, savedFields);
        }

        WHEN("working engine")
        {
            engine.LoadWorld(filePath);

            engine.StartExecution();

            THEN("pushed all material views to graphics manager")
            {
                auto& materialViewRenders = engine.mockGraphicsManager->materialViewRenders;
                REQUIRE(materialViewRenders.size() == 3);
            }
        }
    }
}