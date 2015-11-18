#ifndef ASTAR_H
#define ASTAR_H

#include <algorithm>
#include <set>
#include <vector>
#include "search.h"

using namespace std;

template <class TBoard> class AStar: public Search<TBoard>
{
private:
    void initStartNode()
    {
		Search<TBoard>::start->g = 0;
		Search<TBoard>::start->h = Search<TBoard>::start->board.GoalDistanceEstimate( Search<TBoard>::goal->board );
		Search<TBoard>::start->f = Search<TBoard>::start->g + Search<TBoard>::start->h;
		Search<TBoard>::start->parent = 0;
    }

    void initSuccessorNode(typename Search<TBoard>::Node *successor, typename Search<TBoard>::Node *parent, unsigned int newg)
    {
        successor->parent = parent;
        successor->g = newg;
        successor->h = successor->board.GoalDistanceEstimate( Search<TBoard>::goal->board );
        successor->f = successor->g + successor->h;
    }
};

#endif
