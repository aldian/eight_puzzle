#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "wx/dcbuffer.h"
#include "business/Board.h"
#include "UISettings.h"
#include "eight_puzzleMain.h"
#include "SolutionLog.h"
#include "BoardPanel.h"
#include "main_panel.h"

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__WXMAC__)
        wxbuild << _T("-Mac");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//MainPanel::MainPanel(wxFrame *frame, int x, int y, int w, int h): wxPanel( frame, wxID_ANY, wxPoint(x, y), wxSize(w, h))
MainPanel::MainPanel(wxFrame *frame): wxPanel( frame, wxID_ANY ), frame(frame)
{
    bgImage = new wxImage();
    bgImage->LoadFile(wxString(_("bg/")) + UISettings::BG_FILE_NAME, UISettings::BG_BITMAP_TYPE);
    bgBitmap = new wxBitmap(*bgImage);

    topSizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer *leftSizer = new wxBoxSizer(wxVERTICAL);

    //messageContainer = new wxStaticText(this, -1, _("Puzzle solved!"));
    //leftSizer->Add(messageContainer, wxSizerFlags().Align(wxALIGN_RIGHT).Border(wxALL & ~wxBOTTOM, 5));
    //messageContainer->Hide();

    boardPanel = new BoardPanel(this);
    leftSizer->Add(boardPanel, 0, wxALL, 5);

    wxButton *btnGeneratePuzzle = new wxButton(this, idBtnGeneratePuzzle, wxT("&Generate Puzzle"), wxDefaultPosition, wxDefaultSize, 0);
    leftSizer->Add(btnGeneratePuzzle, 0, wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxButton *btnSolvePuzzle = new wxButton(this, idBtnSolvePuzzle, wxT("&Solve Puzzle"), wxDefaultPosition, wxDefaultSize, 0);
    leftSizer->Add(btnSolvePuzzle, 0, wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxButton *btnStopPuzzle = new wxButton(this, idBtnStopPuzzle, wxT("S&top Puzzle"), wxDefaultPosition, wxDefaultSize, 0);
    leftSizer->Add(btnStopPuzzle, 0, wxLEFT|wxRIGHT|wxBOTTOM, 5);

    topSizer->Add(leftSizer, 0, wxALL, 0);

    wxBoxSizer* rightSizer = new wxBoxSizer(wxVERTICAL);
    solutionLog = new SolutionLog( this );
    solutionLog->SetMaxLength(10000000);
    rightSizer->Add(solutionLog, wxSizerFlags(1).Expand().Border(wxALL, 5));
    topSizer->Add(rightSizer, 1, wxEXPAND, 5);
    this->SetSizer(topSizer);
    topSizer->Fit(this);
    topSizer->SetSizeHints( frame );
    frame->SetSize(wxSize(1024 - 130, 768 - 40));
}

MainPanel::~MainPanel()
{
    if (bgBitmap != NULL) {
        delete bgBitmap;
    }
    if (bgImage != NULL) {
        delete bgImage;
    }
}

BEGIN_EVENT_TABLE(MainPanel, wxPanel)
    EVT_PAINT(MainPanel::OnPaint)
    EVT_BUTTON(idBtnQuit, MainPanel::OnQuit)
    EVT_BUTTON(idBtnAbout, MainPanel::OnAbout)
    EVT_BUTTON(idBtnGeneratePuzzle, MainPanel::OnGeneratePuzzle)
    EVT_BUTTON(idBtnSolvePuzzle, MainPanel::OnSolvePuzzle)
    EVT_BUTTON(idBtnStopPuzzle, MainPanel::OnStopPuzzle)
END_EVENT_TABLE()

void MainPanel::OnPaint(wxPaintEvent &event)
{
    wxBufferedPaintDC pdc(this, m_bufferBitmap);
    wxDC &dc = pdc ;
    PrepareDC(dc);

    wxBitmap subBitmap = bgBitmap->GetSubBitmap(wxRect(0 + 145, 0 + 95, 1024 - 145, 768 - 95));
    dc.DrawBitmap(subBitmap, 0, 0);
}

void MainPanel::OnQuit(wxCommandEvent &event)
{
    ((eight_puzzleDialog *) frame)->OnQuit(event);
}

void MainPanel::OnAbout(wxCommandEvent &event)
{
    ((eight_puzzleDialog *) frame)->OnAbout(event);
}

void MainPanel::OnGeneratePuzzle(wxCommandEvent &event)
{
    ((eight_puzzleDialog *) frame)->OnGeneratePuzzle(event);
}

void MainPanel::OnSolvePuzzle(wxCommandEvent &event)
{
    ((eight_puzzleDialog *) frame)->OnSolvePuzzle(event);
}

void MainPanel::OnStopPuzzle(wxCommandEvent &event)
{
    ((eight_puzzleDialog *) frame)->OnStopPuzzle(event);
}
