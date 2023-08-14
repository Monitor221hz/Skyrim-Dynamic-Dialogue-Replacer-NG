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
            ResponseMap.emplace(uniqueResponse); 
        }
        private: 

        //sub_14056D570+DE call sub_14038DE50 SE
        //sub_140589FE0+DE	call    sub_1403A43D0 AE

        static bool ConstructResponse(TESTopicInfo::ResponseData* a_response, char *a_filePath, BGSVoiceType *a_voiceType, TESTopic *a_topic, TESTopicInfo *a_topicInfo);
        static inline REL::Relocation<decltype(ConstructResponse)> _ConstructResponse;



            
            //sub_14056D570+61 SE
            //sub_140589FE0+61 AE

        


        static inline std::unordered_set<UniqueResponse, UniqueResponseHash> ResponseMap; 

    };

    class SubtitleHook
    {
        public: 


        static void Install()
        {
            auto &trampoline = SKSE::GetTrampoline();
            SKSE::AllocTrampoline(16);
            REL::Relocation<std::uintptr_t> target{REL::RelocationID(34429, 35249)};
            _SetSubtitle = trampoline.write_call<5>(target.address() + REL::Relocate(0x61, 0x61), SetSubtitle);
        }

        static void AddTextOverride(std::string originalSubtitle, std::string newSubtitle)
        {
            auto findResult = Subtitles.find(newSubtitle); 
            auto* textPtr = (findResult == Subtitles.end()) ? &(*Subtitles.insert(newSubtitle).first) : &(*findResult); 
            TextOverrides.emplace(originalSubtitle, textPtr); 
        }

        private: 

        static char* SetSubtitle(DialogueResponse* a_response, char* text, int32_t unk); 
        static inline REL::Relocation<decltype(SetSubtitle)> _SetSubtitle; 
        
        static char* GetTextOverride(char* subtitle); 
        

        
        static inline std::set<std::string> Subtitles; 
        static inline std::unordered_map<std::string, const std::string*> TextOverrides; 

    };

}