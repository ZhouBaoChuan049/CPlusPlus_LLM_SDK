#ifndef __COMMON__
#define __COMMON__
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>
#include <sstream>
#include <memory>
#include <unordered_map>
#define FILE_MODE 0 
#define CONSOLE__MODE 1
#define ROTATING_MODE 2
#define ASYNC_MODE 3

enum Exception
{
    OK = 0,
    INIT_EER ,
    // AVAILABLE_ERR,
    // SERIALIZE_ERR,
    // POST_ERR,
    // DESERIALIZE_ERR,
};

#endif