/*
    This file is part of the WebKit open source project.
    This file has been generated by generate-bindings.pl. DO NOT MODIFY!

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"
#include "JSTestLegacyFactoryFunction.h"

#include "ActiveDOMObject.h"
#include "Document.h"
#include "ExtendedDOMClientIsoSubspaces.h"
#include "ExtendedDOMIsoSubspaces.h"
#include "JSDOMBinding.h"
#include "JSDOMConstructorNotConstructable.h"
#include "JSDOMConvertInterface.h"
#include "JSDOMConvertStrings.h"
#include "JSDOMExceptionHandling.h"
#include "JSDOMGlobalObjectInlines.h"
#include "JSDOMLegacyFactoryFunction.h"
#include "JSDOMWrapperCache.h"
#include "ScriptExecutionContext.h"
#include "WebCoreJSClientData.h"
#include <JavaScriptCore/FunctionPrototype.h>
#include <JavaScriptCore/HeapAnalyzer.h>
#include <JavaScriptCore/JSCInlines.h>
#include <JavaScriptCore/JSDestructibleObjectHeapCellType.h>
#include <JavaScriptCore/SlotVisitorMacros.h>
#include <JavaScriptCore/SubspaceInlines.h>
#include <wtf/GetPtr.h>
#include <wtf/PointerPreparations.h>
#include <wtf/URL.h>


namespace WebCore {
using namespace JSC;

// Attributes

static JSC_DECLARE_CUSTOM_GETTER(jsTestLegacyFactoryFunctionConstructor);

class JSTestLegacyFactoryFunctionPrototype final : public JSC::JSNonFinalObject {
public:
    using Base = JSC::JSNonFinalObject;
    static JSTestLegacyFactoryFunctionPrototype* create(JSC::VM& vm, JSDOMGlobalObject* globalObject, JSC::Structure* structure)
    {
        JSTestLegacyFactoryFunctionPrototype* ptr = new (NotNull, JSC::allocateCell<JSTestLegacyFactoryFunctionPrototype>(vm)) JSTestLegacyFactoryFunctionPrototype(vm, globalObject, structure);
        ptr->finishCreation(vm);
        return ptr;
    }

    DECLARE_INFO;
    template<typename CellType, JSC::SubspaceAccess>
    static JSC::GCClient::IsoSubspace* subspaceFor(JSC::VM& vm)
    {
        STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestLegacyFactoryFunctionPrototype, Base);
        return &vm.plainObjectSpace();
    }
    static JSC::Structure* createStructure(JSC::VM& vm, JSC::JSGlobalObject* globalObject, JSC::JSValue prototype)
    {
        return JSC::Structure::create(vm, globalObject, prototype, JSC::TypeInfo(JSC::ObjectType, StructureFlags), info());
    }

private:
    JSTestLegacyFactoryFunctionPrototype(JSC::VM& vm, JSC::JSGlobalObject*, JSC::Structure* structure)
        : JSC::JSNonFinalObject(vm, structure)
    {
    }

    void finishCreation(JSC::VM&);
};
STATIC_ASSERT_ISO_SUBSPACE_SHARABLE(JSTestLegacyFactoryFunctionPrototype, JSTestLegacyFactoryFunctionPrototype::Base);

using JSTestLegacyFactoryFunctionDOMConstructor = JSDOMConstructorNotConstructable<JSTestLegacyFactoryFunction>;
using JSTestLegacyFactoryFunctionLegacyFactoryFunction = JSDOMLegacyFactoryFunction<JSTestLegacyFactoryFunction>;

template<> const ClassInfo JSTestLegacyFactoryFunctionDOMConstructor::s_info = { "TestLegacyFactoryFunction"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestLegacyFactoryFunctionDOMConstructor) };

template<> JSValue JSTestLegacyFactoryFunctionDOMConstructor::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    UNUSED_PARAM(vm);
    return globalObject.functionPrototype();
}

template<> void JSTestLegacyFactoryFunctionDOMConstructor::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->length, jsNumber(1), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    JSString* nameString = jsNontrivialString(vm, "TestLegacyFactoryFunction"_s);
    m_originalName.set(vm, this, nameString);
    putDirect(vm, vm.propertyNames->name, nameString, JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    putDirect(vm, vm.propertyNames->prototype, JSTestLegacyFactoryFunction::prototype(vm, globalObject), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::DontDelete);
}

template<> EncodedJSValue JSC_HOST_CALL_ATTRIBUTES JSTestLegacyFactoryFunctionLegacyFactoryFunction::construct(JSGlobalObject* lexicalGlobalObject, CallFrame* callFrame)
{
    VM& vm = lexicalGlobalObject->vm();
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto* castedThis = jsCast<JSTestLegacyFactoryFunctionLegacyFactoryFunction*>(callFrame->jsCallee());
    ASSERT(castedThis);
    if (UNLIKELY(callFrame->argumentCount() < 1))
        return throwVMError(lexicalGlobalObject, throwScope, createNotEnoughArgumentsError(lexicalGlobalObject));
    auto* context = castedThis->scriptExecutionContext();
    if (UNLIKELY(!context))
        return throwConstructorScriptExecutionContextUnavailableError(*lexicalGlobalObject, throwScope, "TestLegacyFactoryFunction");
    ASSERT(context->isDocument());
    auto& document = downcast<Document>(*context);
    EnsureStillAliveScope argument0 = callFrame->uncheckedArgument(0);
    auto str1 = convert<IDLDOMString>(*lexicalGlobalObject, argument0.value());
    RETURN_IF_EXCEPTION(throwScope, encodedJSValue());
    EnsureStillAliveScope argument1 = callFrame->argument(1);
    auto str2 = argument1.value().isUndefined() ? "defaultString"_s : convert<IDLDOMString>(*lexicalGlobalObject, argument1.value());
    RETURN_IF_EXCEPTION(throwScope, encodedJSValue());
    EnsureStillAliveScope argument2 = callFrame->argument(2);
    auto str3 = argument2.value().isUndefined() ? String() : convert<IDLDOMString>(*lexicalGlobalObject, argument2.value());
    RETURN_IF_EXCEPTION(throwScope, encodedJSValue());
    auto object = TestLegacyFactoryFunction::createForLegacyFactoryFunction(document, WTFMove(str1), WTFMove(str2), WTFMove(str3));
    if constexpr (IsExceptionOr<decltype(object)>)
        RETURN_IF_EXCEPTION(throwScope, { });
    static_assert(TypeOrExceptionOrUnderlyingType<decltype(object)>::isRef);
    auto jsValue = toJSNewlyCreated<IDLInterface<TestLegacyFactoryFunction>>(*lexicalGlobalObject, *castedThis->globalObject(), throwScope, WTFMove(object));
    if constexpr (IsExceptionOr<decltype(object)>)
        RETURN_IF_EXCEPTION(throwScope, { });
    setSubclassStructureIfNeeded<TestLegacyFactoryFunction>(lexicalGlobalObject, callFrame, asObject(jsValue));
    RETURN_IF_EXCEPTION(throwScope, { });
    return JSValue::encode(jsValue);
}
JSC_ANNOTATE_HOST_FUNCTION(JSTestLegacyFactoryFunctionLegacyFactoryFunctionConstruct, JSTestLegacyFactoryFunctionLegacyFactoryFunction::construct);

template<> const ClassInfo JSTestLegacyFactoryFunctionLegacyFactoryFunction::s_info = { "Audio"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestLegacyFactoryFunctionLegacyFactoryFunction) };

template<> JSValue JSTestLegacyFactoryFunctionLegacyFactoryFunction::prototypeForStructure(JSC::VM& vm, const JSDOMGlobalObject& globalObject)
{
    UNUSED_PARAM(vm);
    return globalObject.functionPrototype();
}

template<> void JSTestLegacyFactoryFunctionLegacyFactoryFunction::initializeProperties(VM& vm, JSDOMGlobalObject& globalObject)
{
    putDirect(vm, vm.propertyNames->length, jsNumber(1), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    JSString* nameString = jsNontrivialString(vm, "Audio"_s);
    m_originalName.set(vm, this, nameString);
    putDirect(vm, vm.propertyNames->name, nameString, JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum);
    putDirect(vm, vm.propertyNames->prototype, JSTestLegacyFactoryFunction::prototype(vm, globalObject), JSC::PropertyAttribute::ReadOnly | JSC::PropertyAttribute::DontEnum | JSC::PropertyAttribute::DontDelete);
}

/* Hash table for prototype */

