#pragma once

namespace Atmos
{
    class Field;
    class FieldComponent
    {
    private:
        Field &field;

        FieldComponent(const FieldComponent &arg) = delete;
        FieldComponent& operator=(const FieldComponent &arg) = delete;

        // Called before the WorldManager starts changing the current field
        virtual void OnFinalizeField();
        // Called before the WorldManager sets this as the current field
        virtual void OnBeforeFieldSet(Field &newField);
        // Called after the WorldManager sets this as the current field
        virtual void OnAfterFieldSet();
        friend Field;
    public:
        FieldComponent(Field &field);
        Field& GetField();
    };
}