#pragma once

#include "String.h"
#include "StringUtility.h"

namespace Atmos::Initialization
{
    class FileEntryBase
    {
    public:
        virtual ~FileEntryBase() = 0;

        virtual void SetToFileString(const String& set) = 0;
        [[nodiscard]] virtual String FileString() const = 0;
    };

    template<class T>
    class FileEntry : public FileEntryBase
    {
    public:
        FileEntry(const String& name, T* value);

        void SetToFileString(const String& set) override;
        [[nodiscard]] String FileString() const override;
    private:
        String name;
        T* value;
    private:
        const String assignmentToken = "=";
    };

    template<class T>
    FileEntry<T>::FileEntry(const String& name, T* value) :
        name(name), value(value)
    {}

    template<class T>
    void FileEntry<T>::SetToFileString(const String& set)
    {
        // First, get past all spaces
        size_t count = 0;
        auto loop = set.begin();
        while (loop != set.end() && *loop == ' ')
            ++loop, ++count;

        // If we're already at the frameEndTime, or the name isn't right here (at the beginning), then leave
        if (loop == set.end() || set.find_first_of(name, count) != count)
            return;

        count += name.size();
        // Now, we need this token
        if (set.find_first_of(assignmentToken, count) != count)
            return;

        count += assignmentToken.size();
        // Now, we assume that the value is here (we can attempt to remove the last character if it's a newline)
        auto retrieveFrom = set.substr(count, String::npos);
        auto retrieveFromLoop = --retrieveFrom.end();
        while (*retrieveFromLoop == '\n')
            retrieveFromLoop = --retrieveFrom.erase(retrieveFromLoop);

        *value = FromString<T>(retrieveFrom);
    }

    template<class T>
    String FileEntry<T>::FileString() const
    {
        return name + assignmentToken + ToString(*value) + "\n";
    }
}