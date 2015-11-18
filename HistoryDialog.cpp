#include "HistoryBoard.h"
#include "HistoryDialog.h"
#include "UISettings.h"
#include "main_panel.h"
#include "SolutionLog.h"
#include "BoardPanel.h"
#include "eight_puzzleMain.h"


BEGIN_EVENT_TABLE(HistoryDialog, wxDialog)
END_EVENT_TABLE()


class MyHistoryBoardEvents : public HistoryBoardEvents {
public:
    MyHistoryBoardEvents(eight_puzzleDialog *frame, HistoryDialog *dialog);
    void OnClick(wxMouseEvent &event);
    void setHistoryBoard(HistoryBoard *historyBoard) {
        this->historyBoard = historyBoard;
    }
private:
    eight_puzzleDialog *frame;
    HistoryDialog *dialog;
    HistoryBoard *historyBoard;
};

MyHistoryBoardEvents::MyHistoryBoardEvents(eight_puzzleDialog *frame, HistoryDialog *dialog)
    : frame(frame)
    , dialog(dialog)
{
}

void MyHistoryBoardEvents::OnClick(wxMouseEvent &event)
{
    frame->setPuzzleStarted(false);
    frame->setPuzzle(historyBoard->board);
    frame->setAnimationFrameNumber(UISettings::EXPECTED_FPS);
    wxPanel *boardPanel = ((MainPanel *) frame->getMainPanel())->getBoardPanel();
    wxTextCtrl *solutionLog = ((MainPanel *) frame->getMainPanel())->getSolutionLog();
    ((SolutionLog *) solutionLog)->generatePuzzle(historyBoard->board);
    ((BoardPanel *) boardPanel)->generatePuzzle(historyBoard->board, frame->getAnimationFrameNumber());

    /*
    if (!isPuzzleStarted) {
        puzzle.shuffle();
        animationFrameNumber = UISettings::EXPECTED_FPS;
        wxPanel *boardPanel = ((MainPanel *) m_mainPanel)->getBoardPanel();
        wxTextCtrl *solutionLog = ((MainPanel *) m_mainPanel)->getSolutionLog();
        ((SolutionLog *) solutionLog)->generatePuzzle(puzzle);
        ((BoardPanel *) boardPanel)->generatePuzzle(puzzle, animationFrameNumber);

    }
    */
    dialog->Destroy();
}


HistoryDialog::HistoryDialog(wxWindow *parent, const vector<Board> &boards)
    : wxDialog(parent, wxID_ANY, wxString(_T("Puzzle History")))
    , boards(boards)
{
    topSizer = new wxBoxSizer(wxHORIZONTAL);
    vector<Board>::const_iterator boardIter;
    for (boardIter = boards.begin(); boardIter != boards.end(); ++boardIter) {
        HistoryBoardEvents *historyBoardEvents = new MyHistoryBoardEvents((eight_puzzleDialog *) parent, this);
        HistoryBoard *histBoard = new HistoryBoard(this, *boardIter, historyBoardEvents);
        historyBoardEvents->setHistoryBoard(histBoard);
        topSizer->Add(histBoard, 0, wxALL, 5);
    }
    this->SetSizer(topSizer);
    topSizer->Fit(this);
    topSizer->SetSizeHints( this );
}

HistoryDialog::~HistoryDialog()
{
    //dtor
    //delete topSizer;
}
