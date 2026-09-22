#include <iostream>
#include "../../Include/Util/LogModule.h"
#include "../../Include/DeepSeekProvider.h"
#include "../../Include/LLMProvider.h"
#include "../../Include/CommonStruct.h"
#include <gtest/gtest.h>
using namespace Cplusplus_LLM_Provider ;

TEST(DeepSeekProviderTest , SendMessageTest)
{
    std::unique_ptr<Cplusplus_LLM_Provider::LLMProvider> provider =
        std::make_unique<Cplusplus_LLM_Provider::DeepSeekProvider>();

    ASSERT_FALSE(provider == nullptr);
    
    std::unordered_map<std::string ,std::string> Config;
    Config["model"] = "deepseek-flash";
    Config["_ApiKey"] = getenv("deepseek_apikey") ;
    Config["_APIAccessAddress"] = "https://api.deepseek.com" ;
    provider->InitModel(Config);

    ASSERT_TRUE(provider->IsModelAvailable());

    std::vector<Message> messages;
    messages.push_back({"user" , "你好!请介绍你自己!"}); 

    std::unordered_map<std::string,std::string> RequestPrograms;
    RequestPrograms["temperature"] = "1.2";
    RequestPrograms["Max_token"] = "4096";
    RequestPrograms["model"] = "deepseek-flash";

    // std::string SendMessagesAsStream(std::vector<CppAiChatSdk::Message>& messages,
    //     std::unordered_map<std::string, std::string>& RequestPrograms,
    //     func_t callback)override;   
    //using func_t = std::function<void(std::string , bool)>;
    std::string AllResponse = provider->SendMessagesAsStream(
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
}   

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc , argv);
    LogModule::SpdLogPack::SpdLogInit("GoogleTestLog",
                          "TestLogFile.dat",
                           spdlog::level::debug,
                           CONSOLE__MODE);
    return RUN_ALL_TESTS();
}