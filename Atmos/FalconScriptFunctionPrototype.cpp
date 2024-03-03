
#include "FalconScriptFunctionPrototype.h"

#include <falcon/engine.h>

namespace Atmos
{
    namespace Fal
    {
        namespace Functions
        {
            Prototype::ParameterBase::ParameterBase(const Name &name, bool strictType) : name(name), strictType(strictType), wasSet(false)
            {}

            void Prototype::ParameterBase::SetItem(Falcon::VMachine &vm)
            {
                auto found = RetrieveItemFromVM(name, &vm);
                if (!found)
                    return;

                SetItem(vm, *found);
            }

            bool Prototype::ParameterBase::WasSet() const
            {
                return wasSet;
            }

            Prototype::Prototype(const Name &functionName, FalconFuncT falconFunc) : functionName(functionName), falconFunc(falconFunc)
            {}

            Prototype::Prototype(const Name &functionName, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc) : functionName(functionName), falconFunc(falconFunc)
            {
                for (auto &loop : parameters)
                    this->parameters.underlying.push_back(ParameterPtr(loop));
            }

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

            typename Prototype::List<Prototype::ParameterPtr>& Prototype::GetParameters()
            {
                return parameters;
            }

            void Prototype::PushAllToModule(Falcon::Module &pushTo)
            {
                Falcon::Symbol *func = pushTo.addExtFunc(Convert(functionName), falconFunc);
                auto def = func->getExtFuncDef();
                for(auto &loop : parameters)
                   loop->PushToFalcon(func, pushTo);
            }
        }
    }
}