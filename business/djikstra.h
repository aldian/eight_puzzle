#ifndef DJIKSTRA_H
#define DJIKSTRA_H

#include <algorithm>
#include <set>
#include <vector>
#include "search.h"

using namespace std;

template <class TBoard> class Djikstra: public Search<TBoard>
{
private:
    void initStartNode()
    {
		Search<TBoard>::start->g = 0;
		Search<TBoard>::start->h = 0; //start->board.GoalDistanceEstimate( goal->board );
		Search<TBoard>::start->f = Search<TBoard>::start->g + Search<TBoard>::start->h;
		Search<TBoard>::start->parent = 0;
    }

    void initSuccessorNode(typename Search<TBoard>::Node *successor, typename Search<TBoard>::Node *parent, unsigned int newg)
    {
        successor->parent = parent;
        successor->g = newg;
        successor->h = 0;
        successor->f = successor->g + successor->h;
    }

	void initOpenList()
	{
	    // override to avoid unnecessary sorting
	}

	void removeOpenListHead()
	{
	    Search<TBoard>::openList.erase(Search<TBoard>::openList.begin());
	}

	void prepareOpenListNodeReplacement()
	{
	    // override to avoid unnecessary sorting
	}

	void insertNodeToOpenList(typename Search<TBoard>::Node *node)
	{
	    if (node->board.IsGoal(this->goal->board)) {
	        Search<TBoard>::openList.insert(Search<TBoard>::openList.begin(), node);
	    } else {
	        Search<TBoard>::openList.push_back(node);
	    }
	}

	void afterOpenListNodeInsertion()
	{
	    // override to avoid unnecessary sorting
	}

};

#endif // DJIKSTRA_H
