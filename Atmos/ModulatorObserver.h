#pragma once

#include <memory>

namespace Atmos
{
    namespace Modulator
    {
        class ModulatorBase;

        class Observer
        {
        private:
            std::shared_ptr<ModulatorBase> ptr;
        public:
            Observer() = default;
            Observer(ModulatorBase *ptr);
            Observer(const Observer &arg) = default;
            Observer(Observer &&arg);
            Observer& operator=(const Observer &arg) = default;
            Observer& operator=(Observer &&arg);
            bool operator==(const Observer &arg) const;
            bool operator!=(const Observer &arg) const;

            ModulatorBase& operator*();
            const ModulatorBase& operator*() const;
            ModulatorBase* operator->();
            const ModulatorBase* operator->() const;
            ModulatorBase* Get();
            const ModulatorBase* Get() const;

            explicit operator bool() const;
            bool IsValid() const;

            void Reset(ModulatorBase *set = nullptr);
        };
    }
}