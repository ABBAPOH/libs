import qbs.base 1.0
import "../library.qbs" as Library

Library {
    name : "FileManager"

    Depends { name: "Qt"; submodules: ["core", "widgets"] }
    Depends { name: "IO" }
    Depends { name: "Widgets" }

    cpp.defines: [ "FILEMANAGER_LIBRARY" ]

    files : [
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
        "filemanagerhistory.cpp",
        "filemanagerhistory.h",
        "filemanagerhistory_p.h",
        "filemanagerhistoryitem.cpp",
        "filemanagerhistoryitem.h",
        "filemanagerhistoryitem_p.h",
        "filemanagersettings.cpp",
        "filemanagersettings.h",
        "filemanagersettings_p.h",
        "filemanagerwidget.cpp",
        "filemanagerwidget.h",
        "filemanagerwidget_p.h",
        "filesystemmanager.cpp",
        "filesystemmanager.h",
        "filesystemmanager_p.h",
        "filesystemmodel.cpp",
        "filesystemmodel.h",
        "filethumbnails.cpp",
        "filethumbnails.h",
        "navigationmodel.cpp",
        "navigationmodel.h",
        "navigationmodel_p.h",
        "navigationpanel.cpp",
        "navigationpanel.h",
        "navigationpanel_p.h",
        "navigationpanelsettings.cpp",
        "navigationpanelsettings.h",
        "openwithmenu.cpp",
        "openwithmenu.h",
        "permissionswidget.cpp",
        "permissionswidget.h",
        "permissionswidget.ui"
    ]
}
