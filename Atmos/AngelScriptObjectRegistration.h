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
            DerivedT& Compare(GenericFunction function);

            DerivedT& Add(GenericFunction function);
            DerivedT& Add(GenericFunction function, String parameter);
            DerivedT& Subtract(GenericFunction function);
            DerivedT& Subtract(GenericFunction function, String parameter);
            DerivedT& Multiply(GenericFunction function);
            DerivedT& Multiply(GenericFunction function, String parameter);
            DerivedT& Divide(GenericFunction function);
            DerivedT& Divide(GenericFunction function, String parameter);
            DerivedT& Modulo(GenericFunction function);
            DerivedT& Modulo(GenericFunction function, String parameter);
            DerivedT& CompoundAdd(GenericFunction function);
            DerivedT& CompoundAdd(GenericFunction function, String parameter);
            DerivedT& CompoundSubtract(GenericFunction function);
            DerivedT& CompoundSubtract(GenericFunction function, String parameter);
            DerivedT& CompoundMultiply(GenericFunction function);
            DerivedT& CompoundMultiply(GenericFunction function, String parameter);
            DerivedT& CompoundDivide(GenericFunction function);
            DerivedT& CompoundDivide(GenericFunction function, String parameter);
            DerivedT& CompoundModulo(GenericFunction function);
            DerivedT& CompoundModulo(GenericFunction function, String parameter);

            DerivedT& Negation(GenericFunction function);
            DerivedT& PrefixIncrement(GenericFunction function);
            DerivedT& PostfixIncrement(GenericFunction function);
            DerivedT& PrefixDecrement(GenericFunction function);
            DerivedT& PostfixDecrement(GenericFunction function);

            DerivedT& Method(GenericFunction function, String returnType, String name, std::vector<String> parameters);
            DerivedT& ConstMethod(GenericFunction function, String returnType, String name, std::vector<String> parameters);
            DerivedT& StaticMethod(GenericFunction function, String returnType, String name, std::vector<String> parameters);

            template<auto property>
            DerivedT& Property(String type, String name);
            DerivedT& SetProperty(GenericFunction function, String returnType, String name, std::vector<String> parameters);
            DerivedT& GetProperty(GenericFunction function, String returnType, String name, std::vector<String> parameters);

            void Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager);
        public:
            using RegistrationItem = std::function<void(asIScriptEngine&, String)>;
            void AddItem(RegistrationItem&& item);

            using StaticRegistrationItem = std::function<void(asIScriptEngine&)>;
            void AddStaticItem(StaticRegistrationItem&& item);
        protected:
            Object(
                std::optional<String> containingNamespace,
                String registrationName,
                String representationName,
                std::vector<String> hiddenConstructorParameters,
                asDWORD knownFlags,
                String documentation);
        private:
            std::optional<String> containingNamespace;
            String registrationName;
            String representationName;

            std::vector<RegistrationItem> items;
            std::vector<StaticRegistrationItem> staticItems;

            asDWORD knownFlags;

            bool hasConstructor = false;
            bool hasCopyConstructor = false;
            bool hasDestructor = false;
            bool hasAssignment = false;

            std::vector<String> hiddenConstructorParameters;

            String documentation;
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

                    const auto result = engine.RegisterObjectBehaviour(
                        representationName.c_str(),
                        asBEHAVE_CONSTRUCT,
                        declaration.c_str(),
                        asFUNCTION(function),
                        asCALL_GENERIC);
                    VerifyResult(
                        result,
                        {
                            { "Declaration", declaration }
                        });
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
                    const auto result = engine.RegisterObjectBehaviour(
                        representationName.c_str(),
                        asBEHAVE_DESTRUCT,
                        "void f()",
                        asFUNCTION(function),
                        asCALL_GENERIC);
                    VerifyResult(
                        result,
                        {
                            { "Name", representationName }
                        });
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
        auto Object<T, DerivedT>::Compare(GenericFunction function) -> DerivedT&
        {
            return ConstMethod(function, "bool", "opCmp", { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Add(GenericFunction function) -> DerivedT&
        {
            return Add(function, { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Add(GenericFunction function, String parameter) -> DerivedT&
        {
            return ConstMethod(function, representationName, "opAdd", { parameter });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Subtract(GenericFunction function) -> DerivedT&
        {
            return Subtract(function, "const " + representationName + " &in");
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Subtract(GenericFunction function, String parameter) -> DerivedT&
        {
            return ConstMethod(function, representationName, "opSub", { parameter });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Multiply(GenericFunction function) -> DerivedT&
        {
            return Multiply(function, { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Multiply(GenericFunction function, String parameter) -> DerivedT&
        {
            return ConstMethod(function, representationName, "opMul", { parameter });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Divide(GenericFunction function) -> DerivedT&
        {
            return Divide(function, { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Divide(GenericFunction function, String parameter) -> DerivedT&
        {
            return ConstMethod(function, representationName, "opDiv", { parameter });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Modulo(GenericFunction function) -> DerivedT&
        {
            return Modulo(function, { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Modulo(GenericFunction function, String parameter) -> DerivedT&
        {
            return ConstMethod(function, representationName, "opMod", { parameter });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CompoundAdd(GenericFunction function) -> DerivedT&
        {
            return CompoundAdd(function, { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CompoundAdd(GenericFunction function, String parameter) -> DerivedT&
        {
            return Method(function, representationName, "opAddAssign", { parameter });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CompoundSubtract(GenericFunction function) -> DerivedT&
        {
            return CompoundSubtract(function, { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CompoundSubtract(GenericFunction function, String parameter) -> DerivedT&
        {
            return Method(function, representationName, "opSubAssign", { parameter });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CompoundMultiply(GenericFunction function) -> DerivedT&
        {
            return CompoundMultiply(function, { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CompoundMultiply(GenericFunction function, String parameter) -> DerivedT&
        {
            return Method(function, representationName, "opMulAssign", { parameter });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CompoundDivide(GenericFunction function) -> DerivedT&
        {
            return CompoundDivide(function, { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CompoundDivide(GenericFunction function, String parameter) -> DerivedT&
        {
            return Method(function, representationName, "opDivAssign", { parameter });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CompoundModulo(GenericFunction function) -> DerivedT&
        {
            return CompoundModulo(function, { "const " + representationName + " &in" });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::CompoundModulo(GenericFunction function, String parameter) -> DerivedT&
        {
            return Method(function, representationName, "opModAssign", { parameter });
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::Negation(GenericFunction function) -> DerivedT&
        {
            return ConstMethod(function, representationName, "&opNeg", {});
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::PrefixIncrement(GenericFunction function) -> DerivedT&
        {
            return Method(function, representationName, "&opPreInc", {});
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::PostfixIncrement(GenericFunction function) -> DerivedT&
        {
            return Method(function, representationName, "&opPostInc", {});
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::PrefixDecrement(GenericFunction function) -> DerivedT&
        {
            return Method(function, representationName, "&opPreDec", {});
        }

        template<class T, class DerivedT>
        auto Object<T, DerivedT>::PostfixDecrement(GenericFunction function) -> DerivedT&
        {
            return Method(function, representationName, "&opPostDec", {});
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
        auto Object<T, DerivedT>::StaticMethod(
            GenericFunction function, String returnType, String name, std::vector<String> parameters)
            -> DerivedT&
        {
            const auto declaration = returnType + " " + name + "(" + Chroma::Join(", ", parameters.begin(), parameters.end()) + ")";

            AddStaticItem([function, declaration](asIScriptEngine& engine)
                {
                    const auto result = engine.RegisterGlobalFunction(
                        declaration.c_str(),
                        asFUNCTION(function),
                        asCALL_GENERIC);
                    VerifyResult(
                        result,
                        {
                            { "Declaration", declaration }
                        });
                });

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        template<auto property>
        auto Object<T, DerivedT>::Property(String type, String name) -> DerivedT&
        {
            AddItem([type, name](asIScriptEngine& engine, String representationName)
                {
                    const auto declaration = type + " " + name;

                    const auto result = engine.RegisterObjectProperty(
                        representationName.c_str(),
                        declaration.c_str(),
                        (char*)&((T*)nullptr->*property) - (char*)nullptr);
                    VerifyResult(
                        result,
                        {
                            { "Declaration", declaration }
                        });
                });

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        DerivedT& Object<T, DerivedT>::SetProperty(GenericFunction function, String returnType, String name, std::vector<String> parameters)
        {
            const auto declaration = returnType + " get_" + name + "(" + Chroma::Join(", ", parameters.begin(), parameters.end()) + ") property";

            GenerateMethod(function, declaration);

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        DerivedT& Object<T, DerivedT>::GetProperty(GenericFunction function, String returnType, String name, std::vector<String> parameters)
        {
            const auto declaration = returnType + " set_" + name + "(" + Chroma::Join(", ", parameters.begin(), parameters.end()) + ") property";

            GenerateMethod(function, declaration);

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        void Object<T, DerivedT>::Actualize(asIScriptEngine& engine, DocumentationManager& documentationManager)
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

            {
                const auto result = engine.SetDefaultNamespace(useNamespace.c_str());
                VerifyResult(
                    result,
                    {
                        { "Namespace", useNamespace }
                    });
            }

            {
                const auto result = engine.RegisterObjectType(
                    registrationName.c_str(),
                    sizeof(T),
                    flags);
                VerifyResult(
                    result,
                    {
                        { "Name", registrationName }
                    });
                if (result > 0)
                    documentationManager.DocumentObject(result, documentation);
            }

            for (auto& item : items)
                item(engine, representationName);

            UserData::RequiredFrom(engine)->registeredTypes.emplace(CreateName({ useNamespace }, registrationName));

            if (!staticItems.empty())
            {
                const auto staticNamespace = CreateName({ containingNamespace ? *containingNamespace : "" }, registrationName);

                const auto result = engine.SetDefaultNamespace(staticNamespace.c_str());
                VerifyResult(
                    result,
                    {
                        { "Namespace", staticNamespace }
                    });

                for (auto& item : staticItems)
                    item(engine);
            }
        }

        template<class T, class DerivedT>
        void Object<T, DerivedT>::AddItem(RegistrationItem&& item)
        {
            items.push_back(std::move(item));
        }

        template<class T, class DerivedT>
        void Object<T, DerivedT>::AddStaticItem(StaticRegistrationItem&& item)
        {
            staticItems.push_back(std::move(item));
        }

        template<class T, class DerivedT>
        Object<T, DerivedT>::Object(
            std::optional<String> containingNamespace,
            String registrationName,
            String representationName,
            std::vector<String> hiddenConstructorParameters,
            asDWORD knownFlags,
            String documentation)
            :
            containingNamespace(containingNamespace),
            registrationName(registrationName),
            representationName(representationName),
            knownFlags(knownFlags),
            hiddenConstructorParameters(hiddenConstructorParameters),
            documentation(documentation)
        {}

        template<class T, class DerivedT>
        void Object<T, DerivedT>::GenerateMethod(GenericFunction function, String declaration)
        {
            AddItem([function, declaration](asIScriptEngine& engine, String representationName)
                {
                    const auto result = engine.RegisterObjectMethod(
                        representationName.c_str(),
                        declaration.c_str(),
                        asFUNCTION(function),
                        asCALL_GENERIC);
                    VerifyResult(
                        result,
                        {
                            { "Declaration", declaration }
                        });
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
                CreateName({ Registration<To>::ContainingNamespace() }, Registration<To>::Name()),
                "opConv",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ValueType<T, DerivedT>::ConstExplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().ConstMethod<function>(
                function,
                CreateName({ Registration<To>::ContainingNamespace() }, Registration<To>::Name()),
                "opConv",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ValueType<T, DerivedT>::ImplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().Method(
                function,
                CreateName({ Registration<To>::ContainingNamespace() }, Registration<To>::Name()),
                "opImplConv",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ValueType<T, DerivedT>::ConstImplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().ConstMethod(
                function,
                CreateName({ Registration<To>::ContainingNamespace() }, Registration<To>::Name()),
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
                    const auto result = engine.RegisterObjectBehaviour(
                        representationName.c_str(),
                        asBEHAVE_TEMPLATE_CALLBACK,
                        "bool f(int &in, bool &out)",
                        asFUNCTION(function),
                        asCALL_GENERIC);
                    VerifyResult(
                        result,
                        {
                            { "Name", representationName }
                        });
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
                    const auto result = engine.RegisterObjectBehaviour(
                        representationName.c_str(),
                        asBEHAVE_ADDREF,
                        "void f()",
                        asFUNCTION(function),
                        asCALL_GENERIC);
                    VerifyResult(
                        result,
                        {
                            { "Name", representationName }
                        });
                });

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        auto ReferenceType<T, DerivedT>::Release(GenericFunction function) -> DerivedT&
        {
            Self().AddItem([function](asIScriptEngine& engine, String representationName)
                {
                    const auto result = engine.RegisterObjectBehaviour(
                        representationName.c_str(),
                        asBEHAVE_RELEASE,
                        "void f()",
                        asFUNCTION(function),
                        asCALL_GENERIC);
                    VerifyResult(
                        result,
                        {
                            { "Name", representationName }
                        });
                });

            return static_cast<DerivedT&>(*this);
        }

        template<class T, class DerivedT>
        template<class To>
        auto ReferenceType<T, DerivedT>::ExplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().Method(
                function,
                CreateName({ Registration<To>::ContainingNamespace() }, Registration<To>::Name()),
                "opCast",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ReferenceType<T, DerivedT>::ConstExplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().ConstMethod(
                function,
                CreateName({ Registration<To>::ContainingNamespace() }, Registration<To>::Name()),
                "opCast",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ReferenceType<T, DerivedT>::ImplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().Method(
                function,
                CreateName({ Registration<To>::ContainingNamespace() }, Registration<To>::Name()),
                "opImplCast",
                std::vector<String>{});
        }

        template<class T, class DerivedT>
        template<class To>
        auto ReferenceType<T, DerivedT>::ConstImplicitConversion(GenericFunction function) -> DerivedT&
        {
            return Self().ConstMethod(
                function,
                CreateName({ Registration<To>::ContainingNamespace() }, Registration<To>::Name()),
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
            flags,
            Registration<T>::Documentation())
    {}

    template<class T>
    ValueTypeRegistration<T>::ValueTypeRegistration(String containingNamespace, String name) :
        Functionality::Object<T, ValueTypeRegistration<T>>(
            containingNamespace,
            name,
            name,
            {},
            flags,
            Registration<T>::Documentation())
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
            flags,
            Registration<T>::Documentation())
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
            flags,
            Registration<T>::Documentation())
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
            flags,
            Registration<T>::Documentation())
    {}

    template<class T>
    ReferenceTypeRegistration<T>::ReferenceTypeRegistration(String containingNamespace, String name) :
        Functionality::Object<T, ReferenceTypeRegistration<T>>(
            containingNamespace,
            name,
            name,
            {},
            flags,
            Registration<T>::Documentation())
    {}
}