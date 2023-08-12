#include "configmanager.h"

namespace DDR
{
    void ConfigManager::LoadConfigurationFiles()
    {

        std::vector<std::string> files = SystemUtil::File::GetConfigs(R"(Data\Sound\Voice\DynamicDialogueReplacer)", "", ".json"sv);
        SKSE::log::info("Found {} configuration files", files.size());
        for (auto path: files)
        {
            std::ifstream reader;
            reader.open(path);
            nlohmann::json configData; 
            
            try 
            {
                reader >> configData; 
                ParseConfig(configData); 
            }
            catch(...)
            {
                SKSE::log::warn("Failed to parse config file {}", path); 
            }
            reader.close(); 
            // for (auto configIt = configData.begin(); configIt != configData.end(); ++configIt) 
            // {

            //     auto lookup = (*configIt).find("Keys"); 
            // }
        }
    }

    

    void ConfigManager::ParseConfig(nlohmann::json json)
    {
        if (json.contains("pathMap"))
        {
            auto &pathObject = json["pathMap"];

            for (auto &keyValuePair : pathObject.items())
            {
                VoicePathHook::AddPermanentPathOverride(keyValuePair.key(), keyValuePair.value());
            }
        }
        if (json.contains("responseMap"))
        {
            auto &jsonGroups = json["responseMap"];

            for (auto &jsonGroup : jsonGroups.items())
            {
                SKSE::log::info("Parsing group : {}", jsonGroup.key()); 
                for(auto &jsonData : jsonGroup.value().items())
                {
                    SKSE::log::info("Parsing response : {}", jsonData.key()); 
                    try
                    {
                        auto responsearray = UniqueResponse::ParseJson(jsonData.value());
                        for(auto& response : responsearray)
                        {
                            ResponseHook::AddResponseOverride(response); 
                        }
                    }
                    catch (...)
                    {
                        SKSE::log::info("Failed to parse response.");
                    }
                }
                
            }
        }


        
    }
}