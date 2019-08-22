#ifndef MATCH_H
#define MATCH_H


#include "playersconnector.h"

#include <thread>
#include <chrono>
#include <memory>
#include <vector>
#include <functional>
#include <random>
#include <atomic>
#include <string>
#include <utility>
#include <mutex>

#include "json/json.h"

namespace GravityRace
{
class Game
{
public:    
    enum State {
        sNA = 0,
        sInitialized,        
    };

    Game();
    ~Game();

private:
};

}

#endif // MATCH_H



