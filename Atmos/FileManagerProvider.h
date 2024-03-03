#pragma once

#include "UniqueProvider.h"
#include "FileManager.h"

namespace Atmos::File
{
    class ManagerProvider final : public UniqueProvider<FileManager>
    {
    public:
        ManagerProvider();
    protected:
        void OnSetup() override;
    private:
        void CreateDefaultDirectories();
    };

    extern ManagerProvider manager;
}