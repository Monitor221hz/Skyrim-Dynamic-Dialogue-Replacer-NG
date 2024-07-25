#pragma once 
using namespace RE;
#include <nlohmann/json.hpp>
#include "util.h"
#include "scripthandler.h"
using FormID = std::uint32_t; 
namespace DDR
{

    struct UniqueResponse 
    {
        struct ScriptObject 
        {
            public: 

            FormID formID = 0x0; 
            std::vector<std::string> propertyNames; 
            ScriptObject() : formID(0x0) {} 
            ScriptObject(FormID a_formId, std::vector<std::string> a_propertyNames) : formID(a_formId), propertyNames(a_propertyNames) {} 

            void CopyScriptTo(TESTopicInfo* target); 
        };
        public: 
        FormID topicInfoID; 

        FormID voiceTypeID; 

        uint8_t index; 

        std::string overridePath; 

        std::string overrideSubtitle; 

        ScriptObject overrideScriptObject; 

        UniqueResponse(FormID a_topicInfoID, FormID a_voiceTypeID, uint8_t a_index, std::string path, std::string subtitle, ScriptObject a_scriptObject) : topicInfoID(a_topicInfoID), voiceTypeID(a_voiceTypeID), index(a_index)
        , overridePath(path), overrideSubtitle(subtitle), overrideScriptObject(a_scriptObject) {}

        static std::vector<UniqueResponse> ParseJson(nlohmann::json jsonObject); 

        
        
        bool operator==(const UniqueResponse& r) const 
        {
            return (voiceTypeID == r.voiceTypeID && topicInfoID == r.topicInfoID && index == r.index);
        }

        static void MapVoiceTypes()
        {
            auto *dataHandler = TESDataHandler::GetSingleton();
            if (!dataHandler)
                return;

            auto &voiceTypes = dataHandler->GetFormArray(FormType::VoiceType);

            for (auto *voiceType : voiceTypes)
            {
                // SKSE::log::info("{} mapped to {:X}", voiceType->GetFormEditorID(), voiceType->GetFormID());
                voiceTypeMap.emplace(std::string(voiceType->GetFormEditorID()), voiceType->GetFormID());
            }
        }


        static std::string FormatOverridePath(BGSVoiceType* voiceType, TESFile* modFile, std::string path); 

        

        private: 


        static inline std::unordered_map<std::string, RE::FormID> voiceTypeMap; 

    };

    class UniqueResponseHash
    {
        public:
            size_t operator()(const     UniqueResponse& response) const
            {
                const std::size_t fnv_prime = 16777619;
                std::size_t hash_val = 2166136261; 

                hash_val = (hash_val ^ static_cast<std::size_t>(response.voiceTypeID)) * fnv_prime;
                hash_val = (hash_val ^ static_cast<std::size_t>(response.topicInfoID)) * fnv_prime;
                hash_val = (hash_val ^ static_cast<std::size_t>(response.index)) * fnv_prime;

                return hash_val;
            }
    };
}