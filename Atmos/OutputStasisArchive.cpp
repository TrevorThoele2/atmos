#include "OutputStasisArchive.h"

#include "Field.h"

#include "FileSystem.h"

#include <Inscription/FileNameScribe.h>

namespace Atmos::World::Serialization
{
    const char* const OutputStasisArchive::defaultExtension = "stasis";

    OutputStasisArchive::OutputStasisArchive(
        const File::Path& filePath,
        const File::Name& worldFileName,
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

        curSaver->fieldID = save.id;
        curSaver->SaveObject(save);

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

    const File::Name& OutputStasisArchive::GetFileName() const
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