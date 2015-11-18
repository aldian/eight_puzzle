#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include <iostream>
//#include <boost/graph/astar_search.hpp>
#include "wx/dcbuffer.h"
#include "business/AStarAlgoView.h"
#include "UISettings.h"
#include "eight_puzzleMain.h"
#include "main_panel.h"
#include "SolutionLog.h"
#include "BoardPanel.h"

wxPoint *BoardPanel::bgTopLefts = BoardPanel::initBgTopLefts();

BoardPanel::BoardPanel(wxPanel *parent)
    : wxPanel( parent, wxID_ANY, wxDefaultPosition, wxSize(UISettings::BOARD_WIDTH, UISettings::BOARD_WIDTH), wxFULL_REPAINT_ON_RESIZE)
    , m_parent(parent)
    , animationFrameNumber(UISettings::EXPECTED_FPS)
    , movingItemIndex(-1)
    , direction(-1)
    , algo(&AStarAlgoView::getInstance())
{
    bgImage = new wxImage();
    bgImage->LoadFile(wxString(_("bg/")) + UISettings::BG_FILE_NAME, UISettings::BG_BITMAP_TYPE);
    bgBitmap = new wxBitmap(*bgImage);
    /*
    setTopLefts(UISettings::BOARD_WIDTH);
    for (int i = 0; i < 9; ++i) {
        if (Board::solvedPuzzle[i] != 0)
        {
            int w, h;
            if (i == 2 || i == 5) {
                w = UISettings::BOARD_WIDTH - topLefts[i][0];
                h = topLefts[i + 3][1] - topLefts[i][1];
            } else if (i == 6 || i == 7) {
                w = topLefts[i + 1][0] - topLefts[i][0];
                h = UISettings::BOARD_WIDTH - topLefts[i][1];
            } else if (i == 8) {
                w = UISettings::BOARD_WIDTH - topLefts[i][0];
                h = UISettings::BOARD_WIDTH - topLefts[i][1];
            } else {
                w = topLefts[i + 1][0] - topLefts[i][0];
                h = topLefts[i + 3][1] - topLefts[i][1];
            }
            bgSubBitmaps[Board::solvedPuzzle[i]] = bgBitmap->GetSubBitmap(wxRect(bgTopLefts[Board::solvedPuzzle[i]].x + 150 + 1, bgTopLefts[Board::solvedPuzzle[i]].y + 100 + 1, w - 2, h - 2));
        }
    }
    */
}

BoardPanel::~BoardPanel()
{
    if (bgBitmap != NULL) {
        delete bgBitmap;
    }
    if (bgImage != NULL) {
        delete bgImage;
    }
}

BEGIN_EVENT_TABLE(BoardPanel, wxPanel)
    EVT_PAINT(BoardPanel::OnPaint)
    //EVT_ERASE_BACKGROUND(BoardPanel::OnEraseBackground)
    EVT_LEFT_UP(BoardPanel::OnClick)
END_EVENT_TABLE()

void BoardPanel::OnClick(wxMouseEvent &event)
{
    wxFrame *frame = ((MainPanel *) m_parent)->getFrame();
    if (((eight_puzzleDialog *) frame)->getPuzzleStarted()) {
        return;
    }
    wxCoord x = event.GetX();
    wxCoord y = event.GetY();

    int slotX;
    int slotY;

    if (x <= (UISettings::BOARD_WIDTH / 3)) {
        slotX = 0;
    } else if (x <= (UISettings::BOARD_WIDTH * 2 / 3)) {
        slotX = 1;
    } else {
        slotX = 2;
    }
    if (y <= (UISettings::BOARD_WIDTH / 3)) {
        slotY = 0;
    } else if (y <= (UISettings::BOARD_WIDTH * 2 / 3)) {
        slotY = 1;
    } else {
        slotY = 2;
    }
    int indexes[3][3] = {{0, 3, 6}, {1, 4, 7}, {2, 5, 8}};
    int index = indexes[slotX][slotY];
    int oldBlankIndex = board.getBlankIndex();
    int direction = board.moveToBlank(index);
    if (direction != -1) {
        setDirection(direction);
        setMovingItemIndex(oldBlankIndex);
        animationFrameNumber = 0;

        ((eight_puzzleDialog *) frame)->setPuzzle(board);
        ((eight_puzzleDialog *) frame)->setAnimationFrameNumber(animationFrameNumber);

        wxTextCtrl *solutionLog = ((MainPanel *) m_parent)->getSolutionLog();
        if (board.isSolved()) {
            ((SolutionLog *) solutionLog)->initLog();
            //((MainPanel *) m_parent)->getMessageContainer()->Show();
        } else {
            ((SolutionLog *) solutionLog)->generatePuzzle(board);
            //((MainPanel *) m_parent)->getMessageContainer()->Hide();
        }
        //((MainPanel *) m_parent)->getTopSizer()->SetSizeHints( ((MainPanel *) m_parent)->getFrame() );
    }
}

