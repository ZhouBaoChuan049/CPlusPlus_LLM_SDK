#ifndef __LLMPROVIDER__
#define __LLMPROVIDER__
#include "Common.h"
#include "CommonStruct.h"

namespace Cplusplus_LLM_Provider
{
    class LLMProvider
    {
    public:
        LLMProvider()
            :_IsAvailable(false),
             _ApiKey(""),
             _APIAccessAddress("")
        {}
        virtual ~LLMProvider() = default ;
        virtual void InitModel(std::unordered_map<std::string,std::string> Config) = 0;
        virtual bool IsModelAvailable() = 0;
        virtual std::string GetModelName() = 0;
        virtual std::string GetModelDescription() = 0;
        virtual std::string SendMessages(std::vector<CppAiChatSdk::Message>& messages,
             std::unordered_map<std::string,std::string>& RequestPrograms) = 0;
        virtual std::string SendMessagesAsStream() = 0;
    protected:
        void SetApiKey(std::string& ApiKey)
        {
            _ApiKey = ApiKey ;
        }
        void SetAPIAccessAddress(std::string APIAccessAddress)
        {
            _APIAccessAddress = APIAccessAddress ;
        }
        void SetAvailable(bool IsAvailable)
        {
            _IsAvailable = IsAvailable ;
        }
        std::string GetApiKey()
        {
            return _ApiKey ;
        }
        std::string GetAPIAccessAddress()
        {
            return _APIAccessAddress ;
        }
        bool GetAvailable()
        {
            return _IsAvailable ;
        }
    private:
        bool _IsAvailable ;
        std::string _ApiKey ;
        std::string _APIAccessAddress ;
    };
}

#endif