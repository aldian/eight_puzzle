#ifndef BFSALGO_H
#define BFSALGO_H

#include <map>
#include "gbfs.h"
#include "AlgoView.h"

class BFSAlgoView: public AlgoView
{
    public:
        static BFSAlgoView &getInstance()
        {
            static BFSAlgoView instance;
            return instance;
        }
        virtual ~BFSAlgoView();

        virtual void execute(const Board &board, wxTextCtrl &textCtrl, bool isSearchLogShown);

        const TCHAR *getName() const;

        //int getNIterations();
        void logSearch(wxTextCtrl &textCtrl);
    protected:
    private:
        BFSAlgoView();
        BFSAlgoView(BFSAlgoView const&);              // Don't Implement
        void operator=(BFSAlgoView const&);
        GBFS<Board> gbfssearch;
};

#endif // BFSALGO_H
