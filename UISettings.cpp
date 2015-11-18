#include "UISettings.h"

const TCHAR *UISettings::APP_TITLE = _("8 Puzzle Solver & Game");
const TCHAR *UISettings::BG_FILE_NAME = _("Penguin.bmp");
const wxBitmapType UISettings::BG_BITMAP_TYPE = wxBITMAP_TYPE_BMP;
const int UISettings::EXPECTED_FPS = 30;
const int UISettings::BOARD_WIDTH = 300;
const int UISettings::LOG_WIDTH = 200;
const int UISettings::FONT_SIZE = 24;

UISettings::UISettings()
{
    //ctor
}

UISettings::~UISettings()
{
    //dtor
}
