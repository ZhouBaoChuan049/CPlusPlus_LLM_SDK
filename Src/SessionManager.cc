#include "../Include/SessionManager.h"
#include "../Include/Common.h"
#include "../Include/CommonStruct.h"
#include "../Include/Util/LogModule.h"
namespace Cplusplus_LLM_Provider
{
    std::string SessionManager::GenerateSessionId()
    {
    }
    std::string SessionManager::GenerateMessageId(size_t MessageCounter)
    {
    }
    std::string SessionManager::CreatSession(const std::string& ModelName)
    {
    }
    std::shared_ptr<SessionInfo> SessionManager::GetSession(const std::string& SessionId)
    {
    }
    bool SessionManager::AddMessage(const std::string SessionId , 
            const Message& message)
    {
    }
    std::vector<Message> SessionManager::GetHistoryMessages(const std::string SessionId) const 
    {

    }
    void SessionManager::UpdateSessionTimesTamp(const std::string& SessionId)
    {

    }
    std::vector<std::string> SessionManager::GetSessionLists() const
    {

    }
    bool SessionManager::DeleteSession(const std::string& SessionId)
    {

    }
    void SessionManager::ClearAllSessions()
    {

    }
    size_t SessionManager::GetSessionCount() const
    {

    }

}


