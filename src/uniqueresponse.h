#pragma once 
using namespace RE;
#include <nlohmann/json.hpp>
#include "util.h"
using FormID = std::uint32_t; 
namespace DDR
{

    struct UniqueResponse 
    {

        public: 
        FormID topicInfoID; 

        FormID voiceTypeID; 

        uint8_t index; 

        std::string overridePath; 

        std::string overrideSubtitle; 

        UniqueResponse(FormID a_topicInfoID, FormID a_voiceTypeID, uint8_t a_index, std::string path, std::string subtitle) : topicInfoID(a_topicInfoID), voiceTypeID(a_voiceTypeID), index(a_index)
        , overridePath(path), overrideSubtitle(subtitle) {}

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