void BoardPanel::OnPaint(wxPaintEvent &event)
{
    //wxPanel::OnPaint(event);
    //wxPaintDC pdc(this);
    wxBufferedPaintDC pdc(this, m_bufferBitmap);
    wxDC &dc = pdc ;
    PrepareDC(dc);

    wxSize sz = GetClientSize();
    int w = std::min(sz.GetWidth(), sz.GetHeight());
    wxRect rectToDraw(0, 0, w, w);
    dc.SetBrush(*wxBLACK_BRUSH);
    //dc.SetPen(*wxBLACK_PEN);
    dc.DrawRectangle(rectToDraw);
    dc.SetBrush(*wxWHITE_BRUSH);

    setTopLefts(w);
    drawBoard(dc);

    //if (IsExposed(rectToDraw))
    //{
        //dc.SetBrush(*wxRED_BRUSH);
        //dc.SetPen(*wxBLACK_PEN);
        //dc.DrawRectangle(rectToDraw);
    //}
}

/*
void BoardPanel::OnEraseBackground( wxEraseEvent& event )
{
    wxDC& dc = *event.GetDC();
    PrepareDC( dc );
    wxSize sz = GetClientSize();
    int w = std::min(sz.GetWidth(), sz.GetHeight());
    wxRect rectToDraw(0, 0, w, w);
    //wxRect rectToDraw(0, 0, 200, 200);
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen(*wxBLACK_PEN);
    dc.DrawRectangle(rectToDraw);
}
*/

void BoardPanel::drawBoard(wxDC &dc)
{
    const int* puzzle = board.getPuzzle();
    dc.SetFont( wxFont( UISettings::FONT_SIZE, wxSWISS, wxNORMAL, wxBOLD ) );
    for (int i = 0; i < 9; ++i)
    {
        if (puzzle[i] != 0)
        {
            int w, h;
            if (i == 2 || i == 5) {
                w = UISettings::BOARD_WIDTH - topLefts[i][0];
                h = topLefts[i + 3][1] - topLefts[i][1];
            } else if (i == 6 || i == 7) {
                w = topLefts[i + 1][0] - topLefts[i][0];
                h = UISettings::BOARD_WIDTH - topLefts[i][1];
            } else if (i == 8) {
                w = UISettings::BOARD_WIDTH - topLefts[i][0];
                h = UISettings::BOARD_WIDTH - topLefts[i][1];
            } else {
                w = topLefts[i + 1][0] - topLefts[i][0];
                h = topLefts[i + 3][1] - topLefts[i][1];
            }

            long length;
            long height;
            long descent;
            wxString val = wxString::Format(_("%d"), puzzle[i]);
            dc.GetTextExtent(val, &length, &height, &descent );
            int xOffset = (w - length) / 2;
            int yOffset = (h - height) / 2;

            if (animationFrameNumber >= UISettings::EXPECTED_FPS)
            {
                wxRect rectToDraw(topLefts[i][0], topLefts[i][1], w, h);
                dc.DrawRectangle(rectToDraw);
                wxBitmap subBitmap = bgBitmap->GetSubBitmap(wxRect(bgTopLefts[puzzle[i]].x + 150 + 1, bgTopLefts[puzzle[i]].y + 100 + 1, w - 2, h - 2));
                dc.DrawBitmap(subBitmap, topLefts[i][0] + 1, topLefts[i][1] + 1);
                //dc.DrawBitmap(bgSubBitmaps[puzzle[i]], topLefts[i][0] + 1, topLefts[i][1] + 1);
                dc.DrawText(val, topLefts[i][0] + xOffset, topLefts[i][1] + yOffset);
            }
            else
            {
                //wxTextCtrl *solutionLog = ((MainPanel *) m_parent)->getSolutionLog();
                //((SolutionLog *) solutionLog)->logCustom(wxString::Format(_("movingItemIndex: %d, i: %d\n"), movingItemIndex, i));
                if (movingItemIndex == i)
                {

                    int x = topLefts[i][0];
                    int y = topLefts[i][1];
                    if (direction == 0) { //left
                        x += (w - (w * std::min(animationFrameNumber, UISettings::EXPECTED_FPS) / UISettings::EXPECTED_FPS));
                    } else if (direction == 1) { //right
                        x -= (w - (w * std::min(animationFrameNumber, UISettings::EXPECTED_FPS) / UISettings::EXPECTED_FPS));
                    } else if (direction == 2) { //up
                        y += (h - (h * std::min(animationFrameNumber, UISettings::EXPECTED_FPS) / UISettings::EXPECTED_FPS));
                    } else { //down
                        y -= (h - (h * std::min(animationFrameNumber, UISettings::EXPECTED_FPS) / UISettings::EXPECTED_FPS));
                    }

                    //((SolutionLog *) solutionLog)->logCustom(wxString::Format(_("x y w h: %d %d %d %d\n"), x, y, w, h));
                    wxRect rectToDraw(x, y, w, h);
                    dc.DrawRectangle(rectToDraw);
                    wxBitmap subBitmap = bgBitmap->GetSubBitmap(wxRect(bgTopLefts[puzzle[i]].x + 150 + 1, bgTopLefts[puzzle[i]].y + 100 + 1, w - 2, h - 2));
                    dc.DrawBitmap(subBitmap, x + 1, y + 1);
                    //dc.DrawBitmap(bgSubBitmaps[puzzle[i]], x + 1, y + 1);
                    dc.DrawText(val, x + xOffset, y + yOffset);
                }
                else
                {
                    wxRect rectToDraw(topLefts[i][0], topLefts[i][1], w, h);
                    dc.DrawRectangle(rectToDraw);
                    wxBitmap subBitmap = bgBitmap->GetSubBitmap(wxRect(bgTopLefts[puzzle[i]].x + 150 + 1, bgTopLefts[puzzle[i]].y + 100 + 1, w - 2, h - 2));
                    dc.DrawBitmap(subBitmap, topLefts[i][0] + 1, topLefts[i][1] + 1);
                    //dc.DrawBitmap(bgSubBitmaps[puzzle[i]], topLefts[i][0] + 1, topLefts[i][1] + 1);
                    dc.DrawText(val, topLefts[i][0] + xOffset, topLefts[i][1] + yOffset);
                }
            }
        }
    }
}

