import qbs.base 1.0
import "../library.qbs" as Library

Library {
    name : "FileManager"

    Depends { name: "Qt"; submodules: ["core", "widgets"] }
    Depends { name: "IO" }
    Depends { name: "Widgets" }

    cpp.defines: [ "FILEMANAGER_LIBRARY" ]
    cpp.includePaths: ["../../include", "core"]

    files : [
        "core/filemanagerhistory.cpp",
        "core/filemanagerhistory.h",
        "core/filemanagerhistory_p.h",
        "core/filemanagerhistoryitem.cpp",
        "core/filemanagerhistoryitem.h",
        "core/filemanagerhistoryitem_p.h",
        "core/filemanagermodel.cpp",
        "core/filemanagermodel.h",
        "core/filemanagermodel_p.h",
        "core/filemanagersettings.cpp",
        "core/filemanagersettings.h",
        "core/filemanagersettings_p.h",
        "core/filesystemmodel.cpp",
        "core/filesystemmodel.h",
        "core/filethumbnails.cpp",
        "core/filethumbnails.h",
        "core/filesystemmanager.cpp",
        "core/filesystemmanager.h",
        "core/filesystemmanager_p.h",
        "core/navigationmodel.cpp",
        "core/navigationmodel.h",
        "core/navigationmodel_p.h",
        "filecopydialog.cpp",
        "filecopydialog.h",
        "filecopydialog_p.h",
        "filecopyerrordialog.cpp",
        "filecopyerrordialog.h",
        "filecopyerrordialog.ui",
        "filecopyreplacedialog.cpp",
        "filecopyreplacedialog.h",
        "filecopyreplacedialog.ui",
        "filecopytask.cpp",
        "filecopytask.h",
        "filecopytask_p.h",
        "filecopywidget.ui",
        "fileexplorerwidget.cpp",
        "fileexplorerwidget.h",
        "fileinfodialog.cpp",
        "fileinfodialog.h",
        "fileitemdelegate.cpp",
        "fileitemdelegate.h",
        "fileitemdelegate_p.h",
        "filemanager.qrc",
        "filemanager_global.h",
        "filemanagerconstants.h",
        "filemanagerwidget.cpp",
        "filemanagerwidget.h",
        "filemanagerwidget_p.h",
        "navigationpanel.cpp",
        "navigationpanel.h",
        "navigationpanel_p.h",
        "openwithmenu.cpp",
        "openwithmenu.h",
        "permissionswidget.cpp",
        "permissionswidget.h",
        "permissionswidget.ui"
    ]
}
