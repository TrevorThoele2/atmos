
#include "FalconScriptObjectPrototype.h"

#include <falcon/engine.h>

namespace Atmos
{
    namespace Fal
    {
        namespace Classes
        {
            Readonly::Readonly(bool value) : value(value)
            {}

            Readonly::operator bool() const
            {
                return value;
            }

            bool Prototype::MethodBase::ShouldSetupSelf() const
            {
                return true;
            }

            void Prototype::MethodBase::CopyParametersFrom(const MethodBase &arg)
            {
                for(auto &loop : arg.parameters)
                    parameters.underlying.push_back(ParameterPtr(static_cast<ParameterBase*>(loop->Clone())));
            }

            Prototype::MethodBase::MethodBase(Prototype &prototype, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc) : falconFunc(falconFunc), prototype(&prototype)
            {
                for (auto &loop : parameters)
                    this->parameters.underlying.push_back(ParameterPtr(loop));
            }

            Prototype::MethodBase::MethodBase(const MethodBase &arg) : falconFunc(arg.falconFunc), prototype(arg.prototype)
            {
                CopyParametersFrom(arg);
            }

            Prototype::MethodBase& Prototype::MethodBase::operator=(const MethodBase &arg)
            {
                CopyParametersFrom(arg);
                falconFunc = arg.falconFunc;
                prototype = arg.prototype;
                return *this;
            }

            bool Prototype::MethodBase::Setup(Falcon::VMachine &vm)
            {
                if (!SetupSelf(vm))
                    return false;

                if (!SetupParameters(vm))
                    return false;

                return true;
            }

            bool Prototype::MethodBase::SetupParameters(Falcon::VMachine &vm)
            {
                for (auto &loop : parameters)
                    if (!loop->FromItem(vm))
                        return false;

                return true;
            }

            bool Prototype::MethodBase::SetupSelf(Falcon::VMachine &vm)
            {
                if (!ShouldSetupSelf())
                    return true;

                for (auto &loop : prototype->GetProperties())
                    if (!loop->IsReadonly() && !loop->FromItem(vm))
                        return false;

                return true;
            }

            bool Prototype::Method::ShouldSetupSelf() const
            {
                return !isStatic;
            }

            Prototype::Method::Method(Prototype &prototype, const Name &name, bool isStatic, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc) : MethodBase(prototype, std::move(parameters), falconFunc), name(name), isStatic(isStatic)
            {}

            Prototype::Method* Prototype::Method::Clone(Prototype &prototype) const
            {
                auto ret = new Method(*this);
                ret->prototype = &prototype;
                return ret;
            }

            void Prototype::Method::PushToFalcon(Falcon::Symbol *cls, Falcon::Module &pushTo)
            {
                auto method = pushTo.addClassMethod(cls, *pushTo.addString(Convert(name)), falconFunc).asSymbol();
                for (auto &loop : parameters)
                    loop->PushToFalcon(method, pushTo);
            }

            Prototype::Constructor::Constructor(Prototype &prototype, std::vector<ParameterBase*> &&parameters, FalconFuncT falconFunc) : MethodBase(prototype, std::move(parameters), falconFunc)
            {}

            Prototype::Constructor* Prototype::Constructor::Clone(Prototype &prototype) const
            {
                auto ret = new Constructor(*this);
                ret->prototype = &prototype;
                return ret;
            }

            Falcon::Symbol* Prototype::Constructor::PushToFalcon(const Name &className, Falcon::Module &pushTo)
            {
                auto pushed = pushTo.addClass(*pushTo.addString(Convert(className)), falconFunc);
                for (auto &loop : parameters)
                    loop->PushToFalcon(pushed, pushTo);
                return pushed;
            }

            void Prototype::CopyFrom(const Prototype &arg)
            {
                properties.underlying.clear();
                methods.underlying.clear();
                for (auto &loop : arg.properties)
                    properties.underlying.push_back(PropertyPtr(static_cast<PropertyBase*>(loop->Clone())));
                for (auto &loop : arg.methods)
                    methods.underlying.push_back(MethodPtr(loop->Clone(*this)));
            }

            Prototype::Prototype(const Prototype &arg)
            {
                CopyFrom(arg);
            }

            Prototype& Prototype::operator=(const Prototype &arg)
            {
                CopyFrom(arg);
                return *this;
            }

            void Prototype::AddProperty(PropertyBase *add)
            {
                GetProperties().underlying.push_back(PropertyPtr(add));
            }

            void Prototype::AddMethod(Method *add)
            {
                GetMethods().underlying.push_back(MethodPtr(add));
            }

            Prototype::List<Prototype::PropertyPtr>& Prototype::GetProperties()
            {
                return properties;
            }

            Prototype::List<Prototype::MethodPtr>& Prototype::GetMethods()
            {
                return methods;
            }

            bool Prototype::Is(Falcon::Item &item)
            {
                return item.isObject() && item.asObject()->generator()->symbol()->name() == Convert(GetClsName());
            }

            bool Prototype::FromItem(Falcon::VMachine &vm, Falcon::Item &item)
            {
                if (!Is(item))
                    return false;

                for (auto &loop : properties)
                {
                    Falcon::Item propItem;
                    item.getProperty(Convert(loop->name), propItem);
                    if (!loop->FromItem(vm, propItem))
                        return false;
                }

                return true;
            }

            bool Prototype::FromItem(Falcon::VMachine &vm, const String &str)
            {
                auto found = RetrieveItemFromVM(str, &vm);
                if (!found)
                    return false;

                return FromItem(vm, *found);
            }

            Falcon::Item Prototype::CreateItem(Falcon::VMachine &vm)
            {
                auto self = Falcon::Item(vm.findGlobalItem(GetClsName().c_str())->asClass()->createInstance());
                for (auto &loop : GetProperties())
                    loop->SetProperty(vm, self);
                return self;
            }
        }
    }
}