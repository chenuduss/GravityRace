#ifndef MATCHSERVERCONTROLLER_H
#define MATCHSERVERCONTROLLER_H


#include "externallogicconnector.h"
#include "playersconnector.h"

#include <memory>
#include <map>
#include <mutex>
#include <string>

#include "GravityRaceGame.h"

class GravityRaceServerStatistics
{
public:
    GravityRaceServerStatistics()
        : TotalGames(0), GamesFinished(0)
    {}
    ~GravityRaceServerStatistics() {}

    unsigned TotalGames;
    unsigned GamesFinished;
};

class GravityRaceServerController
{
public:
    GravityRaceServerController(
        unsigned short webSocketPort);

    ~GravityRaceServerController();

protected:
    GravityRaceServerStatistics m_Stat;

private:
    ExternalLogicConnector m_ExternalLogic;
    std::shared_ptr<PlayersConnector> m_PlayersConnector;

    bool GetConnectedPlayers(
            std::vector<std::string>& connected_players);

    PlayerCheckerFuncResult UserConnectionAssigner(
            std::shared_ptr<PlayerConnection> connection,
            const std::string& userId,
            const std::string& userPass);

    /*bool OnReceiveStartGame(
            const std::vector<Json::Value> &teams,
            const Json::Value &settings,
            const std::string &externalId,
            const UserKeyList& userKeys);

    void OnGameFinished(std::shared_ptr<NineFootball::Match> match);

    bool StopGame(const std::string& externalId);

    void DeleteMatch(std::shared_ptr<NineFootball::Match> match);*/


    bool GetStat(std::map<std::string, std::string>& stat) const;
    bool GetMatchList(std::vector<std::string>& list);

    unsigned m_GameCounter;
    std::map<unsigned, std::shared_ptr<GravityRace::Game>> m_RunnigGames;
    std::map<std::string, unsigned> m_ExternIdToLocalId;
    std::mutex m_RunngingGamesSync;
    std::map<unsigned, std::map<std::string, std::string>> m_UsersAuthData;
};

#endif // MATCHSERVERCONTROLLER_H