static const HashTableValue JSTestLegacyFactoryFunctionPrototypeTableValues[] =
{
    { "constructor"_s, static_cast<unsigned>(JSC::PropertyAttribute::DontEnum), NoIntrinsic, { (intptr_t)static_cast<PropertySlot::GetValueFunc>(jsTestLegacyFactoryFunctionConstructor), (intptr_t) static_cast<PutPropertySlot::PutValueFunc>(0) } },
};

const ClassInfo JSTestLegacyFactoryFunctionPrototype::s_info = { "TestLegacyFactoryFunction"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestLegacyFactoryFunctionPrototype) };

void JSTestLegacyFactoryFunctionPrototype::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    reifyStaticProperties(vm, JSTestLegacyFactoryFunction::info(), JSTestLegacyFactoryFunctionPrototypeTableValues, *this);
    JSC_TO_STRING_TAG_WITHOUT_TRANSITION();
}

const ClassInfo JSTestLegacyFactoryFunction::s_info = { "TestLegacyFactoryFunction"_s, &Base::s_info, nullptr, nullptr, CREATE_METHOD_TABLE(JSTestLegacyFactoryFunction) };

JSTestLegacyFactoryFunction::JSTestLegacyFactoryFunction(Structure* structure, JSDOMGlobalObject& globalObject, Ref<TestLegacyFactoryFunction>&& impl)
    : JSDOMWrapper<TestLegacyFactoryFunction>(structure, globalObject, WTFMove(impl))
{
}

