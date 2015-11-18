/***************************************************************
 * Name:      eight_puzzleMain.h
 * Purpose:   Defines Application Frame
 * Author:    Aldian Fazrihady (mobile@aldian.net)
 * Created:   2012-03-03
 * Copyright: Aldian Fazrihady (http://www.aldian.net)
 * License:
 **************************************************************/

#ifndef EIGHT_PUZZLEMAIN_H
#define EIGHT_PUZZLEMAIN_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <set>
#include "business/Board.h"
#include "business/AlgoView.h"
#include "eight_puzzleApp.h"

class eight_puzzleDialog: public wxFrame
{
    public:
        eight_puzzleDialog(const wxString& title);
        ~eight_puzzleDialog();

        void setPuzzle(Board puzzle)
        {
            this->puzzle = puzzle;
        }

        void setAnimationFrameNumber(int n)
        {
            this->animationFrameNumber = n;
        }

        int getAnimationFrameNumber()
        {
            return this->animationFrameNumber;
        }

        bool getPuzzleStarted() const
        {
            return isPuzzleStarted;
        }

        void setPuzzleStarted(bool isPuzzleStarted)
        {
            this->isPuzzleStarted = isPuzzleStarted;
        }

        wxPanel *getMainPanel()
        {
            return m_mainPanel;
        }

        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnGeneratePuzzle(wxCommandEvent& event);
        void OnSolvePuzzle(wxCommandEvent& event);
        void OnStopPuzzle(wxCommandEvent& event);
        void OnResetPuzzle(wxCommandEvent& event);
        void OnUseAStarAlgo(wxCommandEvent& event);
        void OnUseBFSAlgo(wxCommandEvent& event);
        void OnUseDjikstraAlgo(wxCommandEvent& event);
        void OnUseManhattanHeu(wxCommandEvent& event);
        void OnUseNilssonHeu(wxCommandEvent& event);
        void OnUseSimpleHeu(wxCommandEvent& event);
        void OnShowSearchLog(wxCommandEvent& event);
        void OnShowHistory(wxCommandEvent& event);

        bool isSearchLogShown;
    protected:
        enum
        {
            idBtnQuit = 1000,
            idBtnGeneratePuzzle,
            idBtnSolvePuzzle,
            idBtnStopPuzzle,
            idBtnResetPuzzle,
            idBtnAStarAlgo,
            idBtnBFSAlgo,
            idBtnDjikstraAlgo,
            idBtnHeuManhattan,
            idBtnHeuNilsson,
            idBtnHeuSimple,
            idBtnShowSearchLog,
            idBtnShowHistory,
            idBtnAbout,
            idTimer,
        };
        wxPanel * m_mainPanel;

    private:
        Board puzzle;
        bool isPuzzleStarted;
        time_t currentSecond;
        int animationFrameNumber;
        wxTimer timer;
        AlgoView *algo;
        vector<Board> historicalBoards;

        bool Destroy();
        void OnClose(wxCloseEvent& event);
        void OnTimer(wxTimerEvent& event);
        void OnIdle(wxIdleEvent& event);
        void OnSimulationTimer(wxTimerEvent &event);
        void OnGameTimer(wxTimerEvent &event);
        DECLARE_EVENT_TABLE()
};

#endif // EIGHT_PUZZLEMAIN_H
