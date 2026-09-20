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
        std::string name = "gpt-5.5";
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
        double _temperature ;
        if(RequestPrograms.find("temperature") != RequestPrograms.end())
            _temperature = std::stod(RequestPrograms["temperature"]);
        int _max_output_tokens ;
        if(RequestPrograms.find("max_output_tokens") != RequestPrograms.end())
            _max_output_tokens = std::stoi(RequestPrograms["max_output_tokens"]);

        //把报文序列化
        Json::Value RequestBody ;
        RequestBody["messages"] = BodyMessages ; 
        //RequestBody["input"] = BodyMessages ; //走中转不能用这个
        RequestBody["model"] = _model ;
        RequestBody["temperature"] = _temperature ;
        //RequestBody["max_output_tokens"] = _max_output_tokens ; //走中转不能用这个
        RequestBody["max_tokens"] = _max_output_tokens ; 
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
        //client.set_proxy("172.17.112.1", 10090); // 中转站是国内的可能不需要代理？我要代理可能会出错？
        
        httplib::Headers handers = {
            {"Content-Type" , "application/json"},
            {"Authorization" , "Bearer " + GetApiKey() }
        };
        httplib::Result result = client.Post(
            "/v1/chat/completions", //这里也是走中转站
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
                LogModule::ERROR("HTTP状态码: {}", result->status);
                LogModule::ERROR("服务器返回: {}", result->body);
                return "";
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
        if(!IsModelAvailable()) {
            LogModule::CRITICAL("致命错误!模型不可用。");
            exit(INIT_EER);
        }
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
        
        httplib::Headers _handers = {
            {"Content-Type" , "application/json"},
            {"Authorization" , "Bearer " + GetApiKey() }
        };    

        std::string ERROR_DESCRIPTION ="";
        bool ERROR_STATUS =false ;
        std::string buffer ;


        httplib::Request request ;
        request.method = "POST" ;
        request.path = "/v1/responses";
        request.headers = _handers ;
        request.response_handler = 
            [&](const httplib::Response& response)->bool{
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
        request.content_receiver = [&](
            const char *data, 
            size_t len, 
            size_t offset, 
            size_t alllen
        )->bool{
            if(ERROR_STATUS)
                return false ;
            //从接收缓冲区读上来
            std::string RecvBuffer(data,len);
            buffer += RecvBuffer ;
            // 读取到bufer了，
            // event: response.output_text.delta
            // data: {"type":"response.output_text.delta","item_id":"msg_001","output_index":0,"content_index":0,"delta":"你好"}
            //事件驱动类型:
            // response.create：表示响应对象创建好了
            // response.in_progress：表示模型开始工作
            // response.output_text.delta：表示一段段文字实时流出
            // response.output_completed：表示该输出块结束
            // response.completed：表示整个响应结束。
            int pos = buffer.find(POS);
            while(pos != std::string:: npos)
            {
                std::string trunk = buffer.substr(0 , pos);
                buffer.erase(0 , pos + POS.size());
                                                      //event: {}
                int EventPoint = trunk.find("event:");//012345678
                int DataPoint = trunk.find("data:");
                int SepPoint = trunk.find('\n',0);
                
                std::string EventString = trunk.substr(EventPoint+7, SepPoint);
                std::string DataString = trunk.substr(DataPoint+7);
                
                if(EventString == "response.create")
                {

                }
                else if(EventString == "in_progress")
                {

                }
                else if(EventString == "output_text.delta")
                {
                    
                }
                else if(EventString == "output_completed")
                {
                    
                }
                else if(EventString == "completed")
                {
                    
                }
            }
        };

    }
}

