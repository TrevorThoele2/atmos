#include "OutputStasisArchive.h"

#include "Field.h"

#include "FileSystem.h"

#include <Inscription/FileNameScribe.h>

namespace Atmos
{
    const char* const OutputStasisArchive::defaultExtension = "stasis";

    OutputStasisArchive::OutputStasisArchive(
        const FilePath& filePath,
        const FileName& worldFileName,
        ::Inscription::ContainerSize fieldCount,
        OpenMode openMode) :

        fileName(filePath), worldFileName(worldFileName),
        underlyingArchive(filePath, "ATMOS GAIA STASIS", CurrentVersion()),
        curSaver(fieldSaverList.end()), hasOutputHeader(false)
    {
        if (openMode == OpenMode::FORCE_EXTENSION)
            this->fileName.SetExtension(defaultExtension);

        fieldSaverList.resize(fieldCount, FieldSaver(underlyingArchive));
    }

    void OutputStasisArchive::Save(Field& save)
    {
        OutputHeader();

        // Save the field ID
        curSaver->fieldID = save.id;

        // Save the field itself while clearing out the tracking entries made
        underlyingArchive.StartTrackingSection();
        curSaver->SaveObject(save);
        underlyingArchive.StopTrackingSection(true);

        ++curSaver;
    }

    void OutputStasisArchive::Save(FieldID id, const ::Inscription::Buffer& buffer)
    {
        OutputHeader();

        // Save the next field
        curSaver->fieldID = id;
        curSaver->SaveBuffer(buffer);
        ++curSaver;
    }

    void OutputStasisArchive::OverwriteFieldCount(::Inscription::ContainerSize set)
    {
        fieldSaverList.resize(set, FieldSaver(underlyingArchive));
    }

    const FileName& OutputStasisArchive::GetFileName() const
    {
        return fileName;
    }

    ::Inscription::Version OutputStasisArchive::CurrentVersion()
    {
        return 1;
    }

    ::Inscription::BinaryArchive& OutputStasisArchive::UnderlyingArchive()
    {
        return underlyingArchive;
    }

    const ::Inscription::BinaryArchive& OutputStasisArchive::UnderlyingArchive() const
    {
        return underlyingArchive;
    }

    OutputStasisArchive::FieldSaver::FieldSaver(FocusedArchive& archive) : SkipFileSaver(archive), fieldID(0), size(0)
    {}

    void OutputStasisArchive::FieldSaver::OnBeforeObjectSave()
    {
        positionStart = static_cast<Size>(archive.TellStream());
    }

    void OutputStasisArchive::FieldSaver::OnAfterObjectSave()
    {
        size = static_cast<Size>(archive.TellStream()) - positionStart;
    }

    void OutputStasisArchive::FieldSaver::SavePlaceholderExtra()
    {
        archive.Write(fieldID);
        archive.Write(size);
    }

    void OutputStasisArchive::FieldSaver::SaveExtra()
    {
        archive.Write(fieldID);
        archive.Write(size);
    }

    void OutputStasisArchive::OutputHeader()
    {
        if (!hasOutputHeader)
        {
            // Save base world name
            underlyingArchive(worldFileName);

            // Save WorldStart
            underlyingArchive(worldStart);

            // Save field placeholders
            ::Inscription::ContainerSize newCount(fieldSaverList.size());
            underlyingArchive(newCount);

            for (auto& loop : fieldSaverList)
                loop.SavePlaceholder();
            curSaver = fieldSaverList.begin();

            hasOutputHeader = true;
        }
    }
}