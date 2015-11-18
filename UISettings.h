#ifndef UISETTINGS_H
#define UISETTINGS_H

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <tchar.h>

class UISettings
{
    public:
        static const TCHAR *APP_TITLE;
        static const TCHAR *BG_FILE_NAME;
        static const wxBitmapType BG_BITMAP_TYPE;
        static const int EXPECTED_FPS;
        static const int BOARD_WIDTH;
        static const int LOG_WIDTH;
        static const int FONT_SIZE;
        UISettings();
        virtual ~UISettings();
    protected:
    private:
};

#endif // UISETTINGS_H
