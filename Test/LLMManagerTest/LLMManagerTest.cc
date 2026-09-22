#include <iostream>
#include "../../Include/Util/LogModule.h"
#include "../../Include/LLMManager.h"
#include "../../Include/LLMProvider.h"
#include "../../Include/DeepSeekProvider.h"
#include "../../Include/ChatGPTProvider.h"
#include "../../Include/OllamaProvider.h"
#include <gtest/gtest.h>
using namespace Cplusplus_LLM_Provider ;
TEST(TestLLMManager , TestFunctions)
{
    LLMManager manager ;
    manager.RegistrModule(
        "deepseek-flash",
        std::make_shared<DeepSeekProvider>()
    );
    manager.RegistrModule(
        "gpt-5.5",
        std::make_shared<ChatGPTProvider>()
    );
    manager.RegistrModule(
        "deepseek-r1:1.5b",
        std::make_shared<OllamaProvider>()
    );
    
    std::unordered_map<std::string ,std::string> ConfigForDeepseek;
    ConfigForDeepseek["_ApiKey"] = getenv("deepseek_apikey") ;
    ConfigForDeepseek["_APIAccessAddress"] = "https://api.deepseek.com" ;
    manager.InitThisModule("deepseek-flash", ConfigForDeepseek);

    std::unordered_map<std::string ,std::string> ConfigForGpt;
    ConfigForGpt["_ApiKey"] = getenv("chatgpt_apikey") ;
    ConfigForGpt["_APIAccessAddress"] = "https://leapone.leapinfra.cn" ; 
    manager.InitThisModule("gpt-5.5", ConfigForGpt);
    
    std::unordered_map<std::string ,std::string> ConfigForOllama;
    ConfigForOllama["ModelName"] = "deepseek-r1:1.5b" ;
    ConfigForOllama["ModelDesc"] = "deepseek-r1:1.5b 是 DeepSeek-R1 \
    系列中参数最小的蒸馏模型,基于 Qwen2.5-1.5B 微调而来。它保留了 \
    R1 的推理能力,体积仅约1.1GB,普通电脑就能流畅运行。在数学和编\
    程任务上表现不错,MIT 协议允许免费商用";
    ConfigForOllama["APIAccessAddress"] = "127.0.0.1:11434" ;
    manager.InitThisModule("deepseek-r1:1.5b", ConfigForOllama);
    
    std::vector<std::pair<std::string,std::string>> models = manager.GetAllAvailableModule();
    for(auto m : models)
        std::cout<<"模型名称:["<<m.first<<"],模型描述信息:["<<m.second<<"]"<<std::endl;
    
    ASSERT_TRUE(manager.IsThisModelAvailable("deepseek-flash"));
    ASSERT_TRUE(manager.IsThisModelAvailable("gpt-5.5"));
    ASSERT_TRUE(manager.IsThisModelAvailable("deepseek-r1:1.5b"));

#ifdef __DeepSeek_Send__
    std::vector<CppAiChatSdk::Message> messages;
    messages.push_back({"user" , "你好!请介绍你自己!"}); 
    
    std::unordered_map<std::string,std::string> RequestPrograms;
    RequestPrograms["temperature"] = "1.2";
    RequestPrograms["Max_token"] = "40960";

    manager.SendMessageToThisModlue(
        "deepseek-flash",
        messages,
        RequestPrograms
    );
#ifdef __STREAM__
    std::string AllResponse = manager.SendMessageToThisModlueAsStream(
        "deepseek-flash",
        messages, 
        RequestPrograms,
        [](std::string content , bool least){
            if(least)
                return ;
            LogModule::INFO(content);
        }
    );
    ASSERT_FALSE(AllResponse.empty());
    LogModule::INFO("Response is{}.",AllResponse);
#endif
#endif

}

int main(int argc,char* argv[])
{
    ::testing::InitGoogleTest(&argc,argv);
    LogModule::SpdLogPack::SpdLogInit(
        "LLMTestLog", 
        "log.dat",
        spdlog::level::info,
        0
    );
    return RUN_ALL_TESTS();
}