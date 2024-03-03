
#include "ModulatorController.h"

#include "Modulator.h"

namespace Atmos
{
    namespace Modulator
    {
        bool Controller::IsIn(const Observer &check) const
        {
            for (auto &loop : observers)
                if (loop.second == check)
                    return true;

            return false;
        }

        Event<ModulatorBase*> Controller::onStarted;
        Event<ModulatorBase*> Controller::onStopped;

        Controller::ID Controller::Attach(const Observer &attach)
        {
            if (IsIn(attach))
                return observers.nullID;

            return observers.Add(attach);
        }

        void Controller::Detach(ID detach)
        {
            observers.Remove(detach);
        }

        void Controller::Detach(const Observer &detach)
        {
            for (auto loop = observers.begin(); loop != observers.end(); ++loop)
            {
                if (detach == loop->second)
                {
                    observers.Remove(loop);
                    return;
                }
            }
        }

        void Controller::Detach(const ModulatorBase &detach)
        {
            for (auto loop = observers.begin(); loop != observers.end(); ++loop)
            {
                if (&detach == loop->second.Get())
                {
                    observers.Remove(loop);
                    return;
                }
            }
        }

        Observer Controller::Find(ID find) const
        {
            auto found = observers.Find(find);
            if (found == observers.end())
                return Observer();

            return found->second;
        }

        void Controller::Work()
        {
            for (auto loop = observers.begin(); loop != observers.end();)
            {
                if (loop->second->Work())
                    loop = observers.Remove(loop);
                else 
                    ++loop;
            }
        }

        size_t Controller::GetWorkedSize() const
        {
            return observers.size();
        }

        void Controller::ForceStop(const Observer &stop)
        {
            for (auto loop = observers.begin(); loop != observers.end();)
            {
                if (loop->second == stop)
                {
                    loop->second->Stop();
                    observers.Remove(loop);
                    return;
                }
            }
        }

        bool Controller::IsModulatorWorkingObject(void *obj) const
        {
            for (auto &loop : observers)
            {
                if (loop.second->IsWorkingObject(obj))
                    return true;
            }

            return false;
        }
    }
}