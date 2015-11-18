#ifndef BOARDPANEL_H
#define BOARDPANEL_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <set>
#include "business/Board.h"
#include "business/AlgoView.h"

class BoardPanel: public wxPanel
{
    public:
        BoardPanel(wxPanel *parent);
        virtual ~BoardPanel();

        void OnPaint(wxPaintEvent &event);
        //void OnEraseBackground( wxEraseEvent &event );
        void drawBoard(wxDC &dc);

        void generatePuzzle(const Board &board, int animationFrameNumber);

        void endPuzzle();

        Board nextMove();

        void solve(bool isSearchLogShown);

        Board getBoard() const {
            return board;
        }

        void setBoard(const Board &board)
        {
            this->board = board;
        }

        int getAnimationFrameNumber() const
        {
            return animationFrameNumber;
        }

        void setAnimationFrameNumber(int n)
        {
            animationFrameNumber = n;
        }

        void setMovingItemIndex(int n)
        {
            movingItemIndex = n;
        }

        void setDirection(int n)
        {
            direction = n;
        }

        int getDirection() const
        {
            return direction;
        }

        int getMovingItemIndex() const
        {
            return movingItemIndex;
        }

        void setAlgo(AlgoView *algo) {
            this->algo = algo;
        }

    protected:
    private:
        void setTopLefts(int w);
        int animationFrameNumber;
        int movingItemIndex;
        int direction;
        wxPanel *m_parent;
        wxBitmap m_bufferBitmap;
        Board board;
        int topLefts[9][2];
        AlgoView *algo;
        wxImage *bgImage;
        wxBitmap *bgBitmap;
        //wxBitmap bgSubBitmaps[9];
        static wxPoint *bgTopLefts;

        static wxPoint *initBgTopLefts();
        void OnClick(wxMouseEvent &event);
        DECLARE_EVENT_TABLE()
};

#endif // BOARDPANEL_H
