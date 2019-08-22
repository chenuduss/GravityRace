#include "gravityraceservercontroller.h"

#include <functional>

#include "json/json.h"

#include "logger.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;
using std::placeholders::_4;
using std::placeholders::_5;
using std::placeholders::_6;

PlayerCheckerFuncResult GravityRaceServerController::UserConnectionAssigner(
        std::shared_ptr<PlayerConnection> connection,
        const std::string& userId,
        const std::string& userPass)
{
    PlayerCheckerFuncResult result = ucrMatchNotExist;

    /*if (m_ExternIdToLocalId.count(matchId))
    {
        unsigned localid = m_ExternIdToLocalId[matchId];

        result = ucrMatchAccessDenied;
        if (m_UsersAuthData.count(localid))
        {
            std::map<std::string, std::string>* users_ad = &m_UsersAuthData[localid];
            if (users_ad->count(userId))
            {
                result = ucrInvalidAuth;
                if ((users_ad->at(userId) == userPass))
                {
                    std::shared_ptr<NineFootball::Match> mm = m_RunnigMatches[localid];
                    connection->TeamControl = std::bind(&NineFootball::Match::TeamControl, mm, userId, _1);
                    connection->UserID = userId;
                    result = ucrSucess;
                }
            }
        }
    }*/

    return result;
}

GravityRaceServerController::GravityRaceServerController(unsigned short webSocketPort)
    : m_ExternalLogic(std::bind(&GravityRaceServerController::GetConnectedPlayers, this, _1)),
      m_PlayersConnector(std::make_shared<PlayersConnector>(webSocketPort, std::bind(&GravityRaceServerController::UserConnectionAssigner, this, _1, _2, _3)))

{

}

GravityRaceServerController::~GravityRaceServerController()
{
    //std::cout << "MatchServerController::~MatchServerController" << std::endl;
}

bool GravityRaceServerController::GetStat(
        std::map<std::string, std::string>& stat) const
{
    stat["match_total"] = std::to_string(m_Stat.TotalGames);
    stat["match_finished"] = std::to_string(m_Stat.GamesFinished);

    return true;
}

bool GravityRaceServerController::GetMatchList(
        std::vector<std::string>& list)
{
    std::unique_lock<std::mutex> locker(m_RunngingGamesSync);

    for (const auto& item: m_ExternIdToLocalId)
    {
        list.push_back(item.first);
    }

    return true;
}

bool GravityRaceServerController::GetConnectedPlayers(
        std::vector<std::string>& connected_players)
{
    return true;
}

/*bool GravityRaceServerController::StopGame(
        const std::string& externalId)
{
    std::unique_lock<std::mutex> locker(m_RunngingGamesSync);
    if (m_ExternIdToLocalId.count(externalId))
    {
        std::shared_ptr<NineFootball::Match> match = m_RunnigGames[m_ExternIdToLocalId[externalId]];
        match->InitiateStop();
    }   else
    {
        Log.Info << "MatchServerController::StopGame stoping game initiated for invalid MatchID" << Logger::End;
    }

    return true;
}*/

/*void GravityRaceServerController::DeleteMatch(
        std::shared_ptr<NineFootball::Match> match)
{
    m_ExternIdToLocalId.erase(match->GetExternalId());
    m_RunnigGames.erase(match->GetId());
    m_UsersAuthData.erase(match->GetId());
}*/

/*void GravityRaceServerController::OnGameFinished(
        std::shared_ptr<NineFootball::Match> match)
{
    ++m_Stat.MatchesFinished;

    Log.Debug << "MatchServerController::OnGameFinished" << Logger::End;

    m_ExternalLogic.ReportMatchResults(
                match->GetExternalId(),
                match->GetRecordJSON(),
                match->GetEventsJSON(),
                match->GetResultJSON(),
                match->GetOutTeams());

    Log.Debug << "MatchServerController::OnGameFinished results saved!" << Logger::End;

    std::unique_lock<std::mutex> locker(m_RunngingGamesSync);
    DeleteMatch(match);
}*/

