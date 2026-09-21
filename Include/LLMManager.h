#include "./Util/LogModule.h"
#include "Common.h"
#include "CommonStruct.h"
#include "LLMProvider.h"
namespace Cplusplus_LLM_Provider
{
    class LLMManager
    {
    public:
        LLMManager()
        {}
        ~LLMManager()
        {}
        void InitThisModule(std::string modelname) ;
        void GetAllAvailableModule();
        bool IsThisModelAvailable(std::string modelname);
        std::string SendMessageToThisModlue
            (std::vector<CppAiChatSdk::Message> messages);
        std::string SendMessageToThisModlueAsStream
            (std::vector<CppAiChatSdk::Message> messages, func_t callback);
    private:
        std::unordered_map<std::string, 
            std::unique_ptr<LLMProvider>> ProviderCollections ;
        std::unordered_map<std::string/*Name*/,std::string/*Info*/> ProviderInfo ;
    };
}