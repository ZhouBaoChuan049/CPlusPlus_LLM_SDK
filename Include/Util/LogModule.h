#ifndef __LOG_MODULE__
#define __LOG_MODULE__
#include "../Common.h"

namespace LogModule
{
    class SpdLogPack
    {
    public:
        SpdLogPack() = delete ;
        SpdLogPack(SpdLogPack& spdlog) = delete ;
        void SpdLogInit(std::string logname,std::string filename, 
            spdlog::level::level_enum loglevel = spdlog::level::info
            ,  int mode = FILE_MODE) ;
        std::shared_ptr<spdlog::logger> GetSpdLog() ;
        
    private:
        std::shared_ptr<spdlog::logger> _logger ; 
        std::mutex _mutex;
    };
#define TRACE(format, ...) GetSpdLog()->trace(std::string("[{:>10}][{:<4}]")+format,__FILE__,__LINE__,##__VA_ARGS__);
#define DEBUG(format, ...) GetSpdLog()->debug(std::string("[{:>10}][{:<4}]")+format,__FILE__,__LINE__,##__VA_ARGS__);
#define INFO(format, ...) GetSpdLog()->info(std::string("[{:>10}][{:<4}]")+format,__FILE__,__LINE__,##__VA_ARGS__);
#define WARNING(format, ...) GetSpdLog()->warn(std::string("[{:>10}][{:<4}]")+format,__FILE__,__LINE__,##__VA_ARGS__);
#define ERROR(format, ...) GetSpdLog()->err(std::string("[{:>10}][{:<4}]")+format,__FILE__,__LINE__,##__VA_ARGS__);
#define CRITICAL(format, ...) GetSpdLog()->critical(std::string("[{:>10}][{:<4}]")+format,__FILE__,__LINE__,##__VA_ARGS__);
}
#endif