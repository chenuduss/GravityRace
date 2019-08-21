#include "logger.h"

#include <iostream>

Logger Log;

LoggerEventTerminator Logger::End;

Logger::Logger()
    : Emerg(m_Child[0]),
      Alert(m_Child[1]),
      Crit(m_Child[2]),
      Error(m_Child[3]),
      Warn(m_Child[4]),
      Notice(m_Child[5]),
      Info(m_Child[6]),
      Debug(m_Child[7])
{    
    m_Child[0].SetPrefix("[EMERG]");
    m_Child[1].SetPrefix("[ALERT]");
    m_Child[2].SetPrefix("[CRIT] ");
    m_Child[3].SetPrefix("[ERROR]");
    m_Child[4].SetPrefix("[WARN] ");
    m_Child[5].SetPrefix("[NOTE] ");
    m_Child[6].SetPrefix("[INFO] ");
    m_Child[7].SetPrefix("[DEBUG]");
}

Logger::~Logger()
{

}

LoggerProxy& Logger::operator[] (unsigned level)
{
    return m_Child[level];
}

void Logger::SetMaxLevel(unsigned level)
{
    for (unsigned i = 0; i < 8; i++)
    {
        m_Child[i].SetParent(nullptr);
    }

    if ((level >= 0) && (level < 8))
    {
        for (unsigned i = 0; i <= level; i++)
        {
            m_Child[i].SetParent(this);
        }
    }
}

std::string GetCurrentDataTimeString()
{
    std::time_t t = std::time(NULL);
    char mbstr[100];
    if (!std::strftime(mbstr, sizeof(mbstr), "%Y-%m-%d %H:%M:%S", std::localtime(&t)))
    {
         throw std::runtime_error("LoggerProxy:: strftime failed!");
    }

    return std::string(mbstr);
}

LoggerProxy& LoggerProxy::operator << (const std::string& value)
{
    LockIfNeed();

    if (0 == m_TempBuffer.tellp())
    {
        m_TempBuffer << GetCurrentDataTimeString() << " | " << m_Prefix << " ";
    }

    m_TempBuffer << value;
    return *this;
}

LoggerProxy& LoggerProxy::operator << (unsigned value)
{
    LockIfNeed();
    m_TempBuffer << value;
    return *this;
}

LoggerProxy& LoggerProxy::operator << (signed value)
{
    LockIfNeed();
    m_TempBuffer << value;
    return *this;
}

LoggerProxy& LoggerProxy::operator << (size_t value)
{
    LockIfNeed();
    m_TempBuffer << value;
    return *this;
}

LoggerProxy& LoggerProxy::operator << (double value)
{
    LockIfNeed();
    m_TempBuffer << value;
    return *this;
}

void LoggerProxy::SendAndClearTempBuffer()
{
    if (m_Parent)
    {
        m_Parent->SaveToStorage(m_TempBuffer.str());
        m_TempBuffer.seekp(0);
        m_TempBuffer.str("");
        Unlock();
    }
}

LoggerProxy& LoggerProxy::operator << (const LoggerEventTerminator&)
{
    SendAndClearTempBuffer();
    return *this;
}

void LoggerProxy::LockIfNeed()
{
    if (0 == m_TempBuffer.tellp())
    {
        m_Locker.lock();
    }
}

void LoggerProxy::Unlock()
{
    m_Locker.unlock();
}

void LoggerProxy::SetParent(Logger* parent)
{
    if (m_Parent)
    {
        LockIfNeed();
        m_Parent = parent;
        Unlock();
    }   else
    {
        m_Parent = parent;
    }
}

LoggerProxy::LoggerProxy()
    : m_Parent(nullptr)
{

}

LoggerProxy::~LoggerProxy()
{
    SendAndClearTempBuffer();
}

std::string Logger::GetFileNameForLog() const
{
    std::string result = m_LogFolder;

    if (result.empty())
    {
        throw std::invalid_argument("log folder not recognized");
    }

    if (*result.rbegin() != '/')
    {
        result += "/";
    }

    result += GetCurrentDataTimeString();
    result += ".log";

    //std::cout << result << std::endl;

    return result;
}

std::ofstream& Logger::GetFileWorker()
{
    bool neednew = false;
    if (!m_CurrentFileWorker.is_open())
    {
        neednew = true;
    }   else if (m_CurrentFileWorker.tellp() > LOGGER_MAX_FILESIZE)
    {
        neednew = true;
    }

    if (neednew)
    {
        m_CurrentFileWorker.flush();
        m_CurrentFileWorker.close();
        m_CurrentFileWorker.open(GetFileNameForLog());
    }

    return m_CurrentFileWorker;
}

void Logger::SaveToStorage(const std::string& value)
{
    std::unique_lock<std::mutex> locker(m_WriterLocker);

    if (m_LogFolder.empty())
    {
        std::cout << value << std::endl;
    }   else
    {
        (GetFileWorker() << value << "\r\n").flush();

        /*if (GetFileWorker().tellp() %2)
        {
            GetFileWorker().flush();
        }*/
    }
}

void LoggerProxy::SetPrefix(const char* prefix)
{
    for (char* ch = (char*)prefix; *ch; ch++)
    {
        m_Prefix[ch-prefix] = *ch;
    }
}
