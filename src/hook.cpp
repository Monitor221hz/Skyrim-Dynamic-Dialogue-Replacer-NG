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
        if (newPath->empty()) return targetPath; 

        SKSE::log::info("{} replaced by {} temporarily", target, *newPath);
        auto* newPathPtr = const_cast<char*>(newPath->c_str()); 
        TemporaryPathOverrides.erase(target); 
        return newPathPtr; 
        
    }
    bool ResponseHook::ConstructResponse(TESTopicInfo::ResponseData * a_responseData, char *a_filePath, BGSVoiceType *a_voiceType, TESTopic *a_topic, TESTopicInfo *a_topicInfo)
    {
        bool result = _ConstructResponse(a_responseData, a_filePath, a_voiceType, a_topic, a_topicInfo); 
        auto response = UniqueResponse(a_topicInfo->GetFormID(), a_voiceType->GetFormID(), a_responseData->responseNumber, "","", UniqueResponse::ScriptObject());
        if (ResponseMap.contains(response))
        {
            auto *targetResponse = &(*ResponseMap.find(response));
            auto scriptObject = targetResponse->overrideScriptObject;
            scriptObject.CopyScriptTo(a_topicInfo); 
            SubtitleHook::AddTextOverride(a_responseData->responseText.c_str(), targetResponse->overrideSubtitle); 
            VoicePathHook::AddTemporaryPathOverride(std::string(a_filePath), UniqueResponse::FormatOverridePath(a_voiceType, a_topicInfo->GetDescriptionOwnerFile(), targetResponse->overridePath)); 
        }
        return result; 
    }
    char *SubtitleHook::SetSubtitle(DialogueResponse *a_response, char *text, int32_t unk)
    {
        char* newText = GetTextOverride(text); 
        return _SetSubtitle(a_response, newText, unk); 
    }
    char *SubtitleHook::GetTextOverride(char *targetText)
    {
        std::string target(targetText); 

        if (!TextOverrides.contains(target)) return targetText; 

        auto* newText = TextOverrides[target]; 
        if (newText->empty()) return targetText; 

        auto* newTextPtr = const_cast<char*>(newText->c_str()); 
        TextOverrides.erase(target); 

        return newTextPtr; 
    }
}