#include "../Include/Common.h"
#include "../Include/Util/LogModule.h"
#include "../Include/DeepSeekProvider.h"

namespace Cplusplus_LLM_Provider
{
const std::string POS = "\n\n";
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
        else SetAPIAccessAddress(Config["_APIAccessAddress"]);
        SetAvailable(true);
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
            std::string Excepts("DeepSeek Model is not available");
            throw Excepts;
            //exit(AVAILABLE_ERR);
        }
        return true ;
    }
    std::string DeepSeekProvider::Serialize(std::vector<CppAiChatSdk::Message>& messages,
             std::unordered_map<std::string,std::string>& RequestPrograms, bool isstream)
    {
        std::string model = "" ;
        double temperature = 0.0 ;
        bool stream = isstream;
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
            std::string Excepts("Json Writer fail");
            throw Excepts;
            //exit(SERIALIZE_ERR);
        }
        return ss.str() ; 
    }
    httplib::Client DeepSeekProvider::CreateClient(int commect_timeout , int read_timeout)
    {
        httplib::Client client(GetAPIAccessAddress());
        client.set_connection_timeout(commect_timeout,0);
        client.set_read_timeout(read_timeout,0);
        return client ;
    }
    httplib::Result DeepSeekProvider::SendRequestMessage(std::string& RequestBodyString)
    {
        //然后构建一个轻量化的客户端
        httplib::Client client = CreateClient(30 , 60); 
        //"Authorization: Bearer ${DEEPSEEK_API_KEY}"
        //构建请求报头
        httplib::Headers handers = {
            {"Content-Type" , "application/json"},
            {"Authorization" , "Bearer " + GetApiKey() }
        };
        //发送，发送的时候自带合成请求行 ，同时获取到响应报文
        httplib::Result answer = client.Post("/chat/completions",
             handers, RequestBodyString,
             "application/json");  
        if(answer == nullptr)
        {
            std::cerr << "HTTP Request Error: "
                    << httplib::to_string(answer.error())
                    << std::endl;

            LogModule::ERROR("Post Request Failed!");
            return answer;
        }
        if(answer != nullptr)
        {
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
                std::string Except("Post Get Response Fail");
                throw Except;
                //exit(POST_ERR);
            }
        }
        return answer ;
    }
    Json::Value DeepSeekProvider::Deserialize(std::string& ResponseString)
    {
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
            std::string Except("Deserialize fail");
            throw Except;
            //exit(DESERIALIZE_ERR);
        }
        // std::cout << "Deserialize Response:\n"
        //   << Response.toStyledString()
        //   << std::endl;
        return Response ;
    }
    std::string DeepSeekProvider::SendMessages(std::vector<CppAiChatSdk::Message>& messages,
             std::unordered_map<std::string,std::string>& RequestPrograms)
    {
        //判断一下我们的这个模型有没有转起来。
        IsModelAvailable();
        std::string RequestBodyString = Serialize(messages , RequestPrograms, false);
        httplib::Result answer = SendRequestMessage(RequestBodyString);
        Json::Value Response ;
        if(answer != nullptr)
            Response = Deserialize(answer->body);
        else
        {
            LogModule::ERROR("Post Get Response Fail!");
            std::string Except("Post Get Response Fail");
            throw Except;
            //exit(POST_ERR);
        }
        //最后一层一层把content剥出来
        if(Response.isMember("choices") &&
           Response["choices"].isArray() &&
           !Response["choices"].empty()){
            for(int i = 0 ; i < Response["choices"].size(); i++){
                if(Response["choices"][i].isMember("message")&&
                    Response["choices"][i].isObject()&&
                    !Response["choices"][i].empty()){//应答不一定在"content"里面，——>"reasoning_content"
                    if((Response["choices"][i]["message"].isMember("content") || 
                         Response["choices"][i]["message"].isMember("reasoning_content"))&&
                          Response["choices"][i]["message"]["content"].isString()&&
                          !Response["choices"][i]["message"]["content"].empty())
                    return Response["choices"][i]["message"]["content"].asString() ;
                }
            }
        }
        return "";
    }
    std::string DeepSeekProvider::SendMessagesAsStream(std::vector<CppAiChatSdk::Message>& messages,
            std::unordered_map<std::string, std::string>& RequestPrograms,
            func_t callback)
    {
        //检查模型有没有起来
        IsModelAvailable();
        //构建请求报文的主体部分
        std::string RequestBodyString = Serialize(messages , RequestPrograms, true);
        //客户端，流式请求，发送等待可以长一点
        httplib::Client client = CreateClient(60 , 300); 
        //报头信息
        httplib::Headers TheHanders = {
            {"Content-Type" , "application/json"},
            {"Authorization" , "Bearer " + GetApiKey() },
            {"Accept" , "text/event-stream"}
        };
        
        httplib::Request request ;
        request.method = "POST" ;
        request.path = "/chat/completions" ;
        request.headers = TheHanders ;
        request.body = RequestBodyString ;
        //std::function<bool(const Response &response)>

        //相关的一些处理参数
        bool ERROR_STATUS = false ;
        std::string ERROR_DESCRIPTION = "" ;
        std::string AllResponse ;
        std::string buffer ;
        bool StringEndERR = true ;

        request.response_handler = [&](const httplib::Response &response)->bool{
            if(response.status != 200)
            {
                ERROR_STATUS = true;
                ERROR_DESCRIPTION = 
                    "发送出去了，收到的是错误的,错误码:"+ 
                        std::to_string(response.status);
                LogModule::ERROR(ERROR_DESCRIPTION);
                return false ;
            }
            return true ;
        };
    //     using ContentReceiverWithProgress = std::function<bool(
    // const char *data, size_t data_length, size_t offset, size_t total_length)>;
        request.content_receiver = [&](
            const char *data, 
            size_t len, 
            size_t offset, 
            size_t alllen
        )->bool{
            if(ERROR_STATUS)
                return false ;
            // data:{
            //     choices:[{
            //             delta:{
            //                 content: "Hello"
            //                 role: "assistant"
            //             }
            //         }
            //     ]
            // }
            //从接收缓冲区读上来
            std::string RecdBuffer(data);
            buffer += RecdBuffer.substr(0 , len);
            //分析出一段完整的SSE报文
            size_t sep_message = buffer.find(POS);
            std::string trunk ;
            if(sep_message != buffer.npos)
            {
                trunk = buffer.substr(0 , sep_message);
                buffer.erase(0,sep_message + POS.size());
            }
            size_t sep_word = trunk.find("data:");
            size_t end_point = trunk.find(
                "\n" , 
                sep_word 
            ); 
            std::string DataString = trunk.substr(
                sep_word + (buffer[sep_word+5] == ' ' ? 1 : 0),//跳过空格
                end_point - sep_word
            );
            if(DataString == "[DONE]")
            {
                LogModule::INFO("响应报文读取正常结束![DONE]");
                StringEndERR = false ;
                callback("[DONE]",true);
            }
            //反序列化:
            Json::Value DataJson ;
            std::string Error ;
            Json::CharReaderBuilder builder ;
            std::unique_ptr<Json::CharReader> reader = 
                std::make_unique<Json::CharReader>
                    (builder.newCharReader());
            int check = reader->parse(
                DataString.c_str(),
                DataString.c_str() + DataString.size(),
                &DataJson,
                &Error 
            );//data: [DONE]
            if(!check)
            {
                LogModule::ERROR("Deserialize fail!");
                std::string Except("Deserialize fail");
                throw Except;
            }
            if(DataJson.isObject()&&
               !DataJson.empty()&&
                 DataJson.isMember("choices")){
                if(DataJson["choices"].isArray()&&
                   !DataJson["choices"].empty()&&
                     DataJson["choices"].isMember("delta")){
                    if(DataJson["choices"]["delta"].isObject()&&
                        !DataJson["choices"]["delta"].empty()&&
                         DataJson["choices"]["delta"].isMember("content")){
                        if(DataJson["choices"]["delta"]["content"].isString()&&
                            !DataJson["choices"]["delta"]["content"].empty()){
                                AllResponse += 
                                    DataJson["choices"]["delta"]["content"].asString() ;
                                callback(
                                    DataJson["choices"]["delta"]["content"].asString(),
                                    false
                                );
                            }
                         }
                }
            }
        };
        //httplib::Result 里面重载了类型转换器.
        bool result = client.send(request);
        if(result == false)
        {
            //报文根本没有发出去！
            if(ERROR_STATUS)
            {
                LogModule::CRITICAL("状态错误!");
                callback("" , true);
            }
        }
        if(StringEndERR)
        {
            LogModule::ERROR("接收的报文在没有收到[DONE]的情况下异常结束!");
            callback("" , true);
        }
        return AllResponse ;
    } 
}