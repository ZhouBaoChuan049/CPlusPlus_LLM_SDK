#ifndef __LOG_MODULE__
#define __LOG_MODULE__
#include "../Common.h"


namespace LogModule
{
    class SpdLogPack
    {
    public:
        SpdLogPack()
        {}
        SpdLogPack() = delete ;
        SpdLogPack(SpdLogPack& spdlog) = delete ;
        void SpdLogInit() ;
        std::shared_ptr<spdlog::logger> GetSpdLog() ;
        
    private:
        std::shared_ptr<spdlog::logger> _logger ; 
        std::mutex _mutex;
    };
}
#endif