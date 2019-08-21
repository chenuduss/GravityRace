#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <sstream>
#include <fstream>
#include <mutex>

#define LOGGER_MAX_FILESIZE (8*1024*1024)

class Logger;

class LoggerEventTerminator
{
public:
    LoggerEventTerminator() {}
    ~LoggerEventTerminator() {}
};

class LoggerProxy
{
public:

    LoggerProxy();
    ~LoggerProxy();

    LoggerProxy& operator << (const std::string& value);
    LoggerProxy& operator << (unsigned value);
    LoggerProxy& operator << (signed value);
    LoggerProxy& operator << (size_t value);
    LoggerProxy& operator << (double value);
    LoggerProxy& operator << (const LoggerEventTerminator&);

    void SetParent(Logger* parent);

    void SetPrefix(const char* prefix);
protected:
    Logger* m_Parent;
    std::ostringstream m_TempBuffer;
    char m_Prefix[16];

    std::mutex m_Locker;
    void LockIfNeed();
    void Unlock();

    void SendAndClearTempBuffer();
};

class Logger
{
public:
    static LoggerEventTerminator End;

    Logger();
    ~Logger();
    void SetMaxLevel(unsigned level);

    LoggerProxy& Emerg;
    LoggerProxy& Alert;
    LoggerProxy& Crit;
    LoggerProxy& Error;
    LoggerProxy& Warn;
    LoggerProxy& Notice;
    LoggerProxy& Info;
    LoggerProxy& Debug;

    LoggerProxy& operator[] (unsigned level);

    void SetLogFolder(const std::string& folder)
    {
        m_LogFolder = folder;
    }

    void SaveToStorage(const std::string& value);

protected:
    LoggerProxy m_Child[8];
    std::string m_LogFolder;

    std::mutex m_WriterLocker;
    std::ofstream m_CurrentFileWorker;

    std::ofstream& GetFileWorker();
    std::string GetFileNameForLog() const;
};

extern Logger Log;

std::string GetCurrentDataTimeString();


#endif // LOGGER_H
