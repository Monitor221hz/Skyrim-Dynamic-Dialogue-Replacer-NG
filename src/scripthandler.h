#pragma once 


using namespace RE;
using namespace RE::BSScript;  
using namespace RE::BSScript::Internal; 


namespace DDR
{

    class ScriptHandler
    {
        

        public: 



        static bool AttachDuplicatePropertyScript(RE::TESForm* a_target, RE::TESForm* a_source, std::vector<std::string> propertyNames,std::string scriptName = "");

        private: 

        static RE::BSScript::Object* GetFirstAttachedScript(VirtualMachine* vm, IObjectHandlePolicy* policy, VMHandle handle);

         
        


    }; 
}