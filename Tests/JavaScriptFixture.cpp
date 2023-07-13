#include "JavaScriptFixture.h"

JavaScriptFixture::JavaScriptFixture() : logger(Logging::Severity::Verbose), engine(logger)
{
    logger.AddSink<Logging::FileSink>();

    auto fieldOrigin = Arca::ReliquaryOrigin();
    RegisterFieldTypes(
        fieldOrigin,
        *engine.mockAssetResourceManager,
        *engine.mockAudioManager,
        *engine.mockInputManager,
        *engine.mockGraphicsManager,
        *engine.mockTextManager,
        *engine.scriptManager,
        *engine.mockWorldManager,
        Spatial::Size2D{
            std::numeric_limits<Spatial::Size2D::Value>::max(),
            std::numeric_limits<Spatial::Size2D::Value>::max() },
        *engine.mockWindow,
        engine.Logger());
    fieldOrigin.Register<Arca::OpenRelic>();
    RegisterFieldStages(fieldOrigin);
    engine.mockWorldManager->field = std::make_unique<World::Field>(0, fieldOrigin.Actualize());

    fieldReliquary = &engine.mockWorldManager->field->Reliquary();

    fieldReliquary->On<Scripting::Finished>([this](const Scripting::Finished& signal)
        {
            finishes.push_back(signal);
        });
}