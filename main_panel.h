#ifndef MAINPANEL_H
#define MAINPANEL_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/statline.h>

class MainPanel: public wxPanel
{
    public:
        //MainPanel(wxFrame *frame, int x, int y, int w, int h);
        MainPanel(wxFrame *frame);
        virtual ~MainPanel();

        void OnPaint(wxPaintEvent &event);

        wxPanel *getBoardPanel()
        {
            return boardPanel;
        }

        wxTextCtrl *getSolutionLog()
        {
            return solutionLog;
        }

        wxFrame *getFrame()
        {
            return frame;
        }

        //wxStaticText *getMessageContainer()
        //{
        //    return messageContainer;
        //}

        wxBoxSizer *getTopSizer()
        {
            return topSizer;
        }

        int getBgYOffset() const
        {
            return bgYOffset;
        }

        void setBgYOffset(int y)
        {
            bgYOffset = y;
        }
    protected:
        enum
        {
            idBtnQuit = 1000,
            idBtnGeneratePuzzle,
            idBtnSolvePuzzle,
            idBtnStopPuzzle,
            idBtnAbout
        };
        //wxStaticText* m_staticText1;
        //wxButton* BtnAbout;
        //wxStaticLine* m_staticline1;
        //wxButton* BtnQuit;
    private:
        wxImage *bgImage;
        wxBitmap *bgBitmap;
        wxBitmap m_bufferBitmap;
        wxFrame *frame;
        wxBoxSizer* topSizer;
        wxPanel* boardPanel;
        wxTextCtrl *solutionLog;
        //wxStaticText *messageContainer;
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnGeneratePuzzle(wxCommandEvent& event);
        void OnSolvePuzzle(wxCommandEvent& event);
        void OnStopPuzzle(wxCommandEvent& event);

        int bgYOffset;
        DECLARE_EVENT_TABLE()
};

#endif // MAINPANEL_H
