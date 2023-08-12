#include "hook.h"


namespace DDR
{


    



    char **VoicePathHook::SetResponseFilePath(char **a_pathProperty, char *a_filePath)
    {
        char* filePath = GetPathOverride(a_filePath); 
        filePath = GetTempPathOverride(a_filePath); 
        
        return filePath ? _SetResponseFilePath(a_pathProperty, filePath) : _SetResponseFilePath(a_pathProperty, a_filePath); 
    }

    char* VoicePathHook::GetPathOverride(char *targetPath)
    {
        std::string target(targetPath); 

        if (!PermanentPathOverrides.contains(target)) return targetPath; 
        auto* newPath = PermanentPathOverrides[target]; 

        SKSE::log::info("{} replaced by {}", target, *newPath); 
        auto* newPathPtr = const_cast<char*>(newPath->c_str()); 
        return newPathPtr; 
        
    }
    char *VoicePathHook::GetTempPathOverride(char *targetPath)
    {
        std::string target(targetPath); 

        if (!TemporaryPathOverrides.contains(target)) return targetPath; 
        auto* newPath = TemporaryPathOverrides[target]; 

        SKSE::log::info("{} replaced by {} temporarily", target, *newPath);
        auto* newPathPtr = const_cast<char*>(newPath->c_str()); 
        TemporaryPathOverrides.erase(target); 
        return newPathPtr; 
        
    }
    bool ResponseHook::ConstructResponse(TESTopicInfo::ResponseData * a_responseData, char *a_filePath, BGSVoiceType *a_voiceType, TESTopic *a_topic, TESTopicInfo *a_topicInfo)
    {
        bool result = _ConstructResponse(a_responseData, a_filePath, a_voiceType, a_topic, a_topicInfo); 
        auto response = UniqueResponse(a_topicInfo->GetFormID(), a_voiceType->GetFormID(), a_responseData->responseNumber, "");
        if (ResponseMap.contains(response))
        {
            VoicePathHook::AddTemporaryPathOverride(std::string(a_filePath), UniqueResponse::FormatOverridePath(a_voiceType, a_topicInfo->GetDescriptionOwnerFile(), ResponseMap[response])); 
        }
        a_responseData->responseText = "piss"; 
        return result; 
    }
}