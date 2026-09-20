#ifndef __KIMI_PROVIDER__
#define __KIMI_PROVIDER__
#include "Common.h"
#include "LLMProvider.h"
#include "CommonStruct.h"
namespace Cplusplus_LLM_Provider
{
    class KimiProvider : public LLMProvider
    {
    public:
        KimiProvider() = default;
        ~KimiProvider() override = default;
        void InitModel(std::unordered_map<std::string,std::string> Config)override;
        std::string GetModelName()override;
        std::string GetModelDescription()override;
        std::string SendMessages(std::vector<CppAiChatSdk::Message>& messages,
            std::unordered_map<std::string,std::string>& RequestPrograms)override;
        std::string SendMessagesAsStream(std::vector<CppAiChatSdk::Message>& messages,
            std::unordered_map<std::string, std::string>& RequestPrograms,
            func_t callback)override;    
    private:
        bool IsModelAvailable() override;
    };
}

#endif