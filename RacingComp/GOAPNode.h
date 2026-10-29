#pragma once

/*
CMP304/MAT501 AI Framework (2026)
*/

#include <vector>
#include <string>
#include "../AI_Framework/framework.h"
#include "WorldState.h"



/*
GOAPNode contains the data for the A* pathfinding
*/
class GOAPNode
{
public:
    GOAPNode();

    GOAPNode(WorldState _ws, int _g, int _h, int _id, int _parentID, AIOptionBase* _option);

    // ID's for the Node
     int id;
     int parentID;

    //Pathfinding vars
     int g; // The A* cost from 'start' to 'here'
     int h; // The estimated remaining cost to 'goal' form 'here'

    // This Node's world state and Option that was applied to reach this game state
     WorldState ws;
     AIOptionBase* option;

     int f(); // g + h

     bool IsShorterThan(const GOAPNode _other) const;

};

