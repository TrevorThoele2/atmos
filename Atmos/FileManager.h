#pragma once

#include <memory>

#include "FilePath.h"
#include "String.h"

#include "Serialization.h"

namespace Atmos::File
{
    class FileManager
    {
    public:
        virtual ~FileManager() = 0;

        virtual void MakeDirectory(const Path& path) = 0;
        virtual bool RelocateFile(const Path& from, const Path& to) = 0;
        virtual bool RemoveFile(const Path& remove) = 0;

        virtual Path ExePath() const = 0;
        virtual Path DataPath() const = 0;
        virtual Path TemporaryDirectoryPath() const = 0;
        virtual size_t MaxPathLength() const = 0;
        virtual String FileSeparator() const = 0;
    };
}

namespace Inscription
{
    template<>
    class Scribe<::Atmos::File::FileManager, BinaryArchive> :
        public TableScribe<::Atmos::File::FileManager, BinaryArchive>
    {
    public:
        class Table : public TableBase
        {
        protected:
            void ConstructImplementation(ObjectT* storage, ArchiveT& archive) override;
        };
    };
}