import qbs.base 1.0
import "../library.qbs" as Library

Library {
    name : "ImageViewer"

    Depends { name: "Qt"; submodules: ["core", "widgets", "opengl"] }
    Depends { name: "Widgets" }

    cpp.defines: [ "IMAGEVIEWER_LIBRARY" ]

    files : [
        "imageview.cpp",
        "imageview.h",
        "imageview_p.h",
        "imageviewer_global.h",
        "imageviewsettings.cpp",
        "imageviewsettings.h",
        "imageviewsettings_p.h",
        "preferenceswidget.cpp",
        "preferenceswidget.h",
        "preferenceswidget.ui",
        "resizedialog.cpp",
        "resizedialog.h",
        "resizedialog.ui"
    ]
}