void JSTestLegacyFactoryFunction::finishCreation(VM& vm)
{
    Base::finishCreation(vm);
    ASSERT(inherits(info()));

    static_assert(std::is_base_of<ActiveDOMObject, TestLegacyFactoryFunction>::value, "Interface is marked as [ActiveDOMObject] but implementation class does not subclass ActiveDOMObject.");

}

JSObject* JSTestLegacyFactoryFunction::createPrototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return JSTestLegacyFactoryFunctionPrototype::create(vm, &globalObject, JSTestLegacyFactoryFunctionPrototype::createStructure(vm, &globalObject, globalObject.objectPrototype()));
}

JSObject* JSTestLegacyFactoryFunction::prototype(VM& vm, JSDOMGlobalObject& globalObject)
{
    return getDOMPrototype<JSTestLegacyFactoryFunction>(vm, globalObject);
}

JSValue JSTestLegacyFactoryFunction::getConstructor(VM& vm, const JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestLegacyFactoryFunctionDOMConstructor, DOMConstructorID::TestLegacyFactoryFunction>(vm, *jsCast<const JSDOMGlobalObject*>(globalObject));
}

JSValue JSTestLegacyFactoryFunction::getLegacyFactoryFunction(VM& vm, JSGlobalObject* globalObject)
{
    return getDOMConstructor<JSTestLegacyFactoryFunctionLegacyFactoryFunction, DOMConstructorID::TestLegacyFactoryFunctionLegacyFactory>(vm, *jsCast<JSDOMGlobalObject*>(globalObject));
}

void JSTestLegacyFactoryFunction::destroy(JSC::JSCell* cell)
{
    JSTestLegacyFactoryFunction* thisObject = static_cast<JSTestLegacyFactoryFunction*>(cell);
    thisObject->JSTestLegacyFactoryFunction::~JSTestLegacyFactoryFunction();
}

JSC_DEFINE_CUSTOM_GETTER(jsTestLegacyFactoryFunctionConstructor, (JSGlobalObject* lexicalGlobalObject, EncodedJSValue thisValue, PropertyName))
{
    VM& vm = JSC::getVM(lexicalGlobalObject);
    auto throwScope = DECLARE_THROW_SCOPE(vm);
    auto* prototype = jsDynamicCast<JSTestLegacyFactoryFunctionPrototype*>(JSValue::decode(thisValue));
    if (UNLIKELY(!prototype))
        return throwVMTypeError(lexicalGlobalObject, throwScope);
    return JSValue::encode(JSTestLegacyFactoryFunction::getConstructor(JSC::getVM(lexicalGlobalObject), prototype->globalObject()));
}