void BoardPanel::setTopLefts(int w)
{
    int xs[3];
    xs[0] = 0;
    xs[1] = w / 3;
    xs[2] = w * 2 / 3;
    //{{x1, x1}, {x1, x2}, {x1, x3}, {x2, x1}, {x2, x2}, {x2, x3}, {x3, x1}, {x3, x2}, {x3, x3}};
    int k = 0;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            this->topLefts[k][0] = xs[j];
            this->topLefts[k][1] = xs[i];
            k += 1;
        }
    }
}

wxPoint *BoardPanel::initBgTopLefts()
{
    bgTopLefts = new wxPoint[9];
    int w = UISettings::BOARD_WIDTH;
    int xs[3];
    xs[0] = 0;
    xs[1] = w / 3;
    xs[2] = w * 2 / 3;
    bgTopLefts[1] = wxPoint(xs[0], xs[0]);
    bgTopLefts[2] = wxPoint(xs[1], xs[0]);
    bgTopLefts[3] = wxPoint(xs[2], xs[0]);

    bgTopLefts[8] = wxPoint(xs[0], xs[1]);
    bgTopLefts[0] = wxPoint(xs[1], xs[1]);
    bgTopLefts[4] = wxPoint(xs[2], xs[1]);

    bgTopLefts[7] = wxPoint(xs[0], xs[2]);
    bgTopLefts[6] = wxPoint(xs[1], xs[2]);
    bgTopLefts[5] = wxPoint(xs[2], xs[2]);

    return bgTopLefts;
}

void BoardPanel::generatePuzzle(const Board &board, int animationFrameNumber)
{
    setAnimationFrameNumber(animationFrameNumber);
    setBoard(board);
    Refresh(false);
}

void BoardPanel::endPuzzle()
{
}

Board BoardPanel::nextMove()
{
    board = algo->nextMove(board, direction, movingItemIndex);
    return board;
}

void BoardPanel::solve(bool isSearchLogShown)
{
    algo->execute(board, *((MainPanel *) m_parent)->getSolutionLog(), isSearchLogShown);
}
