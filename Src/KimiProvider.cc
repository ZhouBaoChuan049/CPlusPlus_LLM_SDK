#include "../Include/KimiProvider.h"
#include "../Include/Util/LogModule.h"
#include "../Include/Common.h"
#include "../Include/CommonStruct.h"

namespace Cplusplus_LLM_Provider
{
        bool KimiProvider::IsModelAvailable() 
        {
            return GetAvailable() ;
        }
        void KimiProvider::InitModel(std::unordered_map<std::string,std::string> Config) 
        {
            if(Config.find("_ApiKey") != Config.end())
                SetApiKey(Config["_ApiKey"]) ;
            else 
                LogModule::CRITICAL("致命错误!无法获取ApiKey");
            if(Config.find("_APIAccessAddress") != Config.end())
                SetAPIAccessAddress(Config["_APIAccessAddress"]) ;
            else 
                LogModule::CRITICAL("致命错误!无法获取ApiKey");
            SetAvailable(true) ;
        }
        std::string KimiProvider::GetModelName() 
        {
            return "Kimi K3";
        }
        std::string KimiProvider::GetModelDescription() 
        {
            return "Kimi K3是月之暗面(Moonshot AI)推出的旗舰级、\
            开放权重、超长上下文、重点强化 Coding 和 Agent 能力的大模型";
        }

        std::string Serialize(
            std::vector<CppAiChatSdk::Message>& messages,
            std::unordered_map<std::string,std::string>& RequestPrograms,
            bool isstream)
        {
            
        }
        httplib::Result SendRequestMessage(std::string& RequestBodyString)
        {

        }
        Json::Value Deserialize(std::string& ResponseString)
        {

        }
        httplib::Client CreateClient(int commect_timeout , int read_timeout)
        {
            
        }




        std::string KimiProvider::SendMessages(
            std::vector<CppAiChatSdk::Message>& messages,
            std::unordered_map<std::string,std::string>& RequestPrograms
        ) 
        {

        }
        std::string KimiProvider::SendMessagesAsStream(
            std::vector<CppAiChatSdk::Message>& messages,
            std::unordered_map<std::string, std::string>& RequestPrograms,
            func_t callback
        ) 
        {

        }
}