#include "../Include/ChatGPTProvider.h"
#include "../Include/Util/LogModule.h"
#include "../Include/Common.h"
#include "../Include/CommonStruct.h"

namespace Cplusplus_LLM_Provider
{
    bool ChatGPTProvider::IsModelAvailable()
    {
        return GetAvailable() ;
    }
    void ChatGPTProvider::InitModel
        (std::unordered_map<std::string,std::string> Config)
    {
        if(Config.find("_ApiKey") != Config.end())
            SetApiKey(Config["_ApiKey"]);
        if(Config.find("_APIAccessAddress") != Config.end())
            SetAPIAccessAddress(Config["_APIAccessAddress"]);
        SetAvailable(true) ;
    }
    std::string ChatGPTProvider::GetModelName()
    {
        std::string name = "gpt-5.4";
        return name;
    }
    std::string ChatGPTProvider::GetModelDescription()
    {
        std::string Description = 
        "GPT-5.4 是 OpenAI 的新一代通用大语言模型，具备较强的复杂推理、\
        代码生成、文本理解与任务执行能力。适用于软件开发、技术问答、内容生成、\
        数据分析以及多步骤复杂任务等场景。" ;
        return Description ;
    }
    std::string ChatGPTProvider::SendMessages(
        std::vector<CppAiChatSdk::Message>& messages,
        std::unordered_map<std::string,std::string>& RequestPrograms
    )
    {
        if(!IsModelAvailable()) {
            LogModule::CRITICAL("致命错误!模型不可用。");
            exit(INIT_EER);
        }
        //我们要构建整个请求报文，包括请求体（有历史请求报文和请求参数构成）
        //以及一个请求头，包含三个参数，比Deepseek多一个代理参数。
        Json::Value BodyMessages ;
        for(auto message : messages){
            Json::Value msg ;
            msg["role"] = message._Role ;
            msg["content"] = message._Content ;
            BodyMessages.append(msg);
        }
        std::string _model ;
        if(RequestPrograms.find("model") != RequestPrograms.end())
            _model = RequestPrograms["model"];
        std::string _temperature ;
        if(RequestPrograms.find("temperature") != RequestPrograms.end())
            _temperature = RequestPrograms["temperature"];
        std::string _max_output_tokens ;
        if(RequestPrograms.find("max_output_tokens") != RequestPrograms.end())
            _max_output_tokens = RequestPrograms["max_output_tokens"];

        //把报文序列化
        Json::Value RequestBody ;
        RequestBody["input"] = BodyMessages ;
        RequestBody["model"] = _model ;
        RequestBody["temperature"] = _temperature ;
        RequestBody["max_output_tokens"] = _max_output_tokens ; 

        Json::StreamWriterBuilder builder ;
        std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
        std::ostringstream ss ;
        int check = writer->write(RequestBody , &ss);
        if(check != 0){
            LogModule::ERROR("错误!请求报文的主体部分序列化失败。");
            std::string Except = "请求报文的主体部分序列化失败" ;
            throw Except ;
        }
        std::string RequestBodyString = ss.str();

        httplib::Client client(GetAPIAccessAddress());
        client.set_connection_timeout(60,0);
        client.set_read_timeout(120,0);
        client.set_proxy("172.17.112.1", 10090);
        
        httplib::Headers handers = {
            {"Content-Type" , "application/json"},
            {"Authorization" , "Bearer " + GetApiKey() }
        };
        httplib::Result result = client.Post(
            "/v1/responses",
            handers, 
            RequestBodyString,
            "application/json"
        );///chat/completions
        if(result == nullptr){
            LogModule::ERROR("没有接收到报文啊!");
            std::string Except("没有接收到报文啊!");
            throw Except ; 
        }
        else{
            if(result->status != 200){
                LogModule::ERROR("拿到报文了,但是报文状态错误!");
                return "" ;
            }
            else
                LogModule::INFO("拿到报文了,报文状态200!");
        }
        std::string ResponseString = result->body ;
        Json::Value ResponseJson ;
        std::string errstr ;
        Json::CharReaderBuilder readbuilder ;
        std::unique_ptr<Json::CharReader> reader(readbuilder.newCharReader());
        bool ans = reader->parse(
            ResponseString.c_str(),
            ResponseString.c_str() + ResponseString.size(),
            &ResponseJson,
            &errstr
        );
        if(!ans)
        {
            LogModule::ERROR("错误!应答报文反序列化错误!");
            std::string Except("错误!应答报文反序列化错误!");
            throw Except ; 
        }
        if(ResponseJson.isObject()&&
            ResponseJson.isMember("output")&&
            !ResponseJson.empty()){
                if(ResponseJson["output"].isArray()&&
                   !ResponseJson["output"].empty()){
                    if(!ResponseJson["output"][0].empty()&&
                         ResponseJson["output"][0].isObject()&&
                          ResponseJson["output"][0].isMember("content")){
                            if(ResponseJson["output"][0]["content"].isObject()&&
                                !ResponseJson["output"][0]["content"].empty()&&
                                  ResponseJson["output"][0]["content"].isMember("text")){
                                    return  ResponseJson["output"][0]["content"]["text"].asString();
                                  }
                        }
                   }
            }
        LogModule::ERROR("报文解析失败!");
        return "";
    }
    std::string ChatGPTProvider::SendMessagesAsStream(
        std::vector<CppAiChatSdk::Message>& messages,
        std::unordered_map<std::string, std::string>& RequestPrograms,
        func_t callback
    )
    {
        return "" ;
    }
}

