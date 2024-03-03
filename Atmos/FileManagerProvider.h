#pragma once

#include <memory>
#include "FileManager.h"

namespace Atmos::File
{
    class ManagerProvider final
    {
    public:
        ManagerProvider();

        template<class T, class... Args>
        void Setup(Args&& ... args);

        FileManager& operator*();
        const FileManager& operator*() const;
        FileManager* operator->();
        const FileManager* operator->() const;
    private:
        void CreateDefaultDirectories();
        void SetupFilePaths();
    private:
        using Value = FileManager;
        using ValuePtr = std::unique_ptr<Value>;
        ValuePtr value;
    };

    template<class T, class... Args>
    void ManagerProvider::Setup(Args&& ... args)
    {
        value = std::make_unique<T>(std::forward<Args>(args)...);
        SetupFilePaths();
        CreateDefaultDirectories();
    }

    static ManagerProvider manager;
}