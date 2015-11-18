#ifndef SOLUTIONLOG_H
#define SOLUTIONLOG_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "business/Board.h"
#include "business/AlgoView.h"

class SolutionLog: public wxTextCtrl
{
    public:
        SolutionLog(wxPanel *container);
        virtual ~SolutionLog();

        void generatePuzzle(const Board &board);
        void initLog();
        void logStep(const Board &board, const TCHAR *customText = _(""));
        void logSearch();
        void logCustom(const TCHAR *text);
        void setAlgo(AlgoView *algo) {
            this->algo = algo;
        }
    protected:
    private:
        int step;
        Board puzzle;
        AlgoView *algo;
};

#endif // SOLUTIONLOG_H
