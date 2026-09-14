#ifndef __SEEPSEEK_LLMPROVIDER__
#define __DEEPSEEK_LLMPROVIDER__
#include "Common.h"
#include "LLMProvider.h"
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
        std::string SendMessages();
        std::string SendMessagesAsStream();          
    };
}

#endif