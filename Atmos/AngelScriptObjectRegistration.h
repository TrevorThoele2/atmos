#pragma once

#include <angelscript.h>
#include "AngelScriptRegistration.h"
#include "AngelScriptResultVerification.h"
#include "AngelScriptUserData.h"
#include "AngelScriptName.h"

#include "String.h"

namespace Atmos::Scripting::Angel
{
    namespace Functionality
    {
        template<class T, class DerivedT>
        class Object
        {
        public:
            using GenericFunction = void(*)(asIScriptGeneric*);
        public:
            DerivedT& DefaultConstructor(GenericFunction function);
            DerivedT& Constructor(GenericFunction function, std::vector<String> parameters);
            DerivedT& CopyConstructor(GenericFunction function);
            DerivedT& Destructor(GenericFunction function);

            DerivedT& Assignment(GenericFunction function, String parameter);
            DerivedT& CopyAssignment(GenericFunction function);

            DerivedT& Equals(GenericFunction function);

            DerivedT& Add(GenericFunction function);
            DerivedT& Subtract(GenericFunction function);

            DerivedT& Method(GenericFunction function, String returnType, String name, std::vector<String> parameters);
            DerivedT& ConstMethod(GenericFunction function, String returnType, String name, std::vector<String> parameters);
            template<auto property>
            DerivedT& Property(String type, String name);

            void Actualize(asIScriptEngine& engine);
        public:
            using RegistrationItem = std::function<void(asIScriptEngine&, String)>;
            void AddItem(RegistrationItem&& item);
        protected:
            Object(
                std::optional<String> containingNamespace,
                String registrationName,
                String representationName,
                std::vector<String> hiddenConstructorParameters,
                asDWORD knownFlags);
        private:
            std::optional<String> containingNamespace;
            String registrationName;
            String representationName;

            std::vector<RegistrationItem> items;

            asDWORD knownFlags;

            bool hasConstructor = false;
            bool hasCopyConstructor = false;
            bool hasDestructor = false;
            bool hasAssignment = false;

