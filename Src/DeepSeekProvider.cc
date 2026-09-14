#include "../Include/Common.h"
#include "../Include/Util/LogModule.h"
#include "../Include/DeepSeekProvider.h"

namespace Cplusplus_LLM_Provider
{
    void DeepSeekProvider::InitModel(std::unordered_map<std::string,std::string> Config)
    {
        //第一个存名称，第二个存内容
        if(Config.find("_ApiKey") == Config.end())
        {
            LogModule::CRITICAL("InitModel Fail!{}","_ApiKey NoFind");
            exit(Exception::INIT_EER);
        }
        else SetApiKey(Config["_ApiKey"]);
        if(Config.find("_APIAccessAddress") == Config.end())
        {
            LogModule::CRITICAL("InitModel Fail!{}","_APIAccessAddress NoFind");
            exit(Exception::INIT_EER);
        }
        else SetApiKey(Config["_APIAccessAddress"]);
        SetAvailable(true);
    }
    bool DeepSeekProvider::IsModelAvailable()
    {
        return GetAvailable() ;
    }
    std::string DeepSeekProvider::GetModelName()
    {
        return "DeepSeekv4flash" ;
    }
    std::string GetModelDescription()
    {
        return "DeepSeek:高性能国产大语言模型,擅长代码,推理与通用文本生成.";
    }
    std::string SendMessages();
    std::string SendMessagesAsStream();  

}