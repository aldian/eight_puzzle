#include "BFSAlgoView.h"

BFSAlgoView::BFSAlgoView()
{
    //ctor
}

BFSAlgoView::~BFSAlgoView()
{
    //dtor
}

const TCHAR *BFSAlgoView::getName() const {
    static const TCHAR *name = TEXT("Greedy BFS");
    return name;
}

void BFSAlgoView::execute(const Board &board, wxTextCtrl &textCtrl, bool isSearchLogShown)
{
    logStr = _T("");
    nIterations = 0;
    solutionMap.clear();
    searchLog.clear();
    closedLog.clear();
    fLog.clear();
    gLog.clear();
    hLog.clear();
    cfLog.clear();
    cgLog.clear();
    chLog.clear();
    Board nodeStart = board;
    Board nodeEnd = Board();
    gbfssearch.SetStartAndGoalStates( nodeStart, nodeEnd );
    unsigned int searchState;
    unsigned int searchSteps = 0;
    do
    {
        ++nIterations;
        vector<Board> searchSteps, closedSteps;
        vector<float> fSteps, cfSteps;
        vector<float> gSteps, cgSteps;
        vector<float> hSteps, chSteps;
        vector< GBFS<Board>::Node * >::iterator iter;
        for (iter = gbfssearch.openList.begin(); iter != gbfssearch.openList.end(); ++iter) {
            GBFS<Board>::Node *node = (*iter);
            searchSteps.push_back(node->board);
            fSteps.push_back(node->f);
            gSteps.push_back(node->g);
            hSteps.push_back(node->h);
        }
        if (searchSteps.size() > 0) {
            addToSearchLog(searchSteps);
            addToFLog(fSteps);
            addToGLog(gSteps);
            addToHLog(hSteps);
        }
        for (iter = gbfssearch.closedList.begin(); iter != gbfssearch.closedList.end(); ++iter) {
            GBFS<Board>::Node *node = (*iter);
            closedSteps.push_back(node->board);
            cfSteps.push_back(node->f);
            cgSteps.push_back(node->g);
            chSteps.push_back(node->h);
        }
        if (closedSteps.size() > 0) {
            addToClosedLog(closedSteps);
            addToCFLog(cfSteps);
            addToCGLog(cgSteps);
            addToCHLog(chSteps);
        }
        searchState = gbfssearch.SearchStep();
        if (searchState == GBFS<Board>::SEARCH_STATE_SUCCEEDED) {
            gbfssearch.FreeUnusedNodes();
        }
    }
    while( searchState == GBFS<Board>::SEARCH_STATE_SEARCHING );

    Board *prevNode = NULL;
    if( searchState == GBFS<Board>::SEARCH_STATE_SUCCEEDED )
    {
        Board *node = gbfssearch.GetSolutionStart();
        int steps = 0;
        for( ;; )
        {
            if (prevNode != NULL) {
                solutionMap[*prevNode] = *node;
            }
            prevNode = node;
            node = gbfssearch.GetSolutionNext();
            if( !node )
            {
                break;
            }
            steps ++;
        };
        gbfssearch.FreeSolutionNodes();
    }
}

