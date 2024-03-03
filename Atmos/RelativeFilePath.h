#pragma once

#include "String.h"

#include "Serialization.h"

namespace Atmos
{
    class FileName;
    class FilePath;

    class RelativeFilePath
    {
    public:
        static String fileSeparator;
    public:
        RelativeFilePath();
        RelativeFilePath(const String& moveDown);
        // Must be null-terminated
        RelativeFilePath(const char* moveDown);
        RelativeFilePath(const FilePath& from, const FilePath& to);
        RelativeFilePath(const RelativeFilePath& arg);
        RelativeFilePath(RelativeFilePath&& arg);
        RelativeFilePath& operator=(const RelativeFilePath& arg);
        RelativeFilePath& operator=(RelativeFilePath&& arg);

        bool operator==(const RelativeFilePath& arg) const;
        bool operator!=(const RelativeFilePath& arg) const;
        operator const char*();
        const char* c_str() const;

        RelativeFilePath& SetFileName(const FileName& name);
        RelativeFilePath& SetExtension(const String& replace);
        RelativeFilePath& RemoveFileName();
        RelativeFilePath& RemoveExtension();

        RelativeFilePath& Up(size_t number = 1);
        RelativeFilePath& Down(const String& down);
        size_t GetMoveUpCount() const;
        String GetMoveDown() const;

        bool HasDirectory(const String& directoryName);
        size_t GetDirectoryCount() const;
        String GetDirectoryName(size_t index) const;

        operator String() const;
        String GetValue() const;
        String GetExtension() const;
        String GetWithoutExtension() const;
        bool IsEmpty() const;
        FileName GetFileName() const;
    private:
        String moveDown;
        size_t moveUp;
        String extension;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}