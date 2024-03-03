#pragma once

#include <unordered_set>
#include <unordered_map>
#include <map>
#include <vector>

#include "FieldComponent.h"
#include "RenderFragment.h"
#include "RenderFragmentGrid.h"
#include "GridPosition.h"
#include "Position3D.h"
#include "Event.h"
#include "Optional.h"

namespace Atmos
{
    class RenderFragmentHandler : public FieldComponent
    {
    private:
        typedef RenderFragment Fragment;
        friend RenderFragment;
    private:
        bool dirty;

        std::unordered_set<RenderFragment*> totalFragments;
        RenderFragmentGrid grid;
        std::unordered_set<RenderFragment*> culled;

        Optional<Position3D::ValueT> zStopOverride;

        // Events
        EventScopedConnectionManager eventSlots;
        void OnCameraMoved();
        void OnAfterFieldSet() override;

        // Sprite functions
        void FragmentChanged(Fragment &fragment, const AxisBoundingBox3D &previousBounds);

        void Emplace(Fragment &fragment);
    public:
        RenderFragmentHandler(Field &field);
        RenderFragmentHandler(Field &field, RenderFragmentHandler &&arg);
        ~RenderFragmentHandler();
        RenderFragmentHandler& operator=(RenderFragmentHandler &&arg);

        void Add(Fragment &fragment);
        void Remove(Fragment &fragment);
        bool Has(const Fragment &fragment) const;

        void Render();
        void Cull();

        void Work();
        void Clear();

        // If this is not set, then this will use the Avatar's Z level
        void SetZOverride(Position3D::ValueT set);
        void SetZOverrideToMax();
        void ResetZOverride();
        const Optional<Position3D::ValueT>& GetZOverride() const;
        Position3D::ValueT GetZ() const;
    };
}