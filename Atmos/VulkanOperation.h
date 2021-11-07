#pragma once

namespace Atmos::Render::Vulkan
{
	template<class Begin, class Execute, class End>
	void Operation(Begin begin, Execute execute, End end)
	{
        begin();

        try
        {
            execute();
        }
        catch (...)
        {
            end();
            throw;
        }

        end();
	}
}