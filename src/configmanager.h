#pragma once 

#include "hook.h"
#include "util.h"
#include <nlohmann/json.hpp>
using namespace RE; 



namespace DDR 
{
    class ConfigManager 
    {
        public: 
        static void LoadConfigurationFiles(); 

        private: 

        static void ParseConfig(nlohmann::json json); 


    };
}