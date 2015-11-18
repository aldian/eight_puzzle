#ifndef ALGO_H
#define ALGO_H

#include <vector>
#include <map>
using namespace std;
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif
#include "Board.h"

class AlgoView
{
    public:
        AlgoView();
        virtual ~AlgoView();

        virtual void execute(const Board &board, wxTextCtrl &wxTextCtrl, bool isSearchLogShown) = 0;
        void addToSearchLog(const vector<Board> &searchSteps);
        void addToClosedLog(const vector<Board> &closedSteps);
        void addToFLog(const vector<float> &fSteps);
        void addToGLog(const vector<float> &gSteps);
        void addToHLog(const vector<float> &hSteps);
        void addToCFLog(const vector<float> &cfSteps);
        void addToCGLog(const vector<float> &cgSteps);
        void addToCHLog(const vector<float> &chSteps);

        virtual Board nextMove(const Board &board, int &direction, int &movingItemIndex);

        virtual void logSearch(wxTextCtrl &textCtrl) = 0;

        virtual const TCHAR *getName() const = 0;

        int getNSteps() const
        {
            return solutionMap.size();
        }

        int getNIterations() const
        {
            return nIterations;
        }

        vector<vector<Board> > searchLog;
        vector<vector<Board> > closedLog;

        vector<vector<float> > fLog;
        vector<vector<float> > gLog;
        vector<vector<float> > hLog;

        vector<vector<float> > cfLog;
        vector<vector<float> > cgLog;
        vector<vector<float> > chLog;

        wxString logStr;
    protected:
        std::map<Board, Board, Board> solutionMap;
        int nIterations;
        int nListItemVisible;
};

#endif // ALGO_H
