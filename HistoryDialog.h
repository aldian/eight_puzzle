#ifndef HISTORYDIALOG_H
#define HISTORYDIALOG_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "business/board.h"
#include "HistoryBoard.h"

class HistoryDialog : public wxDialog
{
    public:
        HistoryDialog(wxWindow *parent, const vector<Board> &boards);
        virtual ~HistoryDialog();
    protected:
    private:
        const vector<Board> &boards;
        wxBoxSizer *topSizer;

DECLARE_EVENT_TABLE()
};

#endif // HISTORYDIALOG_H
