#pragma once

#include <Affecter/Engine.h>
#include "Position2D.h"
#include "Position3D.h"

namespace Affecter
{
	class MovementEngine2D : public ::Affecter::Engine<const ::Atmos::Position2D&>
	{
	private:
		::Atmos::Position2D starting, ending;
		void ExecuteImpl(const FunctionT &func, ::Affecter::DeltaT delta) override;
		bool IsActiveImpl() const override;
		void OnDone(const FunctionT &func) override;
	protected:
		MovementEngine2D(FunctionT &&function, const ::Atmos::Position2D &starting, const ::Atmos::Position2D &ending);
	};

	class MovementEngine3D : public ::Affecter::Engine<const ::Atmos::Position3D&>
	{
	private:
		::Atmos::Position3D starting, ending;
		void ExecuteImpl(const FunctionT &func, ::Affecter::DeltaT delta) override;
		bool IsActiveImpl() const override;
		void OnDone(const FunctionT &func) override;
	protected:
		MovementEngine3D(FunctionT &&function, const ::Atmos::Position3D &starting, const ::Atmos::Position3D &ending);
	};
}