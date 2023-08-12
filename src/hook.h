#pragma once 
#include "uniqueresponse.h"
#include "util.h"
#include <unordered_set>

using namespace RE; 
namespace DDR
{
    class VoicePathHook 
    {
        public: 


        static void Install()
        {
            auto& trampoline = SKSE::GetTrampoline(); 
            SKSE::AllocTrampoline(16); 
            REL::Relocation<std::uintptr_t> target { REL::RelocationID(34429,35249)} ; 
            _SetResponseFilePath = trampoline.write_call<5>(target.address() + REL::Relocate(0xEC, 0xEC), SetResponseFilePath); 
        }
 
        static void AddPermanentPathOverride(std::string oldPath, std::string newPath)
        {
            auto findResult = Paths.find(newPath); 
            auto* pathPtr = (findResult == Paths.end()) ? &(*Paths.insert(newPath).first) : &(*findResult); 

            PermanentPathOverrides.emplace(oldPath, pathPtr); 
        }

        static void AddTemporaryPathOverride(std::string oldPath, std::string newPath)
        {
            auto findResult = Paths.find(newPath); 
            auto* pathPtr = (findResult == Paths.end()) ? &(*Paths.insert(newPath).first) : &(*findResult); 
            TemporaryPathOverrides.emplace(oldPath, pathPtr); 
        }



        static char* GetPathOverride(char* targetPath); 
        static char* GetTempPathOverride(char* targetPath); 
        
        private:
            


            static inline std::unordered_map<std::string, const std::string*> PermanentPathOverrides; 
            static inline std::unordered_map<std::string, const std::string*> TemporaryPathOverrides; 

            static inline std::set<std::string> Paths; 
            static inline std::unordered_set<std::string> UniquePaths; 



            static char** SetResponseFilePath(char** a_pathProperty, char * a_filePath); 
            //sub_14056D570+EC SE
            //sub_140589FE0+EC AE


            static inline REL::Relocation<decltype(SetResponseFilePath)> _SetResponseFilePath; 
    };

    class ResponseHook
    {
        public: 

        static void Install()
        {
            auto &trampoline = SKSE::GetTrampoline();
            SKSE::AllocTrampoline(16);
            REL::Relocation<std::uintptr_t> target{REL::RelocationID(34429, 35249)};
            _ConstructResponse = trampoline.write_call<5>(target.address() + REL::Relocate(0xDE, 0xDE), ConstructResponse);
        }

        static void AddResponseOverride(UniqueResponse uniqueResponse)
        {
            SKSE::log::info("OverridePath {}", uniqueResponse.overridePath); 
            ResponseMap.emplace(uniqueResponse, uniqueResponse.overridePath); 
        }
        private: 

        static bool ConstructResponse(TESTopicInfo::ResponseData* a_response, char *a_filePath, BGSVoiceType *a_voiceType, TESTopic *a_topic, TESTopicInfo *a_topicInfo);
        static inline REL::Relocation<decltype(ConstructResponse)> _ConstructResponse;

        static inline std::unordered_map<UniqueResponse, std::string, UniqueResponseHash> ResponseMap; 

    };

}