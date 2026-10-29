#pragma once

#include <vector>
#include <map>
#include <string>

//GameAction to represent an action taken by a player (e.g. in TicTacTow, place marker X in row 1, col 2, or pick an AIAction/AIOption)
struct GameAction
{
    // defines an Action for the game

public:

    
    int actionID = 0;

    // add data that comprises an action 

    GameAction()
    {

    }

    

};




/*
WorldState stores the games current state for the GOAP/MCTS planner
State is currently recorded a Key Value Pairs using std::map, with a boolean value
*/

class WorldState
{
public:

    // data for the world state - this must be customised for game type
    // does not need to be a map, could be any data structure

    std::map<std::string, bool> state;


    void SetAndApplyAction(GameAction _action)
    {
        // TODO apply the action to the game state, changing the game state based on the action


    }
};


