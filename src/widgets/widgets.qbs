import qbs.base 1.0
import "../library.qbs" as Library

Library {
    name : "Widgets"

    Depends { name: "Qt"; submodules: ["core", "widgets", "opengl"] }

    cpp.defines: [ "WIDGETS_LIBRARY" ]

    files : [
        "addressbar.cpp",
        "addressbar.h",
        "fancylineedit.cpp",
        "fancylineedit.h",
        "fancylineedit_p.h",
        "filterlineedit.cpp",
        "filterlineedit.h",
        "minisplitter.cpp",
        "minisplitter.h",
        "modelmenu.cpp",
        "modelmenu.h",
        "modeltoolbar.cpp",
        "modeltoolbar.h",
        "outlinewidget.cpp",
        "outlinewidget.h",
        "outlinewidget_p.h",
        "qcolorbutton.cpp",
        "qcolorbutton.h",
        "qcolorbutton_p.h",
        "shortcutedit.cpp",
        "shortcutedit.h",
        "shortcutedit_p.h",
        "tabbar.cpp",
        "tabbar.h",
        "tabbarbutton.cpp",
        "tabbarbutton.h",
        "tabwidget.cpp",
        "tabwidget.h",
        "widgets.qrc",
        "widgets_global.h",
        "windowsmenu.cpp",
        "windowsmenu.h"
    ]
}
