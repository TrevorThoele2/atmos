
#include "FalconScriptFunctionPrototype.h"

#include <falcon/engine.h>

namespace Atmos
{
    namespace Fal
    {
        namespace Functions
        {
            Prototype::ParameterBase::ParameterBase(const Name &name) : name(name)
            {}

            void Prototype::ParameterBase::SetItem(Falcon::VMachine &vm)
            {
                auto found = vm.findGlobalItem(Convert(name));
                if (!found)
                    return;

                SetItem(vm, *found);
            }

            Prototype::Prototype(const Name &functionName, FalconFuncT falconFunc) : functionName(functionName), falconFunc(falconFunc)
            {}

            Prototype::Prototype(const Name &functionName, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc) : functionName(functionName), parameters(std::move(parameters)), falconFunc(falconFunc)
            {}

            bool Prototype::Setup(Falcon::VMachine &vm)
            {
                for (auto &loop : parameters)
                    if (!loop->FromItem(*this, vm))
                        return false;

                return true;
            }

            const Name& Prototype::GetFunctionName()
            {
                return functionName;
            }

            typename Prototype::List<Prototype::ParameterBase*> Prototype::GetParameters()
            {
                return parameters;
            }

            void Prototype::PushAllToModule(Falcon::Module &pushTo)
            {
                Falcon::Symbol *func = pushTo.addExtFunc(*pushTo.addString(Convert(functionName)), falconFunc);
                for(auto &loop : parameters)
                   loop->PushToFalcon(func, pushTo);
            }
        }
    }
}