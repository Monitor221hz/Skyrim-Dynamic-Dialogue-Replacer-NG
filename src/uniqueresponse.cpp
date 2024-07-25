#include "uniqueresponse.h"


namespace DDR
{
    std::vector<UniqueResponse> UniqueResponse::ParseJson(nlohmann::json jsonObject)
    {
        std::vector<UniqueResponse> responses;
        auto &infoRecord = jsonObject["topicInfo"]; 

        std::vector<std::string> voiceNameArray = jsonObject["voiceTypes"]; 

        auto info = FormUtil::Form::GetFormIDFromConfigString(infoRecord); 
        if (info == -1) SKSE::log::info("Could not find topicInfo record"); 
        uint8_t index = jsonObject["responseIndex"];  

        std::string overridePath = jsonObject.contains("overridePath") ? jsonObject["overridePath"] : ""; 
        std::string overrideSubtitle = jsonObject.contains("overrideSubtitle") ? jsonObject["overrideSubtitle"] : ""; 
        std::string overrideScriptObjectRecord = "";
        ScriptObject overrideScriptObject; 
        if ( jsonObject.contains("overrideScript") ) 
        {
            auto& overrideScriptObjectObject = jsonObject["overrideScript"];
            overrideScriptObjectRecord = overrideScriptObjectObject["record"]; 

            auto objectID =  FormUtil::Form::GetFormIDFromConfigString(overrideScriptObjectRecord); 

            auto& propertyList = overrideScriptObjectObject["copyProperties"];

            overrideScriptObject.propertyNames = propertyList; 
            overrideScriptObject.formID = objectID; 
            
        }


        for (auto &entry : voiceNameArray)
        {
            
            auto voiceType = voiceTypeMap.contains(entry) ? voiceTypeMap[entry] : -1;
            voiceType = (voiceType == -1 && entry.contains('~')) ? FormUtil::Form::GetFormIDFromConfigString(entry): voiceType; 

                
            responses.emplace_back(UniqueResponse(info, voiceType, index, overridePath, overrideSubtitle, overrideScriptObject));
        }

        return responses; 
    }

    std::string UniqueResponse::FormatOverridePath(BGSVoiceType *voiceType, TESFile *modFile, std::string path)
    {
        if (path[0] != '$') return path; 

        auto sections = Util::String::Split(path, "\\"sv); 

        for(auto& section : sections)
        {
            if (section == "[VOICE_TYPE]") 
            {
                section = voiceType->GetFormEditorID(); 
            }
            else if (section == "[TOPIC_MOD_FILE]")
            {
                section = modFile->GetFilename(); 
            }
            else if (section == "[VOICE_MOD_FILE]")
            {
                section = voiceType->GetDescriptionOwnerFile()->GetFilename(); 
            }
            
        }

        return Util::String::Join(sections, "\\"sv); 
    }
    void UniqueResponse::ScriptObject::CopyScriptTo(TESTopicInfo *target)
    {
        if (formID == 0x0) return; 

        auto* dataHandler = TESDataHandler::GetSingleton(); 
        if (!dataHandler) return; 

        auto* form = TESForm::LookupByID(formID);
        if (form->GetFormType() != FormType::Info) return; 

        ScriptHandler::AttachDuplicatePropertyScript(target, form, propertyNames);  
    }
}