JSC::GCClient::IsoSubspace* JSTestLegacyFactoryFunction::subspaceForImpl(JSC::VM& vm)
{
    return WebCore::subspaceForImpl<JSTestLegacyFactoryFunction, UseCustomHeapCellType::No>(vm,
        [] (auto& spaces) { return spaces.m_clientSubspaceForTestLegacyFactoryFunction.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_clientSubspaceForTestLegacyFactoryFunction = WTFMove(space); },
        [] (auto& spaces) { return spaces.m_subspaceForTestLegacyFactoryFunction.get(); },
        [] (auto& spaces, auto&& space) { spaces.m_subspaceForTestLegacyFactoryFunction = WTFMove(space); }
    );
}

void JSTestLegacyFactoryFunction::analyzeHeap(JSCell* cell, HeapAnalyzer& analyzer)
{
    auto* thisObject = jsCast<JSTestLegacyFactoryFunction*>(cell);
    analyzer.setWrappedObjectForCell(cell, &thisObject->wrapped());
    if (thisObject->scriptExecutionContext())
        analyzer.setLabelForCell(cell, "url " + thisObject->scriptExecutionContext()->url().string());
    Base::analyzeHeap(cell, analyzer);
}

bool JSTestLegacyFactoryFunctionOwner::isReachableFromOpaqueRoots(JSC::Handle<JSC::Unknown> handle, void*, AbstractSlotVisitor& visitor, const char** reason)
{
    auto* jsTestLegacyFactoryFunction = jsCast<JSTestLegacyFactoryFunction*>(handle.slot()->asCell());
    auto& wrapped = jsTestLegacyFactoryFunction->wrapped();
    if (!wrapped.isContextStopped() && wrapped.hasPendingActivity()) {
        if (UNLIKELY(reason))
            *reason = "ActiveDOMObject with pending activity";
        return true;
     }
    UNUSED_PARAM(visitor);
    UNUSED_PARAM(reason);
    return false;
}

void JSTestLegacyFactoryFunctionOwner::finalize(JSC::Handle<JSC::Unknown> handle, void* context)
{
    auto* jsTestLegacyFactoryFunction = static_cast<JSTestLegacyFactoryFunction*>(handle.slot()->asCell());
    auto& world = *static_cast<DOMWrapperWorld*>(context);
    uncacheWrapper(world, &jsTestLegacyFactoryFunction->wrapped(), jsTestLegacyFactoryFunction);
}

#if ENABLE(BINDING_INTEGRITY)
#if PLATFORM(WIN)
#pragma warning(disable: 4483)
extern "C" { extern void (*const __identifier("??_7TestLegacyFactoryFunction@WebCore@@6B@")[])(); }
#else
extern "C" { extern void* _ZTVN7WebCore25TestLegacyFactoryFunctionE[]; }
#endif
#endif

JSC::JSValue toJSNewlyCreated(JSC::JSGlobalObject*, JSDOMGlobalObject* globalObject, Ref<TestLegacyFactoryFunction>&& impl)
{

    if constexpr (std::is_polymorphic_v<TestLegacyFactoryFunction>) {
#if ENABLE(BINDING_INTEGRITY)
        const void* actualVTablePointer = getVTablePointer(impl.ptr());
#if PLATFORM(WIN)
        void* expectedVTablePointer = __identifier("??_7TestLegacyFactoryFunction@WebCore@@6B@");
#else
        void* expectedVTablePointer = &_ZTVN7WebCore25TestLegacyFactoryFunctionE[2];
#endif

        // If you hit this assertion you either have a use after free bug, or
        // TestLegacyFactoryFunction has subclasses. If TestLegacyFactoryFunction has subclasses that get passed
        // to toJS() we currently require TestLegacyFactoryFunction you to opt out of binding hardening
        // by adding the SkipVTableValidation attribute to the interface IDL definition
        RELEASE_ASSERT(actualVTablePointer == expectedVTablePointer);
#endif
    }
    return createWrapper<TestLegacyFactoryFunction>(globalObject, WTFMove(impl));
}

JSC::JSValue toJS(JSC::JSGlobalObject* lexicalGlobalObject, JSDOMGlobalObject* globalObject, TestLegacyFactoryFunction& impl)
{
    return wrap(lexicalGlobalObject, globalObject, impl);
}

TestLegacyFactoryFunction* JSTestLegacyFactoryFunction::toWrapped(JSC::VM&, JSC::JSValue value)
{
    if (auto* wrapper = jsDynamicCast<JSTestLegacyFactoryFunction*>(value))
        return &wrapper->wrapped();
    return nullptr;
}

}