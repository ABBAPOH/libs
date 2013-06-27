import qbs.base 1.0
import "../library.qbs" as Library

Library {
    name : "IO"

    Depends { id: qtcore; name: "Qt.core" }
    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends {
        name: "Qt";
        submodules: qbs.targetOS == "unix" || qbs.targetOS == "linux" ?
                        "dbus" :
                        undefined
    }

    Properties {
        condition: qbs.targetOS.contains("osx")
        cpp.frameworks: [
            "CoreServices",
            "DiskArbitration",
            "IOKit",
            "QuickLook",
            "AppKit"
        ]
    }

    Properties {
        condition: qbs.targetOS.contains("windows")
        cpp.dynamicLibraries: [
            "Advapi32",
            "Mpr",
            "Netapi32",
            "Shell32",
            "Userenv",
            "User32",
            "Winmm"
        ]
    }

    cpp.defines: [ "IO_LIBRARY" ]

    files : [
        "inqt5/qsystemerror.cpp",
        "inqt5/qsystemerror_p.h",
        "directorydetails.cpp",
        "directorydetails.h",
        "fileimageprovider.cpp",
        "fileimageprovider.h",
        "io.qrc",
        "io_global.h",
        "kdesettings.cpp",
        "kdesettings.h",
        "qdefaultprogram.cpp",
        "qdefaultprogram.h",
        "qdefaultprogram_p.h",
        "qdrivecontroller.cpp",
        "qdrivecontroller.h",
        "qdrivecontroller_p.h",
        "qdriveinfo.cpp",
        "qdriveinfo.h",
        "qdriveinfo_p.h",
        "qfilecopier.cpp",
        "qfilecopier.h",
        "qfilecopier_p.h",
        "qtrash.cpp",
        "qtrash.h",
        "qtrash_p.h",
        "qtrashfileinfo.cpp",
        "qtrashfileinfo.h",
        "qtrashfileinfo_p.h",
        "qxdg.cpp",
        "qxdg.h"
    ]

    Group {
        name : "linux files"
        condition: qbs.targetOS === "linux"
        files: [
            "qdefaultprogram_linux.cpp",
            "qdrivecontroller_linux.cpp",
            "qdriveinfo_linux.cpp",
            "qtrash_linux.cpp"
        ]
    }
    Group {
        name : "mac files"
        condition: qbs.targetOS.contains("osx")
        files: [
            "qdefaultprogram_mac.mm",
            "qdrivecontroller_mac.cpp",
            "qdriveinfo_mac.cpp",
            "qtrash_mac.mm"
        ]
    }

    Group {
        name : "windows files"
        condition: qbs.targetOS.contains("windows")
        files: [
            "info2.cpp",
            "info2_p.h",
            "qdefaultprogram_win.cpp",
            "qdrivecontroller_win.cpp",
            "qdriveinfo_win.cpp",
            "qtrash_win.cpp"
        ]
    }

    Group {
        name : "inqt5"
        condition : qtcore.versionMajor < 5
        files : [
            "inqt5/qmimedatabase.cpp",
            "inqt5/qmimedatabase.h",
            "inqt5/qmimedatabase_p.h",
            "inqt5/qmimeglobpattern.cpp",
            "inqt5/qmimeglobpattern_p.h",
            "inqt5/qmimemagicrule.cpp",
            "inqt5/qmimemagicrule_p.h",
            "inqt5/qmimemagicrulematcher.cpp",
            "inqt5/qmimemagicrulematcher_p.h",
            "inqt5/qmimeprovider.cpp",
            "inqt5/qmimeprovider_p.h",
            "inqt5/qmimetype.cpp",
            "inqt5/qmimetype.h",
            "inqt5/qmimetype_p.h",
            "inqt5/qmimetypeparser.cpp",
            "inqt5/qmimetypeparser_p.h",
            "inqt5/qstandardpaths.cpp",
            "inqt5/qstandardpaths.h",
        ]
    }
    Group {
        name : "intq5 mac files"
        condition: qbs.targetOS.contains("osx") && qtcore.versionMajor < 5
        files: [
            "inqt5/qstandardpaths_mac.cpp"
        ]
    }
    Group {
        name : "inqt5 unix files"
        condition: (qbs.targetOS === "unix" || qbs.targetOS === "linux") && qtcore.versionMajor < 5
        files: [
            "inqt5/qstandardpaths_unix.cpp"
        ]
    }
    Group {
        name : "inqt5 windows files"
        condition: qbs.targetOS.contains("windows") && qtcore.versionMajor < 5
        files: [
            "inqt5/qstandardpaths_win.cpp"
        ]
    }
}
