#include "wx/dcbuffer.h"
#include "HistoryBoard.h"

BEGIN_EVENT_TABLE(HistoryBoard, wxControl)
    EVT_LEFT_UP(HistoryBoard::OnClick)
    EVT_PAINT(HistoryBoard::OnPaint)
END_EVENT_TABLE()

HistoryBoard::HistoryBoard(wxWindow *parent, const Board &board, HistoryBoardEvents *events)
    : wxControl( parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
    , board(board)
    , events(events)
{
}

HistoryBoard::~HistoryBoard()
{
    delete events;
}

wxSize HistoryBoard::DoGetBestSize() const
{
    return wxSize(100, 100);
}

void HistoryBoard::OnClick(wxMouseEvent &event)
{
    events->OnClick(event);
}

void HistoryBoard::OnPaint(wxPaintEvent &event)
{
    wxBufferedPaintDC pdc(this, m_bufferBitmap);
    wxDC &dc = pdc ;
    PrepareDC(dc);

    wxSize sz = GetClientSize();
    int w = sz.GetWidth();
    int h = sz.GetHeight();
    wxRect rectToDraw(0, 0, w, h);
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.DrawRectangle(rectToDraw);
    dc.SetBrush(*wxWHITE_BRUSH);
    setTopLefts(w, h);
    drawBoard(dc);

    //wxString val = wxString::Format(_("%d"), 2);
    //dc.DrawText(val, 10, 10);
}

void HistoryBoard::setTopLefts(int w, int h)
{
    int xs[3], ys[3];
    xs[0] = 0;
    xs[1] = w / 3;
    xs[2] = w * 2 / 3;
    ys[0] = 0;
    ys[1] = h / 3;
    ys[2] = h * 2 / 3;
    //{{x1, x1}, {x1, x2}, {x1, x3}, {x2, x1}, {x2, x2}, {x2, x3}, {x3, x1}, {x3, x2}, {x3, x3}};
    int k = 0;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            this->topLefts[k][0] = xs[j];
            this->topLefts[k][1] = ys[i];
            k += 1;
        }
    }
}

void HistoryBoard::drawBoard(wxDC &dc)
{
    const int* puzzle = board.getPuzzle();
    dc.SetFont( wxFont( 20, wxSWISS, wxNORMAL, wxBOLD ) );
    for (int i = 0; i < 9; ++i)
    {
        if (puzzle[i] != 0)
        {
            int w, h;
            if (i == 2 || i == 5) {
                w = 100 - topLefts[i][0];
                h = topLefts[i + 3][1] - topLefts[i][1];
            } else if (i == 6 || i == 7) {
                w = topLefts[i + 1][0] - topLefts[i][0];
                h = 100 - topLefts[i][1];
            } else if (i == 8) {
                w = 100 - topLefts[i][0];
                h = 100 - topLefts[i][1];
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

            wxRect rectToDraw(topLefts[i][0], topLefts[i][1], w, h);
            dc.DrawRectangle(rectToDraw);
            dc.DrawText(val, topLefts[i][0] + xOffset, topLefts[i][1] + yOffset);
        }
    }
}
