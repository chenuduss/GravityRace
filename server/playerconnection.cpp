#include "playerconnection.h"

PlayerConnection::PlayerConnection(websocketpp::connection_hdl hdl)
    :m_Handle(hdl)
{
}

PlayerConnection::~PlayerConnection()
{

}

