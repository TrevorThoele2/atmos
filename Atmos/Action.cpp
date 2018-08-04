
#include "Action.h"

#include "ActionException.h"

#include <Inscription\Scribe.h>

namespace Atmos
{
    namespace Act
    {
        INSCRIPTION_SERIALIZE_FUNCTION_DEFINE(Action)
        {
            if (scribe.IsOutput())
            {
                scribe.WriteNumeric(static_cast<bool>(base));
                if (base)
                {
                    scribe.WriteNumeric(GetID());
                    base->Save(scribe);
                }
            }
            else
            {
                bool hadBase = scribe.ReadNumeric<bool>();
                if (!hadBase)
                    return;

                ID id = scribe.ReadNumeric<ID>();
                base.reset(Factories().find(id)->second->Create(scribe));
            }
        }

        Action::FactoryMap& Action::Factories()
        {
            static FactoryMap factories;
            return factories;
        }

        void Action::RequireParameterTypesEqual(ParameterIndex index, const Parameter &parameter)
        {
            if (GetParameterType(index) != Act::GetParameterType(parameter))
                throw ActionException("The given parameter must be the same type as what it's going to set.");
        }

        Action::Action(ID id)
        {
            Set(id);
        }

        Action::Action(const Action &arg) : base((arg.base) ? arg.base->Clone() : nullptr)
        {}

        Action& Action::operator=(const Action &arg)
        {
            base.reset((arg.base) ? arg.base->Clone() : nullptr);
            return *this;
        }

        Action::Action(Action &&arg) : base(std::move(arg.base))
        {}

        Action& Action::operator=(Action &&arg)
        {
            base = std::move(arg.base);
            return *this;
        }

        bool Action::operator==(const Action &arg) const
        {
            return base == arg.base;
        }

        bool Action::operator!=(const Action &arg) const
        {
            return !(*this == arg);
        }

        bool Action::Set(ID id)
        {
            auto found = Factories().find(id);
            if (found != Factories().end())
            {
                base.reset(found->second->Create());
                return true;
            }

            return false;
        }

        void Action::SetParameter(ParameterIndex index)
        {
            if (base)
                base->SetParameter(index);
        }

        void Action::SetParameter(ParameterIndex index, const Parameter &parameter)
        {
            if (base)
            {
                RequireParameterTypesEqual(index, parameter);

                base->SetParameter(index, parameter);
            }
        }

        void Action::SetParameter(ParameterIndex index, Parameter &&parameter)
        {
            if (base)
            {
                RequireParameterTypesEqual(index, parameter);

                base->SetParameter(index, std::move(parameter));
            }
        }

        void Action::SetParameters(std::vector<Parameter> &&parameters)
        {
            if (!base)
                return;

            base->SetParameters(std::move(parameters));
        }

        Optional<Parameter> Action::GetParameter(ParameterIndex index) const
        {
            typedef Optional<Parameter> ReturnT;
            if (!base)
                return ReturnT();
            else
                return base->GetParameter(index);
        }

        ParameterType Action::GetParameterType(ParameterIndex index) const
        {
            if (!base)
                return ParameterType::NONE;
            else
                return base->GetParameterType(index);
        }

        Action::ParameterIndex Action::GetParameterCount() const
        {
            if (!base)
                return 0;
            else
                return base->GetParameterCount();
        }

        ID Action::GetID() const
        {
            if (!base)
                return nullAction;
            else
                return base->GetID();
        }

        void Action::Execute()
        {
            base->Execute();
        }
    }
}