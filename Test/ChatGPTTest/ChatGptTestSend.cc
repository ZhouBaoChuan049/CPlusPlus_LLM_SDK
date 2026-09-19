#include <iostream>
#include "../../Include/Util/LogModule.h"
#include "../../Include/ChatGPTProvider.h"
#include "../../Include/LLMProvider.h"
#include <gtest/gtest.h>

TEST(ChatGptProviderTest , SendMessageTest)
{
    std::unique_ptr<Cplusplus_LLM_Provider::LLMProvider> provider =
         std::make_unique<Cplusplus_LLM_Provider::ChatGPTProvider>();

    ASSERT_FALSE(provider == nullptr);
    
    std::unordered_map<std::string ,std::string> Config;
    Config["model"] = "gpt-5.4";
    Config["_ApiKey"] = getenv("chatgpt_apikey") ;
    Config["_APIAccessAddress"] = "https://api.openai.com" ;
    provider->InitModel(Config);

    ASSERT_TRUE(provider->IsModelAvailable());
    std::cout<<provider->GetModelName()<<std::endl;
    std::cout<<provider->GetModelDescription()<<std::endl;

    std::vector<CppAiChatSdk::Message> messages;
    //messages.push_back({"user" , "你好!请告诉我你的名字!"});
    messages.push_back({"user" , "你好!请介绍你自己!"}); 
    
    std::unordered_map<std::string,std::string> RequestPrograms;
    RequestPrograms["temperature"] = "1.2";
    RequestPrograms["max_output_tokens"] = "2048";
    RequestPrograms["model"] = "gpt-5.4";

    std::string response = provider->SendMessages(messages , RequestPrograms);
    ASSERT_FALSE(response.empty());
    //std::cout<<response<<std::endl;
    LogModule::INFO("Response is{}.",response);
}   

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest(&argc , argv);
    LogModule::SpdLogPack::SpdLogInit("GoogleTestLog",
                          "TestLogFile.dat",
                           spdlog::level::info,
                           CONSOLE__MODE);
    return RUN_ALL_TESTS();
}