void BFSAlgoView::logSearch(wxTextCtrl &textCtrl)
{
    wxString stepStr;
    stepStr += _T("\nSearch steps:\n");
    //WriteText(_T("\n"));
    vector<vector<Board> >::iterator searchLogIter, closedLogIter;
    vector<vector<float> >::iterator fIter, cfIter;
    vector<vector<float> >::iterator gIter, cgIter;
    vector<vector<float> >::iterator hIter, chIter;
    this->closedLog.insert(this->closedLog.begin(), vector<Board>());
    this->cfLog.insert(this->cfLog.begin(), vector<float>());
    this->cgLog.insert(this->cgLog.begin(), vector<float>());
    this->chLog.insert(this->chLog.begin(), vector<float>());
    int j;
    for (j = 1, searchLogIter = this->searchLog.begin(),
         closedLogIter = this->closedLog.begin(),
         fIter = this->fLog.begin(), cfIter = this->cfLog.begin(),
         gIter = this->gLog.begin(), cgIter = this->cgLog.begin(),
         hIter = this->hLog.begin(), chIter = this->chLog.begin();
         searchLogIter != this->searchLog.end();
         ++searchLogIter, ++closedLogIter, ++fIter, ++gIter, ++hIter, ++cfIter, ++cgIter, ++chIter, ++j)
    {
        vector<Board> searchStep = *searchLogIter;
        vector<Board>::iterator iter2;
        vector<float> fStep = *fIter;
        vector<float>::iterator fIter2;
        vector<float> gStep = *gIter;
        vector<float>::iterator gIter2;
        vector<float> hStep = *hIter;
        vector<float>::iterator hIter2;

        int nVisible = 10; //114;
        int i;
        stepStr += wxString::Format(_T("%d. Open list:\n"), j);
        for (i = 0, iter2 = searchStep.begin(); (iter2 != searchStep.end()) && (i < nVisible); ++iter2, ++i) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[0], puzzle[1], puzzle[2]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[0], puzzle[1], puzzle[2]));
        }
        if (i == nVisible) {
            stepStr += _T("...");
        }
        stepStr += _T("\n");
        //WriteText(_T("\n"));
        for (i = 0, iter2 = searchStep.begin(); (iter2 != searchStep.end()) && (i < nVisible); ++iter2, ++i) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[3], puzzle[4], puzzle[5]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[3], puzzle[4], puzzle[5]));
        }
        if (i == nVisible) {
            stepStr += _T("...");
        }
        stepStr += _T("\n");
        //WriteText(_T("\n"));
        for (i = 0, iter2 = searchStep.begin(); (iter2 != searchStep.end()) && (i < nVisible); ++iter2, ++i) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[6], puzzle[7], puzzle[8]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[6], puzzle[7], puzzle[8]));
        }
        if (i == nVisible) {
            stepStr += _T("...");
        }
        stepStr += _T("\n");

        for (i = 0, fIter2 = fStep.begin(); (fIter2 != fStep.end()) && (i < nVisible); ++fIter2, ++i)
        {
            if (*fIter2 < 10.0) {
                stepStr += wxString::Format(_T("f: %0.0f  "), (*fIter2));
            } else {
                stepStr += wxString::Format(_T("f: %0.0f "), (*fIter2));
            }

        }
        if (i == nVisible) {
            stepStr += _T("...");
        }
        stepStr += _T("\n");
        for (i = 0, gIter2 = gStep.begin(); (gIter2 != gStep.end()) && (i < nVisible); ++gIter2, ++i)
        {
            if (*gIter2 < 10.0) {
                stepStr += wxString::Format(_T("g: %0.0f  "), (*gIter2));
            } else {
                stepStr += wxString::Format(_T("g: %0.0f "), (*gIter2));
            }
        }
        if (i == nVisible) {
            stepStr += _T("...");
        }
        stepStr += _T("\n");
        for (i = 0, hIter2 = hStep.begin(); (hIter2 != hStep.end()) && (i < nVisible); ++hIter2, ++i)
        {
            if (*hIter2 < 10.0) {
                stepStr += wxString::Format(_T("h: %0.0f  "), (*hIter2));
            } else {
                stepStr += wxString::Format(_T("h: %0.0f "), (*hIter2));
            }
        }
        if (i == nVisible) {
            stepStr += _T("...");
        }
        stepStr += _T("\n\n");
        if (stepStr.size() >= 30000) {
            textCtrl.WriteText(stepStr);
            stepStr = _T("");
        }

        searchStep = *closedLogIter;
        fStep = *cfIter;
        gStep = *cgIter;
        hStep = *chIter;

        stepStr += wxString::Format(_T("%d. Closed list:\n"), j);
        for (i = 0, iter2 = searchStep.begin(); (iter2 != searchStep.end()) && (i < nVisible); ++iter2, ++i) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[0], puzzle[1], puzzle[2]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[0], puzzle[1], puzzle[2]));
        }
        if (i == nVisible) {
            stepStr += _T("...");
        }
        stepStr += _T("\n");
        //WriteText(_T("\n"));
        for (i = 0, iter2 = searchStep.begin(); (iter2 != searchStep.end()) && (i < nVisible); ++iter2, ++i) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[3], puzzle[4], puzzle[5]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[3], puzzle[4], puzzle[5]));
        }
        if (i == nVisible) {
            stepStr += _T("...");
        }
        stepStr += _T("\n");
        //WriteText(_T("\n"));
        for (i = 0, iter2 = searchStep.begin(); (iter2 != searchStep.end()) && (i < nVisible); ++iter2, ++i) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[6], puzzle[7], puzzle[8]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[6], puzzle[7], puzzle[8]));
        }
        if (i == nVisible) {
            stepStr += _T("...");
        }
        stepStr += _T("\n");

        for (i = 0, fIter2 = fStep.begin(); (fIter2 != fStep.end()) && (i < nVisible); ++fIter2, ++i)
        {
            if (*fIter2 < 10.0) {
                stepStr += wxString::Format(_T("f: %0.0f  "), (*fIter2));
            } else {
                stepStr += wxString::Format(_T("f: %0.0f "), (*fIter2));
            }
        }
        if (i == nVisible) {
            stepStr += _T("...");
        }
        stepStr += _T("\n");
        for (i = 0, gIter2 = gStep.begin(); (gIter2 != gStep.end()) && (i < nVisible); ++gIter2, ++i)
        {
            if (*gIter2 < 10.0) {
                stepStr += wxString::Format(_T("g: %0.0f  "), (*gIter2));
            } else {
                stepStr += wxString::Format(_T("g: %0.0f "), (*gIter2));
            }
        }
        if (i == nVisible) {
            stepStr += _T("...");
        }
        stepStr += _T("\n");
        for (i = 0, hIter2 = hStep.begin(); (hIter2 != hStep.end()) && (i < nVisible); ++hIter2, ++i)
        {
            if (*hIter2 < 10.0) {
                stepStr += wxString::Format(_T("h: %0.0f  "), (*hIter2));
            } else {
                stepStr += wxString::Format(_T("h: %0.0f "), (*hIter2));
            }
        }
        if (i == nVisible) {
            stepStr += _T("...");
        }

        stepStr += _T("\n\n");
        if (stepStr.size() >= 30000) {
            textCtrl.WriteText(stepStr);
            stepStr = _T("");
        }
    }

    textCtrl.WriteText(stepStr);
}
//int BFSAlgoView::getNIterations() {
//    return gbfssearch.GetStepCount();
//}
