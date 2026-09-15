#ifndef __LLMPROVIDER__
#define __LLMPROVIDER__
#include "Common.h"

namespace Cplusplus_LLM_Provider
{
    class LLMProvider
    {
    public:
        LLMProvider()
        {}
        ~LLMProvider()
        {}
        virtual void InitModel() = 0;
        virtual bool IsModelAvailable() = 0;
        virtual std::string GetModelName() = 0;
        virtual std::string GetModelDescription() = 0;
        virtual std::string SendMessages() = 0;
        virtual std::string SendMessagesAsStream() = 0;
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
        bool GetAvailable()
        {
            return _IsAvailable ;
        }
        std::string GetAPIAccessAddress()
        {
            return _APIAccessAddress ;
        }
        std::string GetApiKey()
        {
            return _ApiKey ;
        }
    private:
        bool _IsAvailable ;
        std::string _ApiKey ;
        std::string _APIAccessAddress ;
    };
}


#endif