#include "externallogicconnector.h"
#include "zlibaux.h"

#include <functional>

#include "logger.h"

using std::placeholders::_1;
using std::placeholders::_2;

ExternalLogicConnector::ExternalLogicConnector(
        GetConnectedPlayersCallback GetConnectedPlayersHandler)
    : m_GetConnectedPlayersCallback(GetConnectedPlayersHandler)
{

}


void StringReplace(std::string& str, const std::string& oldStr, const std::string& newStr)
{
    size_t pos = 0;
    while((pos = str.find(oldStr, pos)) != std::string::npos)
    {
        str.replace(pos, oldStr.length(), newStr);
        pos += newStr.length();
    }
}

ExternalLogicConnector::~ExternalLogicConnector()
{
}
