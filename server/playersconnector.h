#ifndef PLAYERSCONNECTOR_H
#define PLAYERSCONNECTOR_H


#include "websocketpp/config/asio.hpp"
#include "websocketpp/server.hpp"

#include <thread>
#include <map>
#include <memory>
#include <functional>

#include "playerconnection.h"

#include "json/json.h"

typedef websocketpp::server<websocketpp::config::asio> WebSocketServer;

typedef websocketpp::config::asio::message_type::ptr message_ptr;
typedef websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context> context_ptr;

enum PlayerCheckerFuncResult {
    ucrSucess = 0,
    ucrInvalidAuth,
    ucrMatchAccessDenied,
    ucrMatchNotExist
};

typedef std::function<PlayerCheckerFuncResult (
        std::shared_ptr<PlayerConnection> connection,
        const std::string& userId,
        const std::string& userPass)> UserConnectionAssigner;

class PlayersConnector
{
public:
    PlayersConnector(
            unsigned short listenPort,
            UserConnectionAssigner connectionAssigner);
    ~PlayersConnector();


    void SendGameState(
            const std::string& userID,
            const std::string &state);

    void SendEvent(
            const std::string& userID,
            const std::string &event);

private:
    bool HandleMessage(
            std::shared_ptr<PlayerConnection> connection,
            const Json::Value& message);

    bool HandleHandshake(
            std::shared_ptr<PlayerConnection> connection,
            const Json::Value& message);

    bool HandleCommand(
            std::shared_ptr<PlayerConnection> connection,
            const Json::Value& message);

    void OnWebSocketMessage(
            websocketpp::connection_hdl hdl,
            message_ptr msg);

    void OnNewConnection(
            websocketpp::connection_hdl hdl);

    void OnCloseConnection(
            websocketpp::connection_hdl hdl);

    void SendString(
            websocketpp::connection_hdl hdl,
            const std::string& value);

    bool SendControlResult(
            websocketpp::connection_hdl hdl,
            const std::string& mid,
            const std::string& ctlresult);

    WebSocketServer m_WebSocketServer;
    std::thread m_WebSocketServerThread;

    UserConnectionAssigner m_UserConnectionAssigner;

    std::map<std::string, websocketpp::connection_hdl> m_UserIDs;
    std::map<websocketpp::connection_hdl, std::shared_ptr<PlayerConnection>, bool (*)(const websocketpp::connection_hdl& a, const websocketpp::connection_hdl& b)> m_PlayerConnections;
};

#endif // PLAYERSCONNECTOR_H
