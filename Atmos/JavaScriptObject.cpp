#include "JavaScriptObject.h"

#include "JavaScriptSymbol.h"

namespace Atmos::Scripting::JavaScript
{
	void ErasedProperty::SetValue(const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object)
	{
		base->SetValue(context, object);
	}

	v8::Local<v8::Value> ErasedProperty::V8Value() const
	{
		return base->V8Value();
	}

	String ErasedProperty::Name() const
	{
		return name;
	}

	ErasedProperty::Base::~Base() = default;
	
	void AnyObjectDefinition::Extract(const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object)
	{
		for (auto& property : properties)
			property.SetValue(context, object);
	}

	v8::Local<v8::Object> CreateObject(v8::Isolate& isolate, const AnyObjectDefinition& definition)
	{
		std::vector<AnyProperty> properties;
		properties.reserve(definition.properties.size());
		for (auto& property : definition.properties)
			properties.emplace_back(property.Name(), property.V8Value());
		return CreateObject(isolate, properties);
	}

	v8::Local<v8::Object> CreateObject(v8::Isolate& isolate, const std::vector<AnyProperty>& properties)
	{
		const auto object = v8::Object::New(&isolate);
		for (auto& property : properties)
			if (object->Set(isolate.GetCurrentContext(), ToV8(isolate, property.name), property.value).IsNothing())
				throw Error(
					"Could not set property on JavaScript object.",
					{
						{"Name", property.name}
					});
		return object;
	}
}