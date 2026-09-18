#include <iostream>
#include "../../Include/Util/LogModule.h"
#include "../../Include/DeepSeekProvider.h"
#include "../../Include/LLMProvider.h"
#include <gtest/gtest.h>

TEST(DeepSeekProviderTest , SendMessageTest)
{

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