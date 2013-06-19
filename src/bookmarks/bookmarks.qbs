import qbs.base 1.0
import "../library.qbs" as Library

Library {
    name : "Bookmarks"

    Depends { name: "Qt"; submodules: ["core", "widgets"] }
    Depends { name: "IO" }
    Depends { name: "Widgets" }
    cpp.defines: [ "BOOKMARKS_LIBRARY" ]

    files : [
        "core/bookmark.cpp",
        "core/bookmark.h",
        "core/bookmarksmodel.cpp",
        "core/bookmarksmodel.h",
        "core/bookmarksmodel_p.h",
        "widgets/bookmarkdialog.cpp",
        "widgets/bookmarkdialog.h",
        "bookmarks_global.h",
        "widgets/bookmarksmenu.cpp",
        "widgets/bookmarksmenu.h",
        "widgets/bookmarkstoolbar.cpp",
        "widgets/bookmarkstoolbar.h",
        "widgets/bookmarkswidget.cpp",
        "widgets/bookmarkswidget.h",
        "widgets/bookmarkswidget_p.h"
    ]
}
