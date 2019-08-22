#ifndef EXTERNALLOGICCONNECTOR_H
#define EXTERNALLOGICCONNECTOR_H

#include <thread>
#include <memory>
#include <functional>
#include <map>
#include <vector>
#include <string>

#include "http/server_http.hpp"
#include "json/json.h"

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

typedef std::function<bool(
        std::vector<std::string>& connected_players)> GetConnectedPlayersCallback;

typedef std::function<std::string(
        const Json::Value &settings)> OnReceiveStartGameCallback;

typedef std::function<bool(
        const std::string& externalId)> StopGameCallback;

typedef std::function<bool(
        std::map<std::string, std::string>& stat)> GetStatCallback;

typedef std::function<bool(
        std::vector<std::string>& list)> GetMatchListCallback;

class ExternalLogicConnector
{
public:
    ExternalLogicConnector(
            GetConnectedPlayersCallback GetConnectedPlayersHandler);
    ~ExternalLogicConnector();

private:
    GetConnectedPlayersCallback m_GetConnectedPlayersCallback;
    OnReceiveStartGameCallback m_OnReceiveStartGameCallback;
    StopGameCallback m_StopGameCallback;
    GetStatCallback m_GetStatCallback;
    GetMatchListCallback m_GetMatchListCallback;    
};

#endif // EXTERNALLOGICCONNECTOR_H
