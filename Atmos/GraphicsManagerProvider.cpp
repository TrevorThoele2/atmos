#include "GraphicsManagerProvider.h"

#include "NullGraphicsManager.h"

namespace Atmos::Render
{
    void GraphicsManagerProvider::PostConstruct()
    {
        value = std::make_unique<NullGraphicsManager>(Owner());
    }

    void GraphicsManagerProvider::OnChange()
    {
        if (value)
            value->Initialize();
    }
}

namespace Arca
{
    const TypeName Traits<Atmos::Render::GraphicsManagerProvider>::typeName = "GraphicsManagerProvider";
}