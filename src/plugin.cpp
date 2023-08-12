#include "log.h"
#include "hook.h"
#include "configmanager.h"
using namespace DDR; 
void OnDataLoaded()
{
   
}

void MessageHandler(SKSE::MessagingInterface::Message* a_msg)
{
	switch (a_msg->type) {
	case SKSE::MessagingInterface::kDataLoaded:
		UniqueResponse::MapVoiceTypes(); 
		ConfigManager::LoadConfigurationFiles(); 
		break;
	case SKSE::MessagingInterface::kPostLoad:
		break;
	case SKSE::MessagingInterface::kPreLoadGame:
		break;
	case SKSE::MessagingInterface::kPostLoadGame:
        break;
	case SKSE::MessagingInterface::kNewGame:
		break;
	}
}

SKSEPluginLoad(const SKSE::LoadInterface *skse) {
    SKSE::Init(skse);
	SetupLog();


    auto messaging = SKSE::GetMessagingInterface();
	if (!messaging->RegisterListener("SKSE", MessageHandler)) {
		return false;
	}
	// if (skse->RuntimeVersion() == SKSE::RUNTIME_SSE_LATEST_SE) Hooks::InstallVoiceFileHook(); 
	// Hooks::InstallResponseHook(); 
	VoicePathHook::Install(); 
	ResponseHook::Install(); 
    return true;
}