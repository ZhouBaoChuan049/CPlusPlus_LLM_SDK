#include "./Util/LogModule.h"
#include "Common.h"
#include "CommonStruct.h"
#include "LLMProvider.h"
namespace Cplusplus_LLM_Provider
{
    class LLMManager
    {
    public:
        LLMManager() = default ;
        ~LLMManager() = default ;
        bool RegistrModule(std::string modelname, 
            std::shared_ptr<LLMProvider> provider);
        bool InitThisModule(std::string modelname,
            std::unordered_map<std::string,std::string>& Config) ;
        std::vector<std::pair<std::string,std::string>> GetAllAvailableModule();
        bool IsThisModelAvailable(std::string modelname);
        std::string SendMessageToThisModlue(
            std::string modelname,
            std::vector<CppAiChatSdk::Message> messages,
            std::unordered_map<std::string,std::string>& RequestPrograms);
        std::string SendMessageToThisModlueAsStream(
            std::string modelname,
            std::vector<CppAiChatSdk::Message> messages, 
            std::unordered_map<std::string,std::string>& RequestPrograms,
            func_t callback);
    private:
        void SetModelInformation(std::string modelname, std::string info);
        std::unordered_map<std::string, 
            std::shared_ptr<LLMProvider>> ProviderCollections ;
        std::unordered_map<std::string/*Name*/,std::string/*Info*/> ProviderInfo ;
    };
}