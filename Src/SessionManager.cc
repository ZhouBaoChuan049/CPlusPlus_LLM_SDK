#include "../Include/SessionManager.h"
#include "../Include/Common.h"
#include "../Include/CommonStruct.h"
#include "../Include/Util/LogModule.h"
namespace Cplusplus_LLM_Provider
{
    std::string SessionManager::GenerateSessionId()
    {
        _mutex.lock();
        _SessionCount.fetch_add(1);
        time_t time = std::time(nullptr);
        std::ostringstream oss ;
        oss<<"Session_"<<time<<"_"<<std::setw(8)<<
            std::setfill('0')<<_SessionCount; 
        _mutex.unlock();
        return oss.str();
    }
    std::string SessionManager::GenerateMessageId(size_t MessageCounter)
    {
        _mutex.lock();
        MessageCounter++ ;
        time_t time = std::time(nullptr);
        std::ostringstream oss ;
        oss<<"Session_"<<time<<"_"<<std::setw(8)<<
            std::setfill('0')<<MessageCounter; 
        _mutex.unlock();
        return oss.str();
    }

    std::string SessionManager::CreatSession
        (const std::string& SessionName,const std::string ModelName)
    {
        _mutex.lock();
        Session session(SessionName) ;
        session._TimeCreate = std::time(nullptr);
        session._LastTime = std::time(nullptr);
        session._SessionID = GenerateSessionId();
        session._ModelNameUsed = ModelName ;
        
        _sessions[session._SessionID] = std::make_shared<Session>(session) ;
        _mutex.unlock();
        return session._SessionID ;
    }
    std::shared_ptr<Session> SessionManager::GetSession(const std::string& SessionId)
    {
        _mutex.lock();
        auto it = _sessions.find(SessionId);
        if(it == _sessions.end())
        {
            _mutex.unlock();
            LogModule::ERROR("没有找到{}对应的会话!",SessionId);
            return nullptr ;
        }
        _mutex.unlock();
        return it -> second ;
    }
    bool SessionManager::AddMessage(const std::string SessionId , 
            const Message& message)
    {
        _mutex.lock();
        std::shared_ptr<Session> TheSession = GetSession(SessionId);
        if(TheSession == nullptr)
        {
            _mutex.unlock();
            LogModule::ERROR("对会话{}添加消息失败!",SessionId);
            return false ;
        }
        Message NewMessage(message._Role,message._Content);
        NewMessage._MessageID = GenerateMessageId(TheSession->_Messages.size());
        NewMessage._Time = std::time(nullptr);
        TheSession->_Messages.push_back(NewMessage);
        _mutex.unlock();
        return true ;
    }
    std::vector<Message> SessionManager::GetHistoryMessages(const std::string SessionId)
    {
        _mutex.lock();
        std::shared_ptr<Session> TheSession = GetSession(SessionId);
        if(TheSession == nullptr)
        {
            _mutex.unlock();
            LogModule::ERROR("对会话{}获取历史消息失败!",SessionId);
            return {};
        }
        _mutex.unlock();
        return TheSession->_Messages;
    }
    void SessionManager::UpdateSessionTimesTamp(const std::string& SessionId)
    {

    }
    std::vector<std::string> SessionManager::GetSessionLists() const
    {
        _mutex.lock();
        std::vector<std::string> lists ;
        for(auto it : _sessions)
        {
            lists.push_back(it.first);
        }
        _mutex.unlock();
        return lists;
    }
    bool SessionManager::DeleteSession(const std::string& SessionId)
    {
        _mutex.lock();
        auto it = _sessions.find(SessionId);
        if(it == _sessions.end())
        {
            _mutex.unlock();
            LogModule::ERROR("删除会话{}失败!",SessionId);
            return false ;
        }
        _sessions.erase(it->first);
        _mutex.unlock();
        return true;
    }
    void SessionManager::ClearAllSessions()
    {
        _mutex.lock();
        for(auto it : _sessions)
        {
            _sessions.erase(it.first);
        }
        _mutex.unlock();
    }
    size_t SessionManager::GetSessionCount() const
    {

    }

}


