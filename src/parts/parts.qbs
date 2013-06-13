import qbs.base 1.0
import "../library.qbs" as Library

Library {
    name: "Parts"

    Depends { name: "Qt"; submodules: ["core", "widgets", "network"] }
    Depends { name: "IO" }
    Depends { name: "Widgets" }

    cpp.includePaths : [ "../../include", ".", "./core", "./widgets", product.buildDirectory ]
//    cpp.cxxFlags : {
//        if (qbs.toolchain == "gcc" || qbs.toolchain == "mingw" || qbs.toolchain == "clang")
//            return [ "-std=c++11", "-stdlib=libstdc++" ];
//    }
    cpp.defines: [ "PARTS_LIBRARY" ]

    files : [
        "actionmanager/abstractcommand.cpp",
        "actionmanager/abstractcommand.h",
        "actionmanager/abstractcommand_p.h",
        "actionmanager/actionmanager.cpp",
        "actionmanager/actionmanager.h",
        "actionmanager/command.cpp",
        "actionmanager/command.h",
        "actionmanager/commandcontainer.cpp",
        "actionmanager/commandcontainer.h",
        "actionmanager/commandsmodel.cpp",
        "actionmanager/commandsmodel.h",
        "actionmanager/commandsmodel_p.h",
        "actionmanager/commandssettingswidget.cpp",
        "actionmanager/commandssettingswidget.h",
        "actionmanager/commandssettingswidget.ui",
        "actionmanager/commandssettingswidget_p.h",
        "actionmanager/proxyaction.cpp",
        "actionmanager/proxyaction.h",
        "actionmanager/standardcommands.cpp",
        "actionmanager/standardcommands.h",
        "actionmanager/standardcommands_p.h",
        "core/abstractdocument.cpp",
        "core/abstractdocument.h",
        "core/abstractdocument_p.h",
        "core/abstractdocumentfactory.cpp",
        "core/abstractdocumentfactory.h",
        "core/documentmanager.cpp",
        "core/documentmanager.h",
        "core/filedocument.cpp",
        "core/filedocument.h",
        "core/filedocument_p.h",
        "core/history.cpp",
        "core/history.h",
        "core/historyitem.cpp",
        "core/historyitem.h",
        "core/ifind.cpp",
        "core/ifind.h",
        "core/ihistory.cpp",
        "core/ihistory.h",
        "core/sharedproperties.cpp",
        "core/sharedproperties_p.h",
        "core/sharedproperties.h",
        "core/toolmodel.cpp",
        "core/toolmodel.h",
        "widgets/abstracteditor.cpp",
        "widgets/abstracteditor.h",
        "widgets/abstracteditorfactory.cpp",
        "widgets/abstracteditorfactory.h",
        "widgets/editormanager.cpp",
        "widgets/editormanager.h",
        "widgets/editorview.cpp",
        "widgets/editorview_p.h",
        "widgets/editorview.h",
        "widgets/editorviewhistory.cpp",
        "widgets/editorviewhistory.h",
        "widgets/editorviewhistory_p.h",
        "widgets/editorwindow.cpp",
        "widgets/editorwindow.h",
        "widgets/editorwindow_p.h",
        "widgets/editorwindowfactory.cpp",
        "widgets/editorwindowfactory.h",
        "widgets/findtoolbar.cpp",
        "widgets/findtoolbar.h",
        "widgets/historybutton.cpp",
        "widgets/historybutton.h",
        "widgets/toolwidget.cpp",
        "widgets/toolwidget.h",
        "widgets/toolwidgetfactory.cpp",
        "widgets/toolwidgetfactory.h",
        "widgets/toolwidgetmanager.cpp",
        "widgets/toolwidgetmanager.h",
        "parts.qrc",
        "parts_global.h",
        "settingspage.cpp",
        "settingspage.h",
        "settingspagemanager.cpp",
        "settingspagemanager.h",
        "settingswindow.cpp",
        "settingswindow.h"
    ]
}
