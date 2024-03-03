
#include "FieldComponent.h"

namespace Atmos
{
    void FieldComponent::OnFinalizeField()
    {}

    void FieldComponent::OnBeforeFieldSet(Field &newField)
    {}

    void FieldComponent::OnAfterFieldSet()
    {}

    FieldComponent::FieldComponent(Field &field) : field(field)
    {}

    Field& FieldComponent::GetField()
    {
        return field;
    }
}