            std::vector<String> hiddenConstructorParameters;
        private:
            void GenerateMethod(GenericFunction function, String declaration);
        };

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::DefaultConstructor(GenericFunction function) -> DerivedT&
        {
            return Constructor(function, {});
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Constructor(GenericFunction function, std::vector<String> parameters) -> DerivedT&
        {
            AddItem([this, function, parameters](asIScriptEngine& engine, String representationName)
                {
                    auto useParameters = hiddenConstructorParameters;
                    useParameters.insert(useParameters.end(), parameters.begin(), parameters.end());
                    const auto declaration =
                        "void f(" + Chroma::Join(", ", useParameters.begin(), useParameters.end()) + ")";

                    VerifyResult(engine.RegisterObjectBehaviour(
                        representationName.c_str(),
                        asBEHAVE_CONSTRUCT,
                        declaration.c_str(),
                        asFUNCTION(function),
                        asCALL_GENERIC));
                });

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CopyConstructor(GenericFunction function) -> DerivedT&
        {
            return Constructor(function, { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Destructor(GenericFunction function) -> DerivedT&
        {
            AddItem([function](asIScriptEngine& engine, String representationName)
                {
                    VerifyResult(engine.RegisterObjectBehaviour(
                        representationName.c_str(),
                        asBEHAVE_DESTRUCT,
                        "void f()",
                        asFUNCTION(function),
                        asCALL_GENERIC));
                });

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Assignment(GenericFunction function, String parameter) -> DerivedT&
        {
            return Method(function, representationName, "&opAssign", { parameter });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CopyAssignment(GenericFunction function) -> DerivedT&
        {
            return Method(function, representationName, "&opAssign", { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Equals(GenericFunction function) -> DerivedT&
        {
            return ConstMethod(function, "bool", "opEquals", { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Add(GenericFunction function) -> DerivedT&
        {
            return ConstMethod(function, representationName, "opAdd", { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Subtract(GenericFunction function) -> DerivedT&
        {
            return ConstMethod(function, representationName, "opSub", { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Method(
            GenericFunction function, String returnType, String name, std::vector<String> parameters)
            -> DerivedT&
        {
            const auto declaration = returnType + " " + name + "(" + Chroma::Join(", ", parameters.begin(), parameters.end()) + ")";

            GenerateMethod(function, declaration);

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::ConstMethod(
            GenericFunction function, String returnType, String name, std::vector<String> parameters)
            -> DerivedT&
        {
            const auto declaration = returnType + " " + name + "(" + Chroma::Join(", ", parameters.begin(), parameters.end()) + ") const";

            GenerateMethod(function, declaration);

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        template<auto property>
        auto Object<T, DerivedT>::Property(String type, String name) -> DerivedT&
        {
            AddItem([type, name](asIScriptEngine& engine, String representationName)
                {
                    const auto declaration = type + " " + name;

                    VerifyResult(engine.RegisterObjectProperty(
                        representationName.c_str(),
                        declaration.c_str(),
                        (char*)&((T*)nullptr->*property) - (char*)nullptr));
                });

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        void Object<T, DerivedT>::Actualize(asIScriptEngine& engine)
        {
            auto flags = knownFlags;
            if (hasConstructor)
                flags |= asOBJ_APP_CLASS_CONSTRUCTOR;
            if (hasCopyConstructor)
                flags |= asOBJ_APP_CLASS_COPY_CONSTRUCTOR;
            if (hasDestructor)
                flags |= asOBJ_APP_CLASS_DESTRUCTOR;
            if (hasAssignment)
                flags |= asOBJ_APP_CLASS_ASSIGNMENT;

            const auto useNamespace = containingNamespace
                ? *containingNamespace
                : "";

            VerifyResult(engine.SetDefaultNamespace(useNamespace.c_str()));

            VerifyResult(engine.RegisterObjectType(
                registrationName.c_str(),
                sizeof(T),
                flags));

            for (auto& item : items)
                item(engine, representationName);

            UserData::RequiredFrom(engine)->registeredTypes.emplace(CreateName({ useNamespace }, registrationName));
        }

        template<class T, class DerivedT>
        void Object<T, DerivedT>::AddItem(RegistrationItem&& item)
        {
            items.push_back(std::move(item));
        }

        template<class T, class DerivedT>
        Object<T, DerivedT>::Object(
            std::optional<String> containingNamespace,
            String registrationName,
            String representationName,
            std::vector<String> hiddenConstructorParameters,
            asDWORD knownFlags)
            :
            containingNamespace(containingNamespace),
            registrationName(registrationName),
            representationName(representationName),
            knownFlags(knownFlags),
            hiddenConstructorParameters(hiddenConstructorParameters)
        {}

        template<class T, class DerivedT>
        void Object<T, DerivedT>::GenerateMethod(GenericFunction function, String declaration)
        {
            AddItem([function, declaration](asIScriptEngine& engine, String representationName)
                {
                    VerifyResult(engine.RegisterObjectMethod(
                        representationName.c_str(),
                        declaration.c_str(),
                        asFUNCTION(function),
                        asCALL_GENERIC));
                });
        }

        template<class T, class DerivedT>
        class ValueType
        {
        public:
            using GenericFunction = void(*)(asIScriptGeneric*);
        public:
            template<class To>
            DerivedT& ExplicitConversion(GenericFunction function);
            template<class To>
            DerivedT& ConstExplicitConversion(GenericFunction function);
            template<class To>
            DerivedT& ImplicitConversion(GenericFunction function);
            template<class To>
            DerivedT& ConstImplicitConversion(GenericFunction function);
        private:
            DerivedT& Self();
        };

        template<class T, class DerivedT>
        template<class To>
        auto ValueType<T, DerivedT>::ExplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().Method<function>(
                function,
                CreateName({ Registration<To>::containingNamespace }, Registration<To>::name),
                "opConv",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ValueType<T, DerivedT>::ConstExplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().ConstMethod<function>(
                function,
                CreateName({ Registration<To>::containingNamespace }, Registration<To>::name),
                "opConv",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ValueType<T, DerivedT>::ImplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().Method(
                function,
                CreateName({ Registration<To>::containingNamespace }, Registration<To>::name),
                "opImplConv",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ValueType<T, DerivedT>::ConstImplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().ConstMethod(
                function,
                CreateName({ Registration<To>::containingNamespace }, Registration<To>::name),
                "opImplConv",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        DerivedT& ValueType<T, DerivedT>::Self()
        {
            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        class TemplateValueType
        {
        public:
            using GenericFunction = void(*)(asIScriptGeneric*);
        public:
            DerivedT& TemplateCallback(GenericFunction function);
        private:
            DerivedT& Self();
        };

        template <class T, class DerivedT>
        auto TemplateValueType<T, DerivedT>::TemplateCallback(GenericFunction function) -> DerivedT&
        {
            Self().AddItem([function](asIScriptEngine& engine, String representationName)
                {
                    VerifyResult(engine.RegisterObjectBehaviour(
                        representationName.c_str(),
                        asBEHAVE_TEMPLATE_CALLBACK,
                        "bool f(int &in, bool &out)",
                        asFUNCTION(function),
                        asCALL_GENERIC));
                });

            return static_cast<DerivedT&>(*this);
        }

        template <class T, class DerivedT>
        DerivedT& TemplateValueType<T, DerivedT>::Self()
        {
            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        class ReferenceType
        {
        public:
            using GenericFunction = void(*)(asIScriptGeneric*);
        public:
            DerivedT& AddRef(GenericFunction function);
            DerivedT& Release(GenericFunction function);

            template<class To>
            DerivedT& ExplicitConversion(GenericFunction function);
            template<class To>
            DerivedT& ConstExplicitConversion(GenericFunction function);
            template<class To>
            DerivedT& ImplicitConversion(GenericFunction function);
            template<class To>
            DerivedT& ConstImplicitConversion(GenericFunction function);
        private:
            DerivedT& Self();
        };

        template<class T, class DerivedT>
        auto ReferenceType<T, DerivedT>::AddRef(GenericFunction function) -> DerivedT&
        {
            Self().AddItem([function](asIScriptEngine& engine, String representationName)
                {
                    VerifyResult(engine.RegisterObjectBehaviour(
                        representationName.c_str(),
                        asBEHAVE_ADDREF,
                        "void f()",
                        asFUNCTION(function),
                        asCALL_GENERIC));
                });

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        auto ReferenceType<T, DerivedT>::Release(GenericFunction function) -> DerivedT&
        {
            Self().AddItem([function](asIScriptEngine& engine, String representationName)
                {
                    VerifyResult(engine.RegisterObjectBehaviour(
                        representationName.c_str(),
                        asBEHAVE_RELEASE,
                        "void f()",
                        asFUNCTION(function),
                        asCALL_GENERIC));
                });

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        template<class To>
        auto ReferenceType<T, DerivedT>::ExplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().Method(
                function,
                CreateName({ Registration<To>::containingNamespace }, Registration<To>::name),
                "opCast",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ReferenceType<T, DerivedT>::ConstExplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().ConstMethod(
                function,
                CreateName({ Registration<To>::containingNamespace }, Registration<To>::name),
                "opCast",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ReferenceType<T, DerivedT>::ImplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().Method(
                function,
                CreateName({ Registration<To>::containingNamespace }, Registration<To>::name),
                "opImplCast",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ReferenceType<T, DerivedT>::ConstImplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().ConstMethod(
                function,
                CreateName({ Registration<To>::containingNamespace }, Registration<To>::name),
                "opImplCast",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        DerivedT& ReferenceType<T, DerivedT>::Self()
        {
            return static_cast<DerivedT&>(*this);
        }
    }

    template<class T>
    class ValueTypeRegistration final :
        public Functionality::Object<T, ValueTypeRegistration<T>>,
        public Functionality::ValueType<T, ValueTypeRegistration<T>>
    {
    public:
        ValueTypeRegistration(String name);
        ValueTypeRegistration(String containingNamespace, String name);
    private:
        static const asDWORD flags = asOBJ_VALUE;
    };

    template<class T>
    ValueTypeRegistration<T>::ValueTypeRegistration(String name) :
        Functionality::Object<T, ValueTypeRegistration<T>>(
            {},
            name,
            name,
            {},
            flags)
    {}

    template<class T>
    ValueTypeRegistration<T>::ValueTypeRegistration(String containingNamespace, String name) :
        Functionality::Object<T, ValueTypeRegistration<T>>(
            containingNamespace,
            name,
            name,
            {},
            flags)
    {}

    template<class T>
    class TemplateValueTypeRegistration final :
        public Functionality::Object<T, TemplateValueTypeRegistration<T>>,
        public Functionality::ValueType<T, TemplateValueTypeRegistration<T>>,
        public Functionality::TemplateValueType<T, TemplateValueTypeRegistration<T>>
    {
    public:
        TemplateValueTypeRegistration(
            String name, std::vector<String> templateNames);
        TemplateValueTypeRegistration(
            String containingNamespace, String name, std::vector<String> templateNames);
    private:
        static const asDWORD flags = asOBJ_VALUE | asOBJ_TEMPLATE;

        static String CreateRegistrationName(String name, std::vector<String> templateNames);
        static String CreateRepresentationName(String name, std::vector<String> templateNames);
    };

    template <class T>
    TemplateValueTypeRegistration<T>::TemplateValueTypeRegistration(
        String name, std::vector<String> templateNames)
        :
        Functionality::Object<T, TemplateValueTypeRegistration<T>>(
            {},
            CreateRegistrationName(name, templateNames),
            CreateRepresentationName(name, templateNames),
            { "int &in" },
            flags)
    {}

    template <class T>
    TemplateValueTypeRegistration<T>::TemplateValueTypeRegistration(
        String containingNamespace, String name, std::vector<String> templateNames)
        :
        Functionality::Object<T, TemplateValueTypeRegistration<T>>(
            containingNamespace,
            CreateRegistrationName(name, templateNames),
            CreateRepresentationName(name, templateNames),
            { "int &in" },
            flags)
    {}

    template <class T>
    String TemplateValueTypeRegistration<T>::CreateRegistrationName(String name, std::vector<String> templateNames)
    {
        std::vector<String> useTemplateNames;
        useTemplateNames.reserve(templateNames.size());
        for (auto& templateName : templateNames)
            useTemplateNames.push_back("class " + templateName);
        return name + "<" + Chroma::Join(", ", useTemplateNames.begin(), useTemplateNames.end()) + ">";
    }

    template <class T>
    String TemplateValueTypeRegistration<T>::CreateRepresentationName(String name, std::vector<String> templateNames)
    {
        return name + "<" + Chroma::Join(", ", templateNames.begin(), templateNames.end()) + ">";
    }

    template<class T>
    class ReferenceTypeRegistration final :
        public Functionality::Object<T, ReferenceTypeRegistration<T>>,
        public Functionality::ReferenceType<T, ReferenceTypeRegistration<T>>
    {
    private:
        using BaseT = Functionality::ValueType<T, ReferenceTypeRegistration<T>>;
    public:
        ReferenceTypeRegistration(String name);
        ReferenceTypeRegistration(String containingNamespace, String name);
    private:
        static const asDWORD flags = asOBJ_REF;
    };

    template<class T>
    ReferenceTypeRegistration<T>::ReferenceTypeRegistration(String name) :
        Functionality::Object<T, ReferenceTypeRegistration<T>>(
            {},
            name,
            name,
            {},
            flags)
    {}

    template<class T>
    ReferenceTypeRegistration<T>::ReferenceTypeRegistration(String containingNamespace, String name) :
        Functionality::Object<T, ReferenceTypeRegistration<T>>(
            containingNamespace,
            name,
            name,
            {},
            flags)
    {}
}