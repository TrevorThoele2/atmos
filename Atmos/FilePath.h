#pragma once

#include "String.h"
#include "Serialization.h"

namespace Atmos
{
    extern const char *worldFolder;

    class FileName;
    class RelativeFilePath;

    class FilePath
    {
    public:
        FilePath() = default;
        FilePath(const String &string);
        // Must be null-terminated
        FilePath(const char *buf);
        FilePath(const FilePath &arg, const RelativeFilePath &relative);
        FilePath(const FilePath &arg);
        FilePath(FilePath &&arg);
        FilePath& operator=(const FilePath &arg);
        FilePath& operator=(FilePath &&arg);

        bool operator==(const FilePath &arg) const;
        bool operator!=(const FilePath &arg) const;
        operator const char*();
        const char* c_str() const;

        void Set(const String &set);
        void SetName(const FileName &name);
        void SetExtension(const String &replace);
        void RemoveFileName();
        void RemoveExtension();
        void RemoveDirectory(size_t index);

        FilePath& Append(const String &append);

        bool HasDirectory(const String &directoryName);
        size_t GetDirectoryCount() const;
        String GetDirectoryName(size_t index) const;

        operator String() const;
        String GetValue() const;
        String GetExtension() const;
        String GetWithoutExtension() const;
        bool IsEmpty() const;
        FileName GetFileName() const;
    private:
        String value;
        String extension;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };

    class FileName
    {
    public:
        FileName() = default;
        FileName(const String &string);
        // Must be null-terminated
        FileName(const char *buf);
        FileName(const FilePath &path);
        FileName(const FileName &arg);
        FileName(FileName &&arg);
        FileName& operator=(const FileName &arg);
        FileName& operator=(FileName &&arg);

        bool operator==(const FileName &arg) const;
        bool operator!=(const FileName &arg) const;
        operator const String&();
        operator const char*();
        const char* c_str() const;

        void Set(const String &set);
        void SetExtension(const String &replace);

        operator const String&() const;
        const String& GetValue() const;
        const String& GetExtension() const;
        String GetWithoutExtension() const;
        bool IsEmpty() const;
    private:
        String value;
        String extension;
    private:
        INSCRIPTION_SERIALIZE_FUNCTION_DECLARE;
        INSCRIPTION_ACCESS;
    };
}

namespace std
{
    template<>
    struct hash<Atmos::FilePath>
    {
        typedef Atmos::FilePath argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<::Atmos::String>()(arg.GetValue()));
            return first;
        }
    };

    template<>
    struct hash<Atmos::FileName>
    {
        typedef Atmos::FileName argument_type;
        typedef std::size_t result_type;

        result_type operator()(const argument_type &arg) const
        {
            const result_type first(std::hash<::Atmos::String>()(arg.GetValue()));
            return first;
        }
    };
}