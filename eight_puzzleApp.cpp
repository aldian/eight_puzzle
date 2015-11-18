/***************************************************************
 * Name:      eight_puzzleApp.cpp
 * Purpose:   Code for Application Class
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

#include "UISettings.h"
#include "eight_puzzleApp.h"
#include "eight_puzzleMain.h"

IMPLEMENT_APP(eight_puzzleApp);

bool eight_puzzleApp::OnInit()
{

    eight_puzzleDialog* dlg = new eight_puzzleDialog(UISettings::APP_TITLE);
    dlg->SetIcon(wxICON(aaaa)); // To Set App Icon
    dlg->Show(true);
    return true;
}
