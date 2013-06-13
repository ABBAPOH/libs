#ifndef STANDARDCOMMANDS_H
#define STANDARDCOMMANDS_H

#include "../parts_global.h"

#include <QObject>

namespace Parts {

class Command;

namespace StandardCommands {

enum StandardCommand {
    NoCommand = -1,
    // File Menu
    New, NewTab, NewWindow, Open, /*OpenRecent,*/ Save, SaveAs, /*Revert,*/
    Close, Refresh, Cancel, /*Print, PrintPreview, Mail,*/ Quit,

    // Edit Menu
    Undo, Redo, Cut, Copy, Paste, /*PasteText,*/ Clear, SelectAll, Deselect,
    Find, FindNext, FindPrev, /*Replace,*/ Preferences,

    // View Menu
    //        ActualSize,
    //        FitToPage,
    //        FitToWidth,
    //        FitToHeight,
    //        ZoomIn,
    //        ZoomOut,
    //        Zoom,
    //        Redisplay,
    ShowMenubar,
    ShowToolBar,
    ShowStatusBar,
    ShowLeftPanel,
    FullScreen,

    // Go Menu
    Up,
    Back,
    Forward,
    //        Home,
    //        Prior,
    //        Next,
    //        Goto,
    //        GotoPage,
    //        GotoLine,
    //        FirstPage,
    //        LastPage,
    //        DocumentBack,
    //        DocumentForward,

    // Bookmarks Menu
    AddBookmark, EditBookmarks,

    //        SaveOptions,
    //        KeyBindings,
    //        ConfigureToolbars,

    // Help menu
    Help,
    //        HelpContents,
    //        WhatsThis,
    About,
    AboutQt,
    //        SwitchApplicationLanguage,
    StandardCommandCount
};

PARTS_EXPORT Command *standardCommand(StandardCommand command);
PARTS_EXPORT void retranslateCommands();

} // namespace StandardCommands

} // namespace Parts

#endif // STANDARDCOMMANDS_H
