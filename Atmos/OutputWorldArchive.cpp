#include "OutputWorldArchive.h"

#include "Field.h"

#include "ObjectTypeNameSerializer.h"

#include "TypeRegistration.h"
#include "EngineTypeRegistration.h"

namespace Atmos::World::Serialization
{
    const char* const OutputWorldArchive::worldExtension = "gaia";

    OutputWorldArchive::OutputWorldArchive(
        const File::Path& filePath,
        ::Inscription::ContainerSize fieldCount,
        ObjectManager& globalObjectManager,
        OpenMode openMode) :

        filePath(GetForcedFilePath(filePath, openMode)),
        underlyingArchive(GetForcedFilePath(filePath, openMode).GetValue(), "ATMOS GAIA", CurrentVersion()),
        globalObjectManager(&globalObjectManager),
        curSaver(fieldSavers.end()), hasOutputHeader(false)
    {
        fieldSavers.resize(fieldCount, FieldSaver(underlyingArchive));
    }

    void OutputWorldArchive::Save(Field& field)
    {
        OutputHeader();

        curSaver->fieldID = field.id;
        curSaver->SaveObject(field);

        ++curSaver;
    }

    void OutputWorldArchive::OverwriteFieldCount(::Inscription::ContainerSize set)
    {
        fieldSavers.resize(set, FieldSaver(underlyingArchive));
    }

    const File::Path& OutputWorldArchive::GetFilePath() const
    {
        return filePath;
    }

    ::Inscription::Version OutputWorldArchive::CurrentVersion()
    {
        return 1;
    }

    ::Inscription::BinaryArchive& OutputWorldArchive::UnderlyingArchive()
    {
        return underlyingArchive;
    }

    const ::Inscription::BinaryArchive& OutputWorldArchive::UnderlyingArchive() const
    {
        return underlyingArchive;
    }

    OutputWorldArchive::FieldSaver::FieldSaver(FocusedArchive& archive) : SkipFileSaver(archive)
    {}

    void OutputWorldArchive::FieldSaver::OnBeforeObjectSave()
    {
        startPosition = static_cast<Size>(archive.TellStream());
    }

    void OutputWorldArchive::FieldSaver::OnAfterObjectSave()
    {
        size = static_cast<Size>(archive.TellStream() - startPosition);
    }

    void OutputWorldArchive::FieldSaver::SavePlaceholderExtra()
    {
        archive(fieldID);
        archive(size);
    }

    void OutputWorldArchive::FieldSaver::SaveExtra()
    {
        archive(fieldID);
        archive(size);
    }

    void OutputWorldArchive::OutputHeader()
    {
        // Save field placeholders
        if (hasOutputHeader)
            return;

        SkipFileSaver<Underlying> registrySizeSaver(underlyingArchive);
        registrySizeSaver.SavePlaceholder();

        // Save object type names
        ObjectTypeGraph objectTypeGraph;
        {
            TypeRegistration typeRegistration;
            auto group = typeRegistration.CreateGroup();
            RegisterLocalTypes(*group);
            RegisterGlobalTypes(*group);
            RegisterInfrastructureTypes(*group);
            group->PushTo(objectTypeGraph);
        }

        ObjectTypeNameSerializer objectTypeNameSerializer;
        objectTypeNameSerializer.AddAll(objectTypeGraph.AllDescriptions());
        objectTypeNameSerializer.AddAll(objectTypeGraph.AllDescriptions());
        objectTypeNameSerializer.SaveAll(underlyingArchive);

        // Save global object manager
        underlyingArchive(*globalObjectManager);

        registrySizeSaver.SaveNothing();

        // Save WorldStart
        underlyingArchive(worldStart);

        ::Inscription::ContainerSize count(fieldSavers.size());
        underlyingArchive(count);

        for (auto& loop : fieldSavers)
            loop.SavePlaceholder();
        curSaver = fieldSavers.begin();
        hasOutputHeader = true;
    }

    File::Path OutputWorldArchive::GetForcedFilePath(const File::Path& filePath, OpenMode openMode)
    {
        File::Path ret(filePath);
        if (openMode == OpenMode::FORCE_EXTENSION)
            ret.SetExtension(worldExtension);
        return ret;
    }
}