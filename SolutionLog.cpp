#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <tchar.h>
#include "business/AStarAlgoView.h"
#include "business/board.h"
#include "UISettings.h"
#include "SolutionLog.h"

SolutionLog::SolutionLog(wxPanel *container)
    : wxTextCtrl( container, wxID_ANY, _T(""), wxDefaultPosition, wxSize(UISettings::LOG_WIDTH,60), wxTE_MULTILINE|wxTE_READONLY|wxTE_DONTWRAP|wxHSCROLL)
    , algo(&AStarAlgoView::getInstance())
    , step(0)
{
    wxFont font(this->GetFont());
    font.SetFamily(wxFONTFAMILY_SWISS);
    font.SetWeight(wxFONTWEIGHT_BOLD);
    font.SetPointSize(UISettings::FONT_SIZE);
    font.SetFaceName(_("Courier"));
    this->SetFont(font);
    initLog();
}

SolutionLog::~SolutionLog()
{
    //dtor
}

void SolutionLog::generatePuzzle(const Board &board)
{
    puzzle = board;
    initLog();
    step = 0;
    WriteText(wxString::Format(_T("\nPuzzle:\n%s"), puzzle.toString().c_str()));
}

void SolutionLog::logStep(const Board &board, const TCHAR *customText)
{
    puzzle = board;
    WriteText(wxString::Format(_T("\nStep %s%d:\n%s"), customText, ++step, puzzle.toString().c_str()));
}

void SolutionLog::logSearch()
{
    algo->logSearch(*this);
}

void SolutionLog::logCustom(const TCHAR *customText)
{
    WriteText(customText);
}

void SolutionLog::initLog()
{
    Board board;
    Clear();
    WriteText(wxString::Format(_T("Goal state:\n%s"), board.toString().c_str()));
}
