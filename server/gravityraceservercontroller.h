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


    UserCheckerFuncResult UserConnectionAssigner(
            std::shared_ptr<PlayerConnection> connection,
            const std::string& matchId,
            const std::string& userId,
            const std::string& userPass);

    bool OnReceiveStartGame(
            const Json::Value &team1,
            const Json::Value &team2,
            const Json::Value &settings,
            const std::string &externalId,
            const UserKeyList& userKeys,
            const Json::Value& initialControl);

    void OnGameFinished(std::shared_ptr<NineFootball::Match> match);

    bool StopGame(const std::string& externalId);

    void DeleteMatch(std::shared_ptr<NineFootball::Match> match);


    bool GetStat(std::map<std::string, std::string>& stat) const;
    bool GetMatchList(std::vector<std::string>& list);

    unsigned m_GameCounter;
    std::map<unsigned, std::shared_ptr<NineFootball::Match>> m_RunnigGames;
    std::map<std::string, unsigned> m_ExternIdToLocalId;
    std::mutex m_RunngingGamesSync;
    std::map<unsigned, std::map<std::string, std::string>> m_UsersAuthData;
};

#endif // MATCHSERVERCONTROLLER_H
