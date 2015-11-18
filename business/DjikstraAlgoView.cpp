#include "DjikstraAlgoView.h"

DjikstraAlgoView::DjikstraAlgoView()
{
    //ctor
}

DjikstraAlgoView::~DjikstraAlgoView()
{
    //dtor
}

const TCHAR *DjikstraAlgoView::getName() const {
    static const TCHAR *name = TEXT("Djikstra");
    return name;
}

void DjikstraAlgoView::execute(const Board &board, wxTextCtrl &textCtrl, bool isSearchLogShown)
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
    djikstrasearch.SetStartAndGoalStates( nodeStart, nodeEnd );
    unsigned int searchState;
    unsigned int searchSteps = 0;

    //wxString stepStr;
    if (isSearchLogShown) {
        //textCtrl.WriteText(_T("\nSearch steps:\n"));
        logStr += _T("\nSearch steps:\n");
    }

    do
    {
        ++nIterations;

        vector<Board> searchSteps, closedSteps;
        vector<float> fSteps, cfSteps;
        vector<float> gSteps, cgSteps;
        vector<float> hSteps, chSteps;
        vector< Djikstra<Board>::Node * >::iterator iter;
        int i;
        for (i = 0, iter = djikstrasearch.openList.begin(); (iter != djikstrasearch.openList.end()) && (i < nListItemVisible); ++iter, ++i) {
            Djikstra<Board>::Node *node = (*iter);
            searchSteps.push_back(node->board);
            fSteps.push_back(node->f);
            gSteps.push_back(node->g);
            hSteps.push_back(node->h);
        }
        if (isSearchLogShown) {
            vector<Board>::iterator iter2;
            logStr += wxString::Format(_T("\n%d. Open list:\n"), nIterations);
            for (iter2 = searchSteps.begin(); iter2 != searchSteps.end(); ++iter2) {
                    Board board = *iter2;
                    int *puzzle = board.getPuzzle();
                    logStr += wxString::Format(_T("%d%d%d   "), puzzle[0], puzzle[1], puzzle[2]);
                    //WriteText(wxString::Format(_T("%d%d%d "), puzzle[0], puzzle[1], puzzle[2]));
            }
            if (i == nListItemVisible) {
                logStr += _T("...");
            }
            logStr += _T("\n");

            for (iter2 = searchSteps.begin(); iter2 != searchSteps.end(); ++iter2) {
                    Board board = *iter2;
                    int *puzzle = board.getPuzzle();
                    logStr += wxString::Format(_T("%d%d%d   "), puzzle[3], puzzle[4], puzzle[5]);
                    //WriteText(wxString::Format(_T("%d%d%d "), puzzle[3], puzzle[4], puzzle[5]));
            }
            if (i == nListItemVisible) {
                logStr += _T("...");
            }
            logStr += _T("\n");
            //WriteText(_T("\n"));
            for (iter2 = searchSteps.begin(); iter2 != searchSteps.end(); ++iter2) {
                    Board board = *iter2;
                    int *puzzle = board.getPuzzle();
                    logStr += wxString::Format(_T("%d%d%d   "), puzzle[6], puzzle[7], puzzle[8]);
                    //WriteText(wxString::Format(_T("%d%d%d "), puzzle[6], puzzle[7], puzzle[8]));
            }
            if (i == nListItemVisible) {
                logStr += _T("...");
            }
            logStr += _T("\n");

            vector<float>::iterator fIter2, gIter2, hIter2;

            for (fIter2 = fSteps.begin(); fIter2 != fSteps.end(); ++fIter2)
            {
                if (*fIter2 < 10.0) {
                    logStr += wxString::Format(_T("f: %0.0f  "), (*fIter2));
                } else {
                    logStr += wxString::Format(_T("f: %0.0f "), (*fIter2));
                }
            }
            if (i == nListItemVisible) {
                logStr += _T("...");
            }
            logStr += _T("\n");
            for (gIter2 = gSteps.begin(); gIter2 != gSteps.end(); ++gIter2)
            {
                if (*gIter2 < 10.0) {
                    logStr += wxString::Format(_T("g: %0.0f  "), (*gIter2));
                } else {
                    logStr += wxString::Format(_T("g: %0.0f "), (*gIter2));
                }
            }
            if (i == nListItemVisible) {
                logStr += _T("...");
            }
            logStr += _T("\n");
            for (hIter2 = hSteps.begin(); hIter2 != hSteps.end(); ++hIter2)
            {
                if (*hIter2 < 10.0) {
                    logStr += wxString::Format(_T("h: %0.0f  "), (*hIter2));
                } else {
                    logStr += wxString::Format(_T("h: %0.0f "), (*hIter2));
                }
            }
            if (i == nListItemVisible) {
                logStr += _T("...");
            }
            logStr += _T("\n");
            //textCtrl.WriteText(stepStr);
        }

        /*
        if (searchSteps.size() > 0) {
            addToSearchLog(searchSteps);
            addToFLog(fSteps);
            addToGLog(gSteps);
            addToHLog(hSteps);
        }
        */
        /*
        for (i = 0, iter = djikstrasearch.closedList.begin(); (iter != djikstrasearch.closedList.end()) && (i < nListItemVisible); ++iter, ++i) {
            Djikstra<Board>::Node *node = (*iter);
            closedSteps.push_back(node->board);
            cfSteps.push_back(node->f);
            cgSteps.push_back(node->g);
            chSteps.push_back(node->h);
        }
        if (isSearchLogShown) {
            vector<Board>::iterator iter2;
            vector<float>::iterator fIter2, gIter2, hIter2;
            stepStr = wxString::Format(_T("\n%d. Closed list:\n"), nIterations);
            for (iter2 = searchSteps.begin(); iter2 != searchSteps.end(); ++iter2) {
                    Board board = *iter2;
                    int *puzzle = board.getPuzzle();
                    stepStr += wxString::Format(_T("%d%d%d   "), puzzle[0], puzzle[1], puzzle[2]);
                    //WriteText(wxString::Format(_T("%d%d%d "), puzzle[0], puzzle[1], puzzle[2]));
            }
            stepStr += _T("\n");
            //WriteText(_T("\n"));
            for (iter2 = searchSteps.begin(); iter2 != searchSteps.end(); ++iter2) {
                    Board board = *iter2;
                    int *puzzle = board.getPuzzle();
                    stepStr += wxString::Format(_T("%d%d%d   "), puzzle[3], puzzle[4], puzzle[5]);
                    //WriteText(wxString::Format(_T("%d%d%d "), puzzle[3], puzzle[4], puzzle[5]));
            }
            stepStr += _T("\n");
            //WriteText(_T("\n"));
            for (iter2 = searchSteps.begin(); iter2 != searchSteps.end(); ++iter2) {
                    Board board = *iter2;
                    int *puzzle = board.getPuzzle();
                    stepStr += wxString::Format(_T("%d%d%d   "), puzzle[6], puzzle[7], puzzle[8]);
                    //WriteText(wxString::Format(_T("%d%d%d "), puzzle[6], puzzle[7], puzzle[8]));
            }
            stepStr += _T("\n");

            for (fIter2 = fSteps.begin(); fIter2 != fSteps.end(); ++fIter2)
            {
                if (*fIter2 < 10.0) {
                    stepStr += wxString::Format(_T("f: %0.0f  "), (*fIter2));
                } else {
                    stepStr += wxString::Format(_T("f: %0.0f "), (*fIter2));
                }

            }
            stepStr += _T("\n");
            for (gIter2 = gSteps.begin(); gIter2 != gSteps.end(); ++gIter2)
            {
                if (*gIter2 < 10.0) {
                    stepStr += wxString::Format(_T("g: %0.0f  "), (*gIter2));
                } else {
                    stepStr += wxString::Format(_T("g: %0.0f "), (*gIter2));
                }
            }
            stepStr += _T("\n");
            for (hIter2 = hSteps.begin(); hIter2 != hSteps.end(); ++hIter2)
            {
                if (*hIter2 < 10.0) {
                    stepStr += wxString::Format(_T("h: %0.0f  "), (*hIter2));
                } else {
                    stepStr += wxString::Format(_T("h: %0.0f "), (*hIter2));
                }
            }
            stepStr += _T("\n");

            textCtrl.WriteText(stepStr);
        }
        */
        /*
        if (closedSteps.size() > 0) {
            addToClosedLog(closedSteps);
            addToCFLog(cfSteps);
            addToCGLog(cgSteps);
            addToCHLog(chSteps);
        }
        */

        searchState = djikstrasearch.SearchStep();
        if (searchState == Djikstra<Board>::SEARCH_STATE_SUCCEEDED) {
            djikstrasearch.FreeUnusedNodes();
        }
        wxApp::GetInstance()->Yield();
    }
    while( searchState == Djikstra<Board>::SEARCH_STATE_SEARCHING );

    Board *prevNode = NULL;
    if( searchState == Djikstra<Board>::SEARCH_STATE_SUCCEEDED )
    {
        Board *node = djikstrasearch.GetSolutionStart();
        int steps = 0;
        for( ;; )
        {
            if (prevNode != NULL) {
                solutionMap[*prevNode] = *node;
            }
            prevNode = node;
            node = djikstrasearch.GetSolutionNext();
            if( !node )
            {
                break;
            }
            steps ++;
        };
        djikstrasearch.FreeSolutionNodes();
    }
}

