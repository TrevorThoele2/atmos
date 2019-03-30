
#include "Material.h"

namespace Atmos
{
    Material::Material(const Name& name) :
        nAsset(name), columns(0), rows(0),
        width([this]() { return _width; }), height([this]() { return _height; })
    {
        SubscribeToProperties();
    }

    Material::Material(const Material& arg) :
        nAsset(arg), columns(0), rows(0),
        width([this]() { return _width; }), height([this]() { return _height; })
    {
        SubscribeToProperties();
    }

    Material::Material(const ::Inscription::Table<Material>& table) :
        INSCRIPTION_TABLE_GET_BASE(nAsset),
        width([this]() { return _width; }), height([this]() { return _height; })
    {
        SubscribeToProperties();
    }

    ObjectTypeDescription Material::TypeDescription() const
    {
        return ObjectTraits<Material>::TypeDescription();
    }

    void Material::CalculateDimensions()
    {
        _width = 0;
        _height = 0;

        void (*maximizeFrom)(const ImageProperty&, Material&) = [](const ImageProperty& from, Material& self)
        {
            if (from->width > self.width)
                self._width = from->width;
            if (from->height > self.height)
                self._height = from->height;
        };

        typedef std::function<void(const ImageProperty&)> FunctionT;
        DoOnImageProperties(FunctionT(std::bind(maximizeFrom, std::placeholders::_1, std::ref(*this))));
    }

    void Material::DoOnImageProperties(std::function<void(ImageProperty&)>& func)
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

    void Material::DoOnImageProperties(std::function<void(const ImageProperty&)>& func) const
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

    void Material::SubscribeToProperties()
    {
        xVisual.onValueChanged.Subscribe(&Material::OnImagePropertyChanged, *this);
        xNormal.onValueChanged.Subscribe(&Material::OnImagePropertyChanged, *this);
        xHeight.onValueChanged.Subscribe(&Material::OnImagePropertyChanged, *this);
        yVisual.onValueChanged.Subscribe(&Material::OnImagePropertyChanged, *this);
        yNormal.onValueChanged.Subscribe(&Material::OnImagePropertyChanged, *this);
        yHeight.onValueChanged.Subscribe(&Material::OnImagePropertyChanged, *this);
        zVisual.onValueChanged.Subscribe(&Material::OnImagePropertyChanged, *this);
        zNormal.onValueChanged.Subscribe(&Material::OnImagePropertyChanged, *this);
        zHeight.onValueChanged.Subscribe(&Material::OnImagePropertyChanged, *this);
    }

    void Material::OnImagePropertyChanged(TypedObjectReference<ImageAsset> reference)
    {
        CalculateDimensions();
    }

    const ObjectTypeName ObjectTraits<Material>::typeName = "Material";
}

namespace Inscription
{
    DEFINE_OBJECT_INSCRIPTER_MEMBERS(::Atmos::Material)
    {

    }
}