#ifndef ASTARALGO_H
#define ASTARALGO_H

#include <map>
#include "astar.h"
#include "AlgoView.h"

class AStarAlgoView: public AlgoView
{
    public:
        static AStarAlgoView &getInstance()
        {
            static AStarAlgoView instance;
            return instance;
        }
        virtual ~AStarAlgoView();

        virtual void execute(const Board &board, wxTextCtrl &textCtrl, bool isSearchLogShown);

        const TCHAR *getName() const;

        //int getNIterations();
        void logSearch(wxTextCtrl &textCtrl);
    protected:
    private:
        AStarAlgoView();
        AStarAlgoView(AStarAlgoView const&);              // Don't Implement
        void operator=(AStarAlgoView const&);
        AStar<Board> astarsearch;
};

#endif // ASTARALGO_H
