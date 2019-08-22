#ifndef PLAYERCONNECTION_H
#define PLAYERCONNECTION_H

#include "websocketpp/config/asio.hpp"
#include "websocketpp/server.hpp"

#include <functional>
#include <string>

#include "json/json.h"

typedef std::function<std::string(
        const Json::Value& params)> OnTeamControlCallback;

class PlayerConnection
{
public:
    PlayerConnection(
             websocketpp::connection_hdl hdl);
    ~PlayerConnection();

    enum State {
        sNA = 0,
        sConnected,
        sHandshakeCompleted,
        sDisconnected
    };

    websocketpp::connection_hdl GetHandle() const noexcept
    {
        return m_Handle;
    }

    State GetState() const noexcept
    {
        return m_State;
    }

    void SetState(State value) noexcept
    {
        m_State = value;
    }

    OnTeamControlCallback TeamControl;
    std::string UserID;

private:
    State m_State;
    websocketpp::connection_hdl m_Handle;
};

#endif // PLAYERCONNECTION_H
