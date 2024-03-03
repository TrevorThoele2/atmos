#pragma once

#include <vector>
#include "FalconScriptBasePrototype.h"
#include "FalconScriptUtility.h"

namespace Atmos
{
    namespace Fal
    {
        namespace Functions
        {
            class Prototype
            {
            public:
                typedef BasePrototype::ParameterBase ParameterBase;
                typedef BasePrototype::ParameterPtr ParameterPtr;
                template<class T>
                using Parameter = BasePrototype::Parameter<T>;

                template<class T>
                using List = BasePrototype::List<T>;

                typedef Falcon::ext_func_t FalconFuncT;
            private:
                FalconFuncT falconFunc;
                List<ParameterPtr> parameters;
            public:
                const Name functionName;

                Prototype(const Name &functionName, FalconFuncT falconFunc);
                Prototype(const Name &functionName, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc);
                // Returns true if it found everything
                // Generally indicates if a method can continue
                bool Setup(Falcon::VMachine &vm);

                const Name& GetFunctionName();
                template<class T>
                Parameter<T>* GetParameter(const Name &name);
                template<class T>
                Parameter<T>* GetParameter(Parameter<T> &prop);
                List<ParameterPtr>& GetParameters();

                void PushAllToModule(Falcon::Module &pushTo);
            };

            template<class T>
            typename Prototype::Parameter<T>* Prototype::GetParameter(const Name &name)
            {
                auto got = GetParameters().Get(name);
                if (!got)
                    return nullptr;

                return static_cast<Parameter<T>*>(got->get());
            }

            template<class T>
            typename Prototype::Parameter<T>* Prototype::GetParameter(Parameter<T> &prop)
            {
                return GetParameter<T>(prop.name);
            }
        }
    }
}