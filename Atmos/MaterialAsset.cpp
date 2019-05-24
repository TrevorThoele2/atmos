
#include "MaterialAsset.h"

namespace Atmos
{
    MaterialAsset::MaterialAsset(ObjectManager& manager, const Name& name) :
        Asset(manager, name), columns(0), rows(0),
        width([this]() { return _width; }), height([this]() { return _height; })
    {
        SubscribeToProperties();
    }

    MaterialAsset::MaterialAsset(const MaterialAsset& arg) :
        Asset(arg), columns(0), rows(0),
        width([this]() { return _width; }), height([this]() { return _height; })
    {
        SubscribeToProperties();
    }

    INSCRIPTION_BINARY_TABLE_CONSTRUCTOR_DEFINE(MaterialAsset) :
        INSCRIPTION_TABLE_GET_BASE(Asset),
        width([this]() { return _width; }), height([this]() { return _height; })
    {
        SubscribeToProperties();
    }

    ObjectTypeDescription MaterialAsset::TypeDescription() const
    {
        return ObjectTraits<MaterialAsset>::TypeDescription();
    }

    void MaterialAsset::CalculateDimensions()
    {
        _width = 0;
        _height = 0;

        void (*maximizeFrom)(const ImageProperty&, MaterialAsset&) = [](const ImageProperty& from, MaterialAsset& self)
        {
            if (from->width > self.width)
                self._width = from->width;
            if (from->height > self.height)
                self._height = from->height;
        };

        typedef std::function<void(const ImageProperty&)> FunctionT;
        DoOnImageProperties(FunctionT(std::bind(maximizeFrom, std::placeholders::_1, std::ref(*this))));
    }

    void MaterialAsset::DoOnImageProperties(std::function<void(ImageProperty&)>& func)
    {
        func(xVisual);
        func(xNormal);
        func(xHeight);
        func(yVisual);
        func(yNormal);
        func(yHeight);
        func(zVisual);
        func(zNormal);
        func(zHeight);
    }

    void MaterialAsset::DoOnImageProperties(std::function<void(const ImageProperty&)>& func) const
    {
        func(xVisual);
        func(xNormal);
        func(xHeight);
        func(yVisual);
        func(yNormal);
        func(yHeight);
        func(zVisual);
        func(zNormal);
        func(zHeight);
    }

    void MaterialAsset::SubscribeToProperties()
    {
        xVisual.onValueChanged.Subscribe(&MaterialAsset::OnImagePropertyChanged, *this);
        xNormal.onValueChanged.Subscribe(&MaterialAsset::OnImagePropertyChanged, *this);
        xHeight.onValueChanged.Subscribe(&MaterialAsset::OnImagePropertyChanged, *this);
        yVisual.onValueChanged.Subscribe(&MaterialAsset::OnImagePropertyChanged, *this);
        yNormal.onValueChanged.Subscribe(&MaterialAsset::OnImagePropertyChanged, *this);
        yHeight.onValueChanged.Subscribe(&MaterialAsset::OnImagePropertyChanged, *this);
        zVisual.onValueChanged.Subscribe(&MaterialAsset::OnImagePropertyChanged, *this);
        zNormal.onValueChanged.Subscribe(&MaterialAsset::OnImagePropertyChanged, *this);
        zHeight.onValueChanged.Subscribe(&MaterialAsset::OnImagePropertyChanged, *this);
    }

    void MaterialAsset::OnImagePropertyChanged(TypedObjectReference<ImageAsset> reference)
    {
        CalculateDimensions();
    }

    const ObjectTypeName ObjectTraits<MaterialAsset>::typeName = "Material";
}

namespace Inscription
{
    OBJECT_INSCRIPTER_DEFINE_MEMBERS(::Atmos::MaterialAsset)
    {

    }
}