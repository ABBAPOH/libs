import qbs.base 1.0

StaticLibrary {
    name: "QtSingleApplication"
    Depends { name: "cpp" }
    Depends { id: qtcore; name: "Qt.core" }

    cpp.includePaths: "qtlockedfile"

    Group {
        name : "QtSingleApplication";
        condition : qtcore.versionMajor < 5
        files : [
            "qtlockedfile/qtlockedfile.cpp",
            "qtlockedfile/qtlockedfile.h",
            "qtsingleapplication/qtlocalpeer.cpp",
            "qtsingleapplication/qtlocalpeer.h",
            "qtsingleapplication/qtsingleapplication.cpp",
            "qtsingleapplication/qtsingleapplication.h",
            "qtsingleapplication/qtsinglecoreapplication.cpp",
            "qtsingleapplication/qtsinglecoreapplication.h"
        ]
    }
    Group {
        name : "unix files";
        condition : qtcore.versionMajor < 5 &&
                    (qbs.targetOS === "unix" || qbs.targetOS === "mac" || qbs.targetOS === "linux")
        files : "qtlockedfile/qtlockedfile_unix.cpp"
    }
    Group {
        name : "windows files";
        condition : qtcore.versionMajor < 5 && qbs.targetOS === "windows"
        files : "qtlockedfile/qtlockedfile_win.cpp"
    }
}
