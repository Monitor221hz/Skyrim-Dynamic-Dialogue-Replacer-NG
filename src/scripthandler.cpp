#include "scripthandler.h"

bool DDR::ScriptHandler::AttachDuplicatePropertyScript(RE::TESForm *a_target, RE::TESForm *a_source, std::vector<std::string> propertyNames, std::string scriptName)
{
    // the voices... make them stop...
    
    if (a_target->GetFormType() != a_source->GetFormType())
        return false;
    a_source->InitializeData();  
    auto *vm = VirtualMachine::GetSingleton();
    auto *handlePolicy = vm->GetObjectHandlePolicy();
    if (!vm || !handlePolicy) return false; 

    BSSpinLockGuard scriptsLocker(vm->attachedScriptsLock); 

    auto sourceHandle = handlePolicy->GetHandleForObject(a_source->GetFormType(), a_source);

    auto targetHandle = handlePolicy->GetHandleForObject(a_target->GetFormType(), a_target);

    if (!sourceHandle)
        return false;
    // to-do: add handling for whether targetHandle already has same script attached , or leave it to user responsibility instead :P

    auto* sourceObject = GetFirstAttachedScript(vm, handlePolicy, sourceHandle); 
    RE::BSTSmartPointer<RE::BSScript::Object> targetObjectPtr;


    scriptName = scriptName.empty() ? sourceObject->GetTypeInfo()->GetName()  : scriptName;
    vm->CreateObject(scriptName, targetObjectPtr); 

    auto* bindPolicy = vm->GetObjectBindPolicy();

    auto* targetObject = targetObjectPtr.get(); 
    for(auto& propertyName : propertyNames)
    {
        auto* sourceProperty = sourceObject->GetProperty(propertyName); 
        auto* targetProperty = targetObject->GetProperty(propertyName); 
        targetProperty->SetObject(sourceProperty->GetObject()); //please let this work oh god please
    }
    try 
    {
        bindPolicy->BindObject(targetObjectPtr, targetHandle); 
    }
    catch (...)
    {
        SKSE::log::info("Object binding failed for script {} to object {} of type {}",scriptName, a_target->GetName(), a_target->GetFormType() );
        return false;  
    }
    SKSE::log::info("Script {} binded to object {} of type {}",scriptName, a_target->GetName(), a_target->GetFormType());
    return true; 
}

RE::BSScript::Object* DDR::ScriptHandler::GetFirstAttachedScript(VirtualMachine *vm, IObjectHandlePolicy *policy, VMHandle handle)
{

    if (!vm->attachedScripts.contains(handle)) return nullptr; 
    BSScript::Object* script = nullptr; 
    for(auto& attachedScript : vm->attachedScripts.find(handle)->second)
    {
        SKSE::log::info("Script {}", attachedScript.get()->GetTypeInfo()->GetName());
        script = attachedScript.get(); 
        break; 
    }
    // auto* firstAttachedScript = vm->attachedScripts.find(handle)->second.begin()->get(); 

    // if (!firstAttachedScript) return nullptr; 

    // return firstAttachedScript; 
    return script; 


}
