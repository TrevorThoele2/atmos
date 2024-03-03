
#include "OutputWorldScribe.h"

#include "Field.h"

#include "ObjectTypeNameSerializer.h"
#include "ObjectRegistration.h"

namespace Atmos
{
    const char* const OutputWorldScribe::worldExtension = "gaia";

    OutputWorldScribe::OutputWorldScribe(
        const FilePath& filePath,
        ::Inscription::ContainerSize fieldCount,
        ObjectManager& globalObjectManager,
        OpenMode openMode) :

        filePath(GetForcedFilePath(filePath, openMode)),
        underlyingScribe(GetForcedFilePath(filePath, openMode).GetValue(), "ATMOS GAIA", CurrentVersion()),
        globalObjectManager(&globalObjectManager),
        curSaver(fieldSavers.end()), hasOutputHeader(false)
    {
        fieldSavers.resize(fieldCount, FieldSaver(underlyingScribe));
    }

    void OutputWorldScribe::Save(Field& field)
    {
        OutputHeader();

        // Save the field ID
        curSaver->fieldID = field.id;

        // Save the field itself while clearing out the tracking entries made
        underlyingScribe.StartTrackingSection();
        curSaver->SaveObject(field);
        underlyingScribe.StopTrackingSection(true);

        ++curSaver;
    }

    void OutputWorldScribe::OverwriteFieldCount(::Inscription::ContainerSize set)
    {
        fieldSavers.resize(set, FieldSaver(underlyingScribe));
    }

    const FilePath& OutputWorldScribe::GetFilePath() const
    {
        return filePath;
    }

    ::Inscription::Version OutputWorldScribe::CurrentVersion()
    {
        return 1;
    }

    ::Inscription::BinaryScribe& OutputWorldScribe::UnderlyingScribe()
    {
        return underlyingScribe;
    }

    const ::Inscription::BinaryScribe& OutputWorldScribe::UnderlyingScribe() const
    {
        return underlyingScribe;
    }

    OutputWorldScribe::FieldSaver::FieldSaver(FocusedScribe& scribe) : SkipFileSaver(scribe)
    {}

    void OutputWorldScribe::FieldSaver::OnBeforeObjectSave()
    {
        startPosition = static_cast<Size>(scribe.TellStream());
    }

    void OutputWorldScribe::FieldSaver::OnAfterObjectSave()
    {
        size = static_cast<Size>(scribe.TellStream() - startPosition);
    }

    void OutputWorldScribe::FieldSaver::SavePlaceholderExtra()
    {
        scribe.WriteNumeric(fieldID);
        scribe.WriteNumeric(size);
    }

    void OutputWorldScribe::FieldSaver::SaveExtra()
    {
        scribe.WriteNumeric(fieldID);
        scribe.WriteNumeric(size);
    }

    void OutputWorldScribe::OutputHeader()
    {
        // Save field placeholders
        if (hasOutputHeader)
            return;

        SkipFileSaver<Underlying> registrySizeSaver(underlyingScribe);
        registrySizeSaver.SavePlaceholder();

        // Save object type names
        ObjectTypeGraph objectTypeGraph;
        ObjectRegistration objectRegistration;
        objectRegistration.PushTo(objectTypeGraph);

        ObjectTypeNameSerializer objectTypeNameSerializer;
        objectTypeNameSerializer.AddAll(objectTypeGraph.AllDescriptions());
        objectTypeNameSerializer.AddAll(objectTypeGraph.AllDescriptions());
        objectTypeNameSerializer.SaveAll(underlyingScribe);

        // Save global object manager
        underlyingScribe.Save(*globalObjectManager);

        registrySizeSaver.SaveNothing();

        // Save WorldStart
        underlyingScribe.Save(worldStart);

        ::Inscription::ContainerSize count(fieldSavers.size());
        underlyingScribe.Save(count);

        for (auto& loop : fieldSavers)
            loop.SavePlaceholder();
        curSaver = fieldSavers.begin();
        hasOutputHeader = true;
    }

    FilePath OutputWorldScribe::GetForcedFilePath(const FilePath& filePath, OpenMode openMode)
    {
        FilePath ret(filePath);
        if (openMode == OpenMode::FORCE_EXTENSION)
            ret.SetExtension(worldExtension);
        return ret;
    }
}