#ifndef HISTORYBOARD_H
#define HISTORYBOARD_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "business/board.h"

class HistoryBoard;

class HistoryBoardEvents
{
public:
    virtual void OnClick(wxMouseEvent &event) = 0;
    virtual void setHistoryBoard(HistoryBoard *historyBoard) = 0;
};

class HistoryBoard: public wxControl
{
public:
    HistoryBoard(wxWindow *parent, const Board &board, HistoryBoardEvents *events);
    virtual ~HistoryBoard();

    Board board;
private:
    wxSize DoGetBestSize() const;
    void OnClick(wxMouseEvent &event);
    void OnPaint(wxPaintEvent &event);
    void setTopLefts(int w, int h);
    void drawBoard(wxDC &dc);

    HistoryBoardEvents *events;
    wxBitmap m_bufferBitmap;
    int topLefts[9][2];
    DECLARE_EVENT_TABLE()
};

#endif // HISTORYBOARD_H
