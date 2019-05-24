
#include "OutputStasisScribe.h"

#include "Field.h"

#include "FileSystem.h"

namespace Atmos
{
    const char* const OutputStasisScribe::defaultExtension = "stasis";

    OutputStasisScribe::OutputStasisScribe(
        const FilePath& filePath,
        const FileName& worldFileName,
        ::Inscription::ContainerSize fieldCount,
        OpenMode openMode) :

        fileName(filePath), worldFileName(worldFileName),
        underlyingScribe(filePath, "ATMOS GAIA STASIS", CurrentVersion()),
        curSaver(fieldSaverList.end()), hasOutputHeader(false)
    {
        if (openMode == OpenMode::FORCE_EXTENSION)
            this->fileName.SetExtension(defaultExtension);

        fieldSaverList.resize(fieldCount, FieldSaver(underlyingScribe));
    }

    void OutputStasisScribe::Save(Field& save)
    {
        OutputHeader();

        // Save the field ID
        curSaver->fieldID = save.id;

        // Save the field itself while clearing out the tracking entries made
        underlyingScribe.StartTrackingSection();
        curSaver->SaveObject(save);
        underlyingScribe.StopTrackingSection(true);

        ++curSaver;
    }

    void OutputStasisScribe::Save(FieldID id, const ::Inscription::Buffer& buffer)
    {
        OutputHeader();

        // Save the next field
        curSaver->fieldID = id;
        curSaver->SaveBuffer(buffer);
        ++curSaver;
    }

    void OutputStasisScribe::OverwriteFieldCount(::Inscription::ContainerSize set)
    {
        fieldSaverList.resize(set, FieldSaver(underlyingScribe));
    }

    const FileName& OutputStasisScribe::GetFileName() const
    {
        return fileName;
    }

    ::Inscription::Version OutputStasisScribe::CurrentVersion()
    {
        return 1;
    }

    ::Inscription::BinaryScribe& OutputStasisScribe::UnderlyingScribe()
    {
        return underlyingScribe;
    }

    const ::Inscription::BinaryScribe& OutputStasisScribe::UnderlyingScribe() const
    {
        return underlyingScribe;
    }

    OutputStasisScribe::FieldSaver::FieldSaver(FocusedScribe& scribe) : SkipFileSaver(scribe), fieldID(0), size(0)
    {}

    void OutputStasisScribe::FieldSaver::OnBeforeObjectSave()
    {
        positionStart = static_cast<Size>(scribe.TellStream());
    }

    void OutputStasisScribe::FieldSaver::OnAfterObjectSave()
    {
        size = static_cast<Size>(scribe.TellStream()) - positionStart;
    }

    void OutputStasisScribe::FieldSaver::SavePlaceholderExtra()
    {
        scribe.WriteNumeric(fieldID);
        scribe.WriteNumeric(size);
    }

    void OutputStasisScribe::FieldSaver::SaveExtra()
    {
        scribe.WriteNumeric(fieldID);
        scribe.WriteNumeric(size);
    }

    void OutputStasisScribe::OutputHeader()
    {
        if (!hasOutputHeader)
        {
            // Save base world name
            underlyingScribe.Save(worldFileName);

            // Save WorldStart
            underlyingScribe.Save(worldStart);

            // Save field placeholders
            ::Inscription::ContainerSize newCount(fieldSaverList.size());
            underlyingScribe.Save(newCount);

            for (auto& loop : fieldSaverList)
                loop.SavePlaceholder();
            curSaver = fieldSaverList.begin();

            hasOutputHeader = true;
        }
    }
}