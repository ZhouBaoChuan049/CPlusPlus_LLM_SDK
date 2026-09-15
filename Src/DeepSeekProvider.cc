#include "../Include/Common.h"
#include "../Include/Util/LogModule.h"
#include "../Include/DeepSeekProvider.h"
#include <jsoncpp/json/json.h>
#include "../Third_Party/Httplib/httplib.h"
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
    std::string DeepSeekProvider::GetModelDescription()
    {
        return "DeepSeek是一款高性能国产大语言模型,擅长代码,推理与通用文本生成.";
    }
    bool DeepSeekProvider::IsModelAvailable()
    {
        if(!GetAvailable())
        {
            LogModule::CRITICAL("DeepSeek Model is not available!");
            exit(AVAILABLE_ERR);
        }
    }
    std::string DeepSeekProvider::Serialize(std::vector<CppAiChatSdk::Message>& messages,
             std::unordered_map<std::string,std::string>& RequestPrograms)
    {
        std::string model = "" ;
        int temperature = 0 ;
        bool stream = false;
        int Max_token = 0 ;
        if(RequestPrograms.find("model") != RequestPrograms.end())
            model = RequestPrograms["model"] ; 
        if(RequestPrograms.find("temperature") != RequestPrograms.end())
            temperature = std::stod(RequestPrograms["temperature"]) ; 
        if(RequestPrograms.find("Max_token") != RequestPrograms.end())
            Max_token = std::stoi(RequestPrograms["Max_token"]) ; 
        // std::string _Role ;
        // std::string _Content ;
        Json::Value BodyValues ;
        for(auto message : messages)
        {
            Json::Value value ;
            value["role"] = message._Role;
            value["content"] = message._Content;
            BodyValues.append(value);
        }

        //然后序列化这个请求体。
        Json::Value RequestBody ;
        RequestBody["model"] = model ;
        RequestBody["messages"] = BodyValues ;
        RequestBody["temperature"] = temperature ;
        RequestBody["max_tokens"] = Max_token ;
        RequestBody["stream"] = stream ;

        Json::StreamWriterBuilder builder ;
        std::unique_ptr<Json::StreamWriter> writer 
                (builder.newStreamWriter());
        std::ostringstream ss ;
        int CheckWrite = writer->write(RequestBody,&ss);
        if(CheckWrite != 0)
        {
            LogModule::ERROR("Json Writer fail!");
            exit(SERIALIZE_ERR);
        }
        return ss.str() ; 
    }
    httplib::Result DeepSeekProvider::SendRequestMessage(std::string& RequestBodyString)
    {
        //然后构建一个轻量化的客户端
        httplib::Client client(GetAPIAccessAddress());
        client.set_connection_timeout(30,0);
        client.set_read_timeout(60,0);

        //"Authorization: Bearer ${DEEPSEEK_API_KEY}"
        //构建请求报头
        httplib::Headers handers = {
            {"Content-Type" , "application/json"},
            {"Authorization" , "Bearer "+GetApiKey() }
        };
        //发送，发送的时候自带合成请求行 ，同时获取到响应报文
        httplib::Result answer = client.Post("/chat/completions",
             handers, RequestBodyString,
             "application/json");
        if(answer->status == 200)
        {
            LogModule::INFO("Get Response Success!");
            std::cout<<"The Response Status is:["
                << answer->status<<"]"<<std::endl;
            std::cout<<"The Response Body is:["
                << answer->body<<"]"<<std::endl;
        }
        else if(answer->status != 200)
        {
            LogModule::ERROR("Post Get Response Fail!");
            exit(POST_ERR);
        }
        return answer ;
    }
    Json::Value Deserialize(std::string ResponseString)
    {
        std::string ResponseString = ResponseString;
        Json::CharReaderBuilder readbuilder ;
        std::unique_ptr<Json::CharReader> reader
            (readbuilder.newCharReader());
        std::string parseerr ;
        Json::Value Response ;
        bool CheckParse = reader->parse(ResponseString.c_str() ,
                              ResponseString.c_str()+ResponseString.size(),
                              &Response, &parseerr);
        if(!CheckParse)
        {
            LogModule::ERROR("Deserialize fail!");
            exit(DESERIALIZE_ERR);
        }
        return Response ;
    }
    std::string DeepSeekProvider::SendMessages(std::vector<CppAiChatSdk::Message>& messages,
             std::unordered_map<std::string,std::string>& RequestPrograms)
    {
        //判断一下我们的这个模型有没有转起来。
        IsModelAvailable();
        std::string RequestBodyString = Serialize(messages , RequestPrograms);
        httplib::Result answer = SendRequestMessage(RequestBodyString);
        Json::Value Response = Deserialize(answer->body);
        //最后一层一层把content剥出来
        if(Response.isMember("choices") &&
           Response["choices"].isArray() &&
           !Response["choices"].empty()){
            for(int i = 0 ; i < Response["choices"].size(); i++){
                if(Response["choices"][i].isMember("message")&&
                    Response["choices"][i].isObject()&&
                    !Response["choices"][i].empty()){
                    if(Response["choices"][i]["message"].isMember("content")&&
                        Response["choices"][i]["message"].isString()&&
                        !Response["choices"][i]["message"].empty())
                    return Response["choices"][i]["message"]["content"].asString() ;
                }
            }
        }
        return "None";
    }
    std::string SendMessagesAsStream();  
}