void DjikstraAlgoView::logSearch(wxTextCtrl &textCtrl)
{
    /*
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
    int i;
    for (i = 1, searchLogIter = this->searchLog.begin(),
         closedLogIter = this->closedLog.begin(),
         fIter = this->fLog.begin(), cfIter = this->cfLog.begin(),
         gIter = this->gLog.begin(), cgIter = this->cgLog.begin(),
         hIter = this->hLog.begin(), chIter = this->chLog.begin();
         searchLogIter != this->searchLog.end();
         ++searchLogIter, ++closedLogIter, ++fIter, ++gIter, ++hIter, ++cfIter, ++cgIter, ++chIter, ++i)
    {
        vector<Board> searchStep = *searchLogIter;
        vector<Board>::iterator iter2;
        vector<float> fStep = *fIter;
        vector<float>::iterator fIter2;
        vector<float> gStep = *gIter;
        vector<float>::iterator gIter2;
        vector<float> hStep = *hIter;
        vector<float>::iterator hIter2;

        stepStr += wxString::Format(_T("%d. Open list:\n"), i);
        for (iter2 = searchStep.begin(); iter2 != searchStep.end(); ++iter2) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[0], puzzle[1], puzzle[2]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[0], puzzle[1], puzzle[2]));
        }
        stepStr += _T("\n");
        //WriteText(_T("\n"));
        for (iter2 = searchStep.begin(); iter2 != searchStep.end(); ++iter2) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[3], puzzle[4], puzzle[5]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[3], puzzle[4], puzzle[5]));
        }
        stepStr += _T("\n");
        //WriteText(_T("\n"));
        for (iter2 = searchStep.begin(); iter2 != searchStep.end(); ++iter2) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[6], puzzle[7], puzzle[8]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[6], puzzle[7], puzzle[8]));
        }
        stepStr += _T("\n");

        for (fIter2 = fStep.begin(); fIter2 != fStep.end(); ++fIter2)
        {
            if (*fIter2 < 10.0) {
                stepStr += wxString::Format(_T("f: %0.0f  "), (*fIter2));
            } else {
                stepStr += wxString::Format(_T("f: %0.0f "), (*fIter2));
            }

        }
        stepStr += _T("\n");
        for (gIter2 = gStep.begin(); gIter2 != gStep.end(); ++gIter2)
        {
            if (*gIter2 < 10.0) {
                stepStr += wxString::Format(_T("g: %0.0f  "), (*gIter2));
            } else {
                stepStr += wxString::Format(_T("g: %0.0f "), (*gIter2));
            }
        }
        stepStr += _T("\n");
        for (hIter2 = hStep.begin(); hIter2 != hStep.end(); ++hIter2)
        {
            if (*hIter2 < 10.0) {
                stepStr += wxString::Format(_T("h: %0.0f  "), (*hIter2));
            } else {
                stepStr += wxString::Format(_T("h: %0.0f "), (*hIter2));
            }
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

        stepStr += wxString::Format(_T("%d. Closed list:\n"), i);
        for (iter2 = searchStep.begin(); iter2 != searchStep.end(); ++iter2) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[0], puzzle[1], puzzle[2]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[0], puzzle[1], puzzle[2]));
        }
        stepStr += _T("\n");
        //WriteText(_T("\n"));
        for (iter2 = searchStep.begin(); iter2 != searchStep.end(); ++iter2) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[3], puzzle[4], puzzle[5]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[3], puzzle[4], puzzle[5]));
        }
        stepStr += _T("\n");
        //WriteText(_T("\n"));
        for (iter2 = searchStep.begin(); iter2 != searchStep.end(); ++iter2) {
                Board board = *iter2;
                int *puzzle = board.getPuzzle();
                stepStr += wxString::Format(_T("%d%d%d   "), puzzle[6], puzzle[7], puzzle[8]);
                //WriteText(wxString::Format(_T("%d%d%d "), puzzle[6], puzzle[7], puzzle[8]));
        }
        stepStr += _T("\n");

        for (fIter2 = fStep.begin(); fIter2 != fStep.end(); ++fIter2)
        {
            if (*fIter2 < 10.0) {
                stepStr += wxString::Format(_T("f: %0.0f  "), (*fIter2));
            } else {
                stepStr += wxString::Format(_T("f: %0.0f "), (*fIter2));
            }

        }
        stepStr += _T("\n");
        for (gIter2 = gStep.begin(); gIter2 != gStep.end(); ++gIter2)
        {
            if (*gIter2 < 10.0) {
                stepStr += wxString::Format(_T("g: %0.0f  "), (*gIter2));
            } else {
                stepStr += wxString::Format(_T("g: %0.0f "), (*gIter2));
            }
        }
        stepStr += _T("\n");
        for (hIter2 = hStep.begin(); hIter2 != hStep.end(); ++hIter2)
        {
            if (*hIter2 < 10.0) {
                stepStr += wxString::Format(_T("h: %0.0f  "), (*hIter2));
            } else {
                stepStr += wxString::Format(_T("h: %0.0f "), (*hIter2));
            }
        }

        stepStr += _T("\n\n");
        if (stepStr.size() >= 30000) {
            textCtrl.WriteText(stepStr);
            stepStr = _T("");
        }
    }

    textCtrl.WriteText(stepStr);
    */
}
//int DjikstraAlgoView::getNIterations() {
//    return djikstrasearch.GetStepCount();
//}

