#include <iostream>
#include <string>
#include <ctime>
#include <vector>
namespace CppAiChatSdk
{
    class Message
    {
    public:
        std::string _MessageID ;
        std::string _Role ;
        std::string _Content ;
        std::time_t _Time ; 
        Message(const std::string& role , const std::string& content)
            :_Role(role),
            _Content(content)
        {}
    };
    //模型配置信息
    class Config
    {
    public:
        std::string _ModdelName ;
        double _Temperature = 0.7 ;//模型温度
        int _MaxToken = 2048 ;
    };
    //API接入
    class APIConfig : public  Config
    {
    public:
        std::string _APIKey ;
    };
    //Ollama接入
    class OllamaConfig
    {
    public:
        
    };
    //LLM介绍信息
    class ModelInfo
    {
    public:
        std::string _ModelName ;
        std::string _ModelDesc ;
        std::string _ModelProvider ;
        std::string _APIAccessAddress ; 
        bool _IsThisModelAvailable ;
        ModelInfo(const std::string& modelname = "", const std::string modeldesc = "",
            const std::string& provider = "",const std::string& address = "")
            :_ModelName(modelname),
             _ModelDesc(modeldesc),
             _ModelProvider(provider),
             _APIAccessAddress(address)
        {}
    };
    class SessionInfo
    {
    public:
        std::string _SessionID ;
        std::string _ModelNameUsed ;
        std::vector<Message> _Messages;
        std::time_t _TimeCreate ;
        std::time_t _LastTime ;
        SessionInfo(std::string name)
            :_ModelNameUsed(name)
        {}
    };
}