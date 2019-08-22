#include "playersconnector.h"

#include "logger.h"

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;


bool comparator_connection_hdl(const websocketpp::connection_hdl& a, const websocketpp::connection_hdl& b)
{
    return (a.lock().get() < b.lock().get());
}


std::string CheckResultToString(PlayerCheckerFuncResult value)
{
    std::string result("invalid auth data");

    switch (value)
    {
        case ucrSucess:
            result = "success";
        break;

        case ucrMatchAccessDenied:
            result = "access denied for match";
        break;

        case ucrMatchNotExist:
            result = "match not exist";
        break;
    }

    return result;
}

bool PlayersConnector::HandleHandshake(
        std::shared_ptr<PlayerConnection> connection,
        const Json::Value& message)
{
    bool result = false;

    if (message.get("method", "").asString() == "handshake")
    {
        Json::Value params =  message.get("parameters", "");
        if (!params.empty())
        {
            std::string userid = params.get("userid", "").asString();
            std::string userpass = params.get("userpass", "").asString();
            std::string matchid = params.get("matchid", "").asString();

            PlayerCheckerFuncResult checkres  = m_UserConnectionAssigner(connection, userid, userpass);

            if (ucrSucess == checkres)
            {
                connection->SetState(PlayerConnection::sHandshakeCompleted);

                m_UserIDs[connection->UserID] = connection->GetHandle();
                result = true;
            }   else
            {
                Log.Info << "PlayersConnector:: if (ucrSucess != checkres) else " << Logger::End;
            }

            std::string handshakeresp("{\"method\":\"handshake\", \"parameters\":{\"response\": \"");
            handshakeresp += CheckResultToString(checkres);
            handshakeresp += "\"}}";
            SendString(connection->GetHandle(), handshakeresp);
        }
    }

    return result;
}

bool PlayersConnector::SendControlResult(
        websocketpp::connection_hdl hdl,
        const std::string& mid,
        const std::string& ctlresult)
{
    std::string result = "{\"method\":\"result\", \"parameters\":{\"result\": " + ctlresult +"}, \"mid\": \"" + mid + "\"}";

    SendString(hdl, result);

    return true;
}

bool PlayersConnector::HandleCommand(
        std::shared_ptr<PlayerConnection> connection,
        const Json::Value& message)
{
    bool result = false;

    if (message.get("method", "").asString() == "control")
    {
        Json::Value params =  message.get("parameters", "");

        std::string ctlres = connection->TeamControl(params);

        if (!ctlres.empty())
        {
            std::string mid = message.get("mid", "").asString();

            if (!mid.empty())
            {
                result = SendControlResult(connection->GetHandle(), mid, ctlres);
            }   else
            {
                result = true;
            }
        }   else
        {
            Log.Notice << "PlayersConnector::HandleCommand empty ctlres" << Logger::End;
        }
    }   else
    {
        Log.Notice << "PlayersConnector::HandleCommand   method != control" << Logger::End;
    }

    return result;
}

bool PlayersConnector::HandleMessage(
        std::shared_ptr<PlayerConnection> connection,
        const Json::Value& message)
{
    bool result = false;

    switch(connection->GetState())
    {
        case PlayerConnection::sConnected:
            result = HandleHandshake(connection, message);
        break;

        case PlayerConnection::sHandshakeCompleted:            
            result = HandleCommand(connection, message);
        break;

        Log.Error << "PlayersConnector::HandleMessage UNKNOWN STATE" << Logger::End;
    }

    return result;
}

void PlayersConnector::OnWebSocketMessage(
        websocketpp::connection_hdl hdl,
        message_ptr msg)
{
    std::shared_ptr<PlayerConnection> conn = m_PlayerConnections[hdl];

    if (conn) try
    {        
        Json::Reader reader;
        Json::Value request;
        reader.parse(msg->get_payload(), request);

        if (!HandleMessage(conn, request))
        {
            Log.Info << "PlayersConnector:: delete connection" << Logger::End;
            m_PlayerConnections.erase(hdl);

            websocketpp::lib::error_code ec;
            m_WebSocketServer.close(hdl, websocketpp::close::status::going_away, "", ec);
        }
    }   catch(const std::exception& e)
    {
        Log.Notice << "PlayersConnector::OnWebSocketMessage EXCEPTION! " << e.what() << Logger::End;
    }
}

void PlayersConnector::OnNewConnection(
        websocketpp::connection_hdl hdl)
{
    std::shared_ptr<PlayerConnection> newConn = std::make_shared<PlayerConnection>(hdl);
    m_PlayerConnections[hdl] = newConn;

    newConn->SetState(PlayerConnection::sConnected);
}

void PlayersConnector::OnCloseConnection(
        websocketpp::connection_hdl hdl)
{
    std::shared_ptr<PlayerConnection> plConn = m_PlayerConnections[hdl];
    if (plConn)
    {
        m_PlayerConnections.erase(hdl);
    }
}

void PlayersConnector::SendString(
        websocketpp::connection_hdl hdl,
        const std::string& value)
{    
    try
    {
        m_WebSocketServer.send(hdl, value, websocketpp::frame::opcode::text);
    }   catch (const std::exception& e)
    {
        Log.Notice << "PlayersConnector::SendString exception " << e.what() << Logger::End;
    }   catch (...)
    {
        Log.Notice << "PlayersConnector::SendString exception unknown" << Logger::End;
    }
}

void PlayersConnector::SendGameState(
        const std::string& userID,
        const std::string& state)
{    
    if (m_UserIDs.count(userID))
    {
        websocketpp::connection_hdl chdl = m_UserIDs[userID];
        if (m_PlayerConnections.count(chdl))
        {
            std::string state_message = "{\"method\": \"gamestate\", \"parameters\": " + state + "}";
            SendString(chdl, state_message);
        }   else
        {
            //std::cout << "PlayersConnector::SendGameState no connection of user" << std::endl;
        }
    }   else
    {
        //std::cout << "PlayersConnector::SendGameState no user" << std::endl;
    }
}

void PlayersConnector::SendEvent(
        const std::string& userID,
        const std::string& event)
{
    if (m_UserIDs.count(userID))
    {
        websocketpp::connection_hdl chdl = m_UserIDs[userID];
        if (m_PlayerConnections.count(chdl))
        {
            std::string event_message = "{\"method\": \"gameevent\", \"parameters\": " + event + "}";
            SendString(chdl, event_message);
        }   else
        {
        }
    }   else
    {
    }
}

PlayersConnector::PlayersConnector(unsigned short listenPort,
        UserConnectionAssigner connectionAssigner)
    : m_PlayerConnections(comparator_connection_hdl),
      m_UserConnectionAssigner(connectionAssigner)
{
    m_WebSocketServer.init_asio();

    m_WebSocketServer.clear_access_channels(websocketpp::log::alevel::all);

    m_WebSocketServer.set_message_handler(bind(&PlayersConnector::OnWebSocketMessage, this,::_1,::_2));
    m_WebSocketServer.set_open_handler(bind(&PlayersConnector::OnNewConnection, this, ::_1));
    m_WebSocketServer.set_close_handler(bind(&PlayersConnector::OnCloseConnection, this, ::_1));

    //---
    m_WebSocketServer.set_reuse_addr(true);
    //---

    m_WebSocketServer.listen(listenPort);

    m_WebSocketServerThread = std::thread([this]()
    {
        m_WebSocketServer.start_accept();
        m_WebSocketServer.run();
    });
}

PlayersConnector::~PlayersConnector()
{
    m_WebSocketServerThread.join();
}
