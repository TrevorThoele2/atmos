#pragma once

#include <v8.h>
#include <optional>
#include "String.h"
#include "JavaScriptString.h"

namespace Atmos::Scripting::JavaScript
{
	template<class T>
	class Property
	{
	public:
		Property(const String& name);
		
		void SetValue(v8::Isolate& isolate, const T& value);
		void SetValue(const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object);

		[[nodiscard]] v8::Local<v8::Value> V8Value() const;
		[[nodiscard]] std::optional<T> AtmosValue() const;

		[[nodiscard]] String Name() const;
	private:
		String name;
		std::optional<T> atmosValue;
		v8::Local<v8::Value> v8Value;
	};

	template<class T>
	Property<T>::Property(const String& name) : name(name)
	{}

	template<class T>
	void Property<T>::SetValue(v8::Isolate& isolate, const T& value)
	{
		v8Value = ToV8(isolate, value);
	}

	template<class T>
	void Property<T>::SetValue(const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object)
	{
		const auto isolate = context->GetIsolate();
		atmosValue = FromV8<T>(*isolate, object->Get(context, ToV8(*isolate, name)));
	}

	template<class T>
	v8::Local<v8::Value> Property<T>::V8Value() const
	{
		return v8Value;
	}

	template<class T>
	std::optional<T> Property<T>::AtmosValue() const
	{
		return atmosValue;
	}
	
	template<class T>
	String Property<T>::Name() const
	{
		return name;
	}

	class ErasedProperty
	{
	public:
		template<class T>
		ErasedProperty(Property<T>& underlying);

		void SetValue(const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object);

		[[nodiscard]] v8::Local<v8::Value> V8Value() const;

		[[nodiscard]] String Name() const;
	private:
		class Base
		{
		public:
			virtual ~Base() = 0;

			virtual void SetValue(const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object) = 0;

			[[nodiscard]] virtual v8::Local<v8::Value> V8Value() const = 0;
		};

		template<class T>
		class Derived final : public Base
		{
		public:
			Derived(Property<T>& underlying);

			void SetValue(const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object) override;

			[[nodiscard]] v8::Local<v8::Value> V8Value() const override;
		private:
			Property<T>* underlying;
		};

		std::shared_ptr<Base> base;
	private:
		String name;
	};

	template<class T>
	ErasedProperty::ErasedProperty(Property<T>& underlying) : base(std::make_unique<Derived<T>>(underlying)), name(underlying.Name())
	{}

	template<class T>
	ErasedProperty::Derived<T>::Derived(Property<T>& underlying) : underlying(&underlying)
	{}

	template<class T>
	void ErasedProperty::Derived<T>::SetValue(const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object)
	{
		underlying->SetValue(context, object);
	}

	template<class T>
	v8::Local<v8::Value> ErasedProperty::Derived<T>::V8Value() const
	{
		return underlying->V8Value();
	}

	struct AnyObjectDefinition
	{
		std::vector<ErasedProperty> properties;

		void Extract(const v8::Local<v8::Context>& context, const v8::Local<v8::Object>& object);
	};

	struct AnyProperty
	{
		String name;
		v8::Local<v8::Value> value;
	};

	[[nodiscard]] v8::Local<v8::Object> CreateObject(v8::Isolate& isolate, const AnyObjectDefinition& definition);
	[[nodiscard]] v8::Local<v8::Object> CreateObject(v8::Isolate& isolate, const std::vector<AnyProperty>& properties);

	template<class T, class DefinitionT>
	[[nodiscard]] std::optional<T> FromObject(
		v8::Isolate& isolate, const v8::Local<v8::Value>& value, const std::function<std::optional<T>(const DefinitionT&)> extraction)
	{
		const auto context = isolate.GetCurrentContext();

		if (!value->IsObject())
			return std::optional<T>{};
		else
		{
			const auto object = value->ToObject(context).ToLocalChecked();
			auto definition = DefinitionT();
			definition.ToAny().Extract(context, object);
			return extraction(definition);
		}
	}
}