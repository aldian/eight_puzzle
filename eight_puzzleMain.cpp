/***************************************************************
 * Name:      eight_puzzleMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Aldian Fazrihady (mobile@aldian.net)
 * Created:   2012-03-03
 * Copyright: Aldian Fazrihady (http://www.aldian.net)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <tchar.h>
#include <string>
#include <boost/format.hpp>
#include <boost/scoped_ptr.hpp>

#include "business/Board.h"
#include "business/AStarAlgoView.h"
#include "business/BFSAlgoView.h"
#include "business/DjikstraAlgoView.h"
#include "business/NilssonSequence.h"
#include "business/ManhattanDistance.h"
#include "business/SimpleCostCalculation.h"
#include "UISettings.h"
#include "BoardPanel.h"
#include "SolutionLog.h"
#include "main_panel.h"
#include "HistoryDialog.h"
#include "eight_puzzleMain.h"


BEGIN_EVENT_TABLE(eight_puzzleDialog, wxFrame)
    EVT_MENU(idBtnQuit, eight_puzzleDialog::OnQuit)
    EVT_MENU(idBtnAbout, eight_puzzleDialog::OnAbout)
    EVT_MENU(idBtnGeneratePuzzle, eight_puzzleDialog::OnGeneratePuzzle)
    EVT_MENU(idBtnSolvePuzzle, eight_puzzleDialog::OnSolvePuzzle)
    EVT_MENU(idBtnStopPuzzle, eight_puzzleDialog::OnStopPuzzle)
    EVT_MENU(idBtnResetPuzzle, eight_puzzleDialog::OnResetPuzzle)
    //EVT_MENU(idBtnDummyAlgo, eight_puzzleDialog::OnUseDummyAlgo)
    EVT_MENU(idBtnAStarAlgo, eight_puzzleDialog::OnUseAStarAlgo)
    EVT_MENU(idBtnBFSAlgo, eight_puzzleDialog::OnUseBFSAlgo)
    EVT_MENU(idBtnDjikstraAlgo, eight_puzzleDialog::OnUseDjikstraAlgo)
    EVT_MENU(idBtnHeuManhattan, eight_puzzleDialog::OnUseManhattanHeu)
    EVT_MENU(idBtnHeuNilsson, eight_puzzleDialog::OnUseNilssonHeu)
    EVT_MENU(idBtnHeuSimple, eight_puzzleDialog::OnUseSimpleHeu)
    EVT_MENU(idBtnShowSearchLog, eight_puzzleDialog::OnShowSearchLog)
    EVT_MENU(idBtnShowHistory, eight_puzzleDialog::OnShowHistory)
    EVT_TIMER(idTimer, eight_puzzleDialog::OnTimer)
    EVT_CLOSE(eight_puzzleDialog::OnClose)
END_EVENT_TABLE()

eight_puzzleDialog::eight_puzzleDialog(const wxString &title)
    : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE)
    , currentSecond(0)
    , timer(this, idTimer)
    , isPuzzleStarted(false)
    , isSearchLogShown(false)
    , algo(&AStarAlgoView::getInstance())
    , animationFrameNumber(UISettings::EXPECTED_FPS)
{
    wxMenu *algoSubmenu = new wxMenu;
    //algoSubmenu->AppendRadioItem(idBtnDummyAlgo, _T("&Dummy"), _T("Dummy"));
    algoSubmenu->AppendRadioItem(idBtnAStarAlgo, _T("&A*"), _T("A*"));
    algoSubmenu->AppendRadioItem(idBtnBFSAlgo, _T("Greedy &BFS"), _T("Greedy BFS"));
    algoSubmenu->AppendRadioItem(idBtnDjikstraAlgo, _T("&Djikstra"), _T("Djikstra"));

    wxMenu *heuristicSubmenu = new wxMenu;
    heuristicSubmenu->AppendRadioItem(idBtnHeuManhattan, _T("&Manhattan Distance"), _T("Manhattan Distance"));
    heuristicSubmenu->AppendRadioItem(idBtnHeuNilsson, _T("&Nilsson's Sequence"), _T("Nilsson's Sequence"));
    heuristicSubmenu->AppendRadioItem(idBtnHeuSimple, _T("&Simple Comparison"), _T("Simple Comparison"));

    wxMenu *file_menu = new wxMenu;
    file_menu->Append(idBtnGeneratePuzzle, _T("&Generate Puzzle"), _T("Generate Puzzle"));
    file_menu->Append(idBtnSolvePuzzle, _T("&Solve Puzzle"), _T("Solve Puzzle"));
    file_menu->Append(idBtnStopPuzzle, _T("S&top Puzzle"), _T("Stop Puzzle"));
    file_menu->Append(idBtnResetPuzzle, _T("&Reset Puzzle"), _T("Reset Puzzle"));
    file_menu->AppendSeparator();
    file_menu->AppendCheckItem(idBtnShowSearchLog, _T("Show Search &Log"), _T("Show Search Log"));
    file_menu->AppendSeparator();
    file_menu->AppendSubMenu(algoSubmenu, _T("&Algorithms"));
    file_menu->AppendSeparator();
    file_menu->AppendSubMenu(heuristicSubmenu, _T("He&uristics"));
    file_menu->AppendSeparator();
    file_menu->Append(idBtnShowHistory, _T("Show &History..."), _T("Show History..."));
    file_menu->AppendSeparator();
    file_menu->Append(idBtnQuit, _T("&Quit"), _T("Quit"));

    wxMenu *help_menu = new wxMenu;
    help_menu->Append(idBtnAbout, _T("&About"), _T("About 8 Puzzle Sover..."));

    wxMenuBar *menu_bar = new wxMenuBar;
    menu_bar->Append(file_menu, _T("&File"));
    menu_bar->Append(help_menu, _T("&Help"));
    SetMenuBar(menu_bar);

    m_mainPanel = new MainPanel( this );

    timer.Start(1.0 * 1000 / UISettings::EXPECTED_FPS, false);
}


eight_puzzleDialog::~eight_puzzleDialog()
{
}

bool eight_puzzleDialog::Destroy()
{
    return wxFrame::Destroy();
}

void eight_puzzleDialog::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void eight_puzzleDialog::OnQuit(wxCommandEvent &event)
{
    Destroy();
}


void eight_puzzleDialog::OnAbout(wxCommandEvent &event)
{
    wxString msg = wxString(UISettings::APP_TITLE) + _("\nby Aldian Fazrihady (23211328)\nDikerjakan sebagai ujian akhir mata kuliah Sistem Intelijen\nTMDG 6 semester 1 2012");
    wxMessageBox(msg, _("Welcome to..."));
}

void eight_puzzleDialog::OnGeneratePuzzle(wxCommandEvent &event)
{
    if (!isPuzzleStarted) {
        puzzle.shuffle();
        animationFrameNumber = UISettings::EXPECTED_FPS;
        wxPanel *boardPanel = ((MainPanel *) m_mainPanel)->getBoardPanel();
        wxTextCtrl *solutionLog = ((MainPanel *) m_mainPanel)->getSolutionLog();
        ((SolutionLog *) solutionLog)->generatePuzzle(puzzle);
        ((BoardPanel *) boardPanel)->generatePuzzle(puzzle, animationFrameNumber);

        //((MainPanel *) m_mainPanel)->getMessageContainer()->Hide();
        //((MainPanel *) m_mainPanel)->getTopSizer()->SetSizeHints(this);
    }
}

void eight_puzzleDialog::OnSolvePuzzle(wxCommandEvent &event)
{
    vector<Board>::iterator iter;
    for (iter = historicalBoards.begin(); iter != historicalBoards.end();)
    {
        if (*iter == puzzle) {
            vector<Board>::iterator currentIter = iter;
            ++iter;
            historicalBoards.erase(currentIter);
            break;
        } else {
            ++iter;
        }
    }
    historicalBoards.insert(historicalBoards.begin(), puzzle);

    wxPanel *boardPanel = ((MainPanel *) m_mainPanel)->getBoardPanel();
    ((BoardPanel *) boardPanel)->solve(isSearchLogShown);
    wxTextCtrl *solutionLog = ((MainPanel *) m_mainPanel)->getSolutionLog();
    if (isSearchLogShown) {
        ((SolutionLog *) solutionLog)->logSearch();
    }
    //((SolutionLog *) solutionLog)->WriteText(wxString::Format(_T("\nAlgoritma %s melakukan %d iterasi sebelum menemukan solusi.\n"), algo->getName(), algo->getNIterations()));
    //((SolutionLog *) solutionLog)->WriteText(wxString::Format(_T("Algoritma %s menghasilkan %d steps dari start state ke goal state.\n"), algo->getName(), algo->getNSteps()));
    algo->logStr += wxString::Format(_T("\nAlgoritma %s melakukan %d iterasi sebelum menemukan solusi.\n"), algo->getName(), algo->getNIterations());
    algo->logStr += wxString::Format(_T("Algoritma %s menghasilkan %d steps dari start state ke goal state.\n"), algo->getName(), algo->getNSteps());
    isPuzzleStarted = true;
}

void eight_puzzleDialog::OnStopPuzzle(wxCommandEvent &event)
{
    isPuzzleStarted = false;
}

void eight_puzzleDialog::OnResetPuzzle(wxCommandEvent &event)
{
    isPuzzleStarted = false;
    animationFrameNumber = UISettings::EXPECTED_FPS;
    puzzle = Board();
    wxPanel *boardPanel = ((MainPanel *) m_mainPanel)->getBoardPanel();
    ((BoardPanel *) boardPanel)->setBoard(puzzle);
    wxTextCtrl *solutionLog = ((MainPanel *) m_mainPanel)->getSolutionLog();
    ((SolutionLog *) solutionLog)->initLog();
    boardPanel->Refresh(false);
}

void eight_puzzleDialog::OnUseAStarAlgo(wxCommandEvent &event)
{
    isPuzzleStarted = false;
    animationFrameNumber = UISettings::EXPECTED_FPS;
    //animationFrameNumber = 0;
    puzzle = Board();
    wxPanel *boardPanel = ((MainPanel *) m_mainPanel)->getBoardPanel();
    ((BoardPanel *) boardPanel)->setBoard(puzzle);
    //((BoardPanel *) boardPanel)->setAnimationFrameNumber(animationFrameNumber);
    algo = &AStarAlgoView::getInstance();
    ((BoardPanel *) boardPanel)->setAlgo(&AStarAlgoView::getInstance());
    wxTextCtrl *solutionLog = ((MainPanel *) m_mainPanel)->getSolutionLog();
    ((SolutionLog *) solutionLog)->setAlgo(&AStarAlgoView::getInstance());
    ((SolutionLog *) solutionLog)->initLog();
    boardPanel->Refresh(false);
}

void eight_puzzleDialog::OnUseBFSAlgo(wxCommandEvent &event)
{
    isPuzzleStarted = false;
    animationFrameNumber = UISettings::EXPECTED_FPS;
    puzzle = Board();
    wxPanel *boardPanel = ((MainPanel *) m_mainPanel)->getBoardPanel();
    ((BoardPanel *) boardPanel)->setBoard(puzzle);
    algo = &BFSAlgoView::getInstance();
    ((BoardPanel *) boardPanel)->setAlgo(&BFSAlgoView::getInstance());
    wxTextCtrl *solutionLog = ((MainPanel *) m_mainPanel)->getSolutionLog();
    ((SolutionLog *) solutionLog)->setAlgo(&BFSAlgoView::getInstance());
    ((SolutionLog *) solutionLog)->initLog();
    boardPanel->Refresh(false);
}


void eight_puzzleDialog::OnUseDjikstraAlgo(wxCommandEvent &event)
{
    isPuzzleStarted = false;
    animationFrameNumber = UISettings::EXPECTED_FPS;
    puzzle = Board();
    wxPanel *boardPanel = ((MainPanel *) m_mainPanel)->getBoardPanel();
    ((BoardPanel *) boardPanel)->setBoard(puzzle);
    algo = &DjikstraAlgoView::getInstance();
    ((BoardPanel *) boardPanel)->setAlgo(&DjikstraAlgoView::getInstance());
    wxTextCtrl *solutionLog = ((MainPanel *) m_mainPanel)->getSolutionLog();
    ((SolutionLog *) solutionLog)->setAlgo(&DjikstraAlgoView::getInstance());
    ((SolutionLog *) solutionLog)->initLog();
    boardPanel->Refresh(false);
}


void eight_puzzleDialog::OnUseManhattanHeu(wxCommandEvent &event)
{
    Board::setHeuristic(&ManhattanDistance::getInstance());
}


void eight_puzzleDialog::OnUseNilssonHeu(wxCommandEvent &event)
{
    Board::setHeuristic(&NilssonSequence::getInstance());
}


void eight_puzzleDialog::OnUseSimpleHeu(wxCommandEvent &event)
{
    Board::setHeuristic(&SimpleCostCalculation::getInstance());
}


void eight_puzzleDialog::OnShowSearchLog(wxCommandEvent &event)
{
    isSearchLogShown = !isSearchLogShown;
}

void eight_puzzleDialog::OnShowHistory(wxCommandEvent &event)
{
    if (historicalBoards.size() > 0) {
        HistoryDialog dlg(this, historicalBoards);
        dlg.ShowModal();
    }
}

void eight_puzzleDialog::OnTimer(wxTimerEvent& event)
{
    if (isPuzzleStarted && (algo->logStr.size() == 0))
    {
        OnSimulationTimer(event);
    }
    else
    {
        if (algo->logStr.size() > 0) {
            wxTextCtrl *solutionLog = ((MainPanel *) m_mainPanel)->getSolutionLog();
            solutionLog->WriteText(algo->logStr);
            algo->logStr.clear();
        } else {
            OnGameTimer(event);
        }
    }

    //timer.Start(1000 / UISettings::EXPECTED_FPS, true);
}


void eight_puzzleDialog::OnSimulationTimer(wxTimerEvent& event)
{
    wxPanel *boardPanel = ((MainPanel *) m_mainPanel)->getBoardPanel();
    if (puzzle.isSolved())
    {
        if (animationFrameNumber < UISettings::EXPECTED_FPS)
        {
            // try to finish animation as smooth as possible
            ((BoardPanel *) boardPanel)->setAnimationFrameNumber(animationFrameNumber);
            boardPanel->Refresh(false);
            animationFrameNumber += 1;
        }
        else
        {
            isPuzzleStarted = false;
            ((BoardPanel *) boardPanel)->endPuzzle();

            //((MainPanel *) m_mainPanel)->getMessageContainer()->Show();
            //((MainPanel *) m_mainPanel)->getTopSizer()->SetSizeHints( this );
        }
    }
    else
    {
        wxTextCtrl *solutionLog = ((MainPanel *) m_mainPanel)->getSolutionLog();

        if (currentSecond < time(NULL)) // second has passed, force move
        {
            animationFrameNumber = UISettings::EXPECTED_FPS;
            ((BoardPanel *) boardPanel)->setAnimationFrameNumber(animationFrameNumber);
            boardPanel->Refresh(false);

            puzzle = ((BoardPanel *) boardPanel)->nextMove();
            ((SolutionLog *) solutionLog)->logStep(puzzle);
            currentSecond = time(NULL);
        }

        animationFrameNumber += 1;
        if (animationFrameNumber > UISettings::EXPECTED_FPS) {
            animationFrameNumber = 1;
        }

        ((BoardPanel *) boardPanel)->setAnimationFrameNumber(animationFrameNumber);
        boardPanel->Refresh(false);
    }
}


void eight_puzzleDialog::OnGameTimer(wxTimerEvent& event)
{
    wxPanel *boardPanel = ((MainPanel *) m_mainPanel)->getBoardPanel();
    int boardFrameNumber = ((BoardPanel *) boardPanel)->getAnimationFrameNumber();
    if (boardFrameNumber < UISettings::EXPECTED_FPS) {
        animationFrameNumber = boardFrameNumber + 3;
        ((BoardPanel *) boardPanel)->setAnimationFrameNumber(animationFrameNumber);
        boardPanel->Refresh(false);
    }
}


void eight_puzzleDialog::OnIdle(wxIdleEvent& event)
{
    event.RequestMore();
    event.Skip();
}
