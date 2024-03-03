
#include "FalconScriptBasePrototype.h"

namespace Atmos
{
    namespace Fal
    {
        namespace BasePrototype
        {
            StrictType::StrictType(bool value) : value(value)
            {}
            
            StrictType::operator bool() const
            {
                return value;
            }

            ValueBase::ValueBase(const Name &name, const StrictType &strictType) : name(name), strictType(strictType), wasSet(false)
            {}

            void ValueBase::SetItem(Falcon::VMachine &vm)
            {
                auto found = RetrieveItemFromVM(name, &vm);
                if (!found)
                    return;

                SetItem(vm, *found);
            }

            bool ValueBase::WasSet() const
            {
                return wasSet;
            }

            PropertyBase::PropertyBase(const Name &name) : ValueBase(name, true)
            {}

            PropertyBase::PropertyBase(const Name &name, const StrictType &strictType) : ValueBase(name, strictType)
            {}

            ParameterBase::ParameterBase(const Name &name) : ValueBase(name, true)
            {}

            ParameterBase::ParameterBase(const Name &name, const StrictType &strictType) : ValueBase(name, strictType)
            {}
        }
    }
}