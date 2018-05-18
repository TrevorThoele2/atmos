
#include "ModulatorObserver.h"
#include "Modulator.h"

namespace Atmos
{
    namespace Modulator
    {
        Observer::Observer(ModulatorBase *ptr) : ptr(ptr)
        {}

        Observer::Observer(Observer &&arg) : ptr(std::move(arg.ptr))
        {}

        Observer& Observer::operator=(Observer &&arg)
        {
            ptr = std::move(arg.ptr);
            return *this;
        }

        bool Observer::operator==(const Observer &arg) const
        {
            return ptr == arg.ptr;
        }

        bool Observer::operator!=(const Observer &arg) const
        {
            return !(*this == arg);
        }

        ModulatorBase& Observer::operator*()
        {
            return *ptr;
        }

        const ModulatorBase& Observer::operator*() const
        {
            return *ptr;
        }

        ModulatorBase* Observer::operator->()
        {
            return Get();
        }

        const ModulatorBase* Observer::operator->() const
        {
            return Get();
        }

        ModulatorBase* Observer::Get()
        {
            return ptr.get();
        }

        const ModulatorBase* Observer::Get() const
        {
            return ptr.get();
        }

        Observer::operator bool() const
        {
            return IsValid();
        }

        bool Observer::IsValid() const
        {
            return static_cast<bool>(ptr);
        }

        void Observer::Reset(ModulatorBase *set)
        {
            ptr.reset(set);
        }
    }
}