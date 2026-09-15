#ifndef __SEEPSEEK_LLMPROVIDER__
#define __DEEPSEEK_LLMPROVIDER__
#include "Common.h"
#include "LLMProvider.h"
#include "CommonStruct.h"
namespace Cplusplus_LLM_Provider
{
//class LLMProvider
    class DeepSeekProvider : public LLMProvider
    {
    public:
        void InitModel(std::unordered_map<std::string,std::string> Config);
        bool IsModelAvailable();
        std::string GetModelName();
        std::string GetModelDescription();
        std::string SendMessages(std::vector<CppAiChatSdk::Message>& messages,
             std::unordered_map<std::string,std::string>& RequestPrograms);
        std::string SendMessagesAsStream();    
    private:
        bool IsModelAvailable();
        std::string Serialize(std::vector<CppAiChatSdk::Message>& messages,
             std::unordered_map<std::string,std::string>& RequestPrograms);
        httplib::Result SendRequestMessage(std::string& RequestBodyString);
        Json::Value Deserialize(std::string ResponseString);
    };
}

#endif