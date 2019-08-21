#include <iostream>

#include "matchservercontroller.h"

#include <string>
#include <memory>
#include <thread>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

#include "logger.h"


const char* ws = " \t\n\r\f\v\"";


inline std::string& rtrim(std::string& s, const char* t = ws)
{
    s.erase(s.find_last_not_of(t) + 1);
    return s;
}

// trim from beginning of string (left)
inline std::string& ltrim(std::string& s, const char* t = ws)
{
    s.erase(0, s.find_first_not_of(t));
    return s;
}

// trim from both ends of string (left & right)
inline std::string& trim(std::string& s, const char* t = ws)
{
    return ltrim(rtrim(s, t), t);
}

int main()
{
    std::string redisHost = "127.0.0.1";

    unsigned GrpPort = 2020;
    unsigned LogLevel = 7;
    std::string LogFolder = "";

    try
    {
        boost::property_tree::ptree pt;
        boost::property_tree::ini_parser::read_ini("GravityRaceServer.conf", pt);

        try
        {
            GrpPort = pt.get<unsigned>("Main.GrpPort");
        }   catch(...)
        {
            std::cout << "GRP port not declared in config. Setting GRP port to 2020" << std::endl;
        }

        try
        {
            LogLevel = pt.get<signed>("Main.LogLevel");
        }   catch(...)
        {
            std::cout << "Log level not declared in config. Setting log level to 7" << std::endl;
        }

        try
        {
            LogFolder = pt.get<std::string>("Main.LogFolder");
            trim(LogFolder);
        }   catch(...)
        {
            std::cout << "Log folder not declared in config. Working without save logs to file" << std::endl;
        }
    }   catch (...)
    {
        std::cout << "No config file. Starting work with default params (redis = 127.0.0.1:6379, MCP listen on 2020)" << std::endl;
    }

    Log.SetMaxLevel(LogLevel);
    Log.SetLogFolder(LogFolder);

    std::string FirstMessage = " *** GravityRace server started! Log level - ";
    FirstMessage += std::to_string(LogLevel);

    Log.SaveToStorage(GetCurrentDataTimeString()+ FirstMessage);

    GravityRaceServerController srv(GrpPort);

    return 0;
}

