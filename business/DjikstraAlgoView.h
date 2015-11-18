#ifndef DJIKSTRAALGO_H
#define DJIKSTRAALGO_H

#include <map>
#include "djikstra.h"
#include "AlgoView.h"

class DjikstraAlgoView: public AlgoView
{
    public:
        static DjikstraAlgoView &getInstance()
        {
            static DjikstraAlgoView instance;
            return instance;
        }
        virtual ~DjikstraAlgoView();

        virtual void execute(const Board &board, wxTextCtrl &textCtrl, bool isSearchLogShown);

        const TCHAR *getName() const;

        //int getNIterations();
        void logSearch(wxTextCtrl &textCtrl);
    protected:
    private:
        DjikstraAlgoView();
        DjikstraAlgoView(DjikstraAlgoView const&);              // Don't Implement
        void operator=(DjikstraAlgoView const&);
        Djikstra<Board> djikstrasearch;
};


#endif // DJIKSTRAALGO_H
