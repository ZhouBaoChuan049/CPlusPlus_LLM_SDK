#ifndef __SESSION_MANAGER__
#define __SESSION_MANAGER__
#include "Common.h"
#include "CommonStruct.h"
namespace Cplusplus_LLM_Provider
{
    class SessionManager
    {
    public:
        SessionManager();
        std::string CreatSession(const std::string& ModelName);
        std::shared_ptr<SessionInfo> GetSession(const std::string& SessionId);
        bool AddMessage(const std::string SessionId , const Message& message);
        std::vector<Message> GetHistoryMessages(const std::string SessionId) const ;
        void UpdateSessionTimesTamp(const std::string& SessionId);
        std::vector<std::string> GetSessionLists() const;
        bool DeleteSession(const std::string& SessionId);
        void ClearAllSessions();
        size_t GetSessionCount() const;
    private:
        std::string GenerateSessionId();
        std::string GenerateMessageId(size_t MessageCounter);
        std::unordered_map<std::string,std::shared_ptr<SessionInfo>> _sessions;
        mutable std::mutex _mutex;
        std::atomic<int64_t> _SessionCount = {0};
    };
}
#endif