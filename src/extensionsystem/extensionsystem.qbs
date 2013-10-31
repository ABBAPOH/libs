import qbs.base 1.0
import "../library.qbs" as Library

Library {
    name: "ExtensionSystem"

    Depends { name: "Qt"; submodules: ["core", "widgets"] }

    cpp.defines: [ "EXTENSIONSYSTEM_LIBRARY" ]

    files : [
        "core/iplugin.cpp",
        "core/iplugin.h",
        "core/mutablepluginspec.cpp",
        "core/mutablepluginspec.h",
        "core/pluginmanager.cpp",
        "core/pluginmanager.h",
        "core/pluginmanager_p.h",
        "core/pluginspec.cpp",
        "core/pluginspec.h",
        "core/pluginspec_p.h",
        "core/pluginspecformathandler.cpp",
        "core/pluginspecformathandler_p.h",
        "core/pluginviewmodel.cpp",
        "core/pluginviewmodel.h",
        "core/pluginviewmodel_p.h",
        "core/qobjectpool.cpp",
        "core/qobjectpool.h",
        "core/qobjectpool_p.h",
        "widgets/errorsdialog.cpp",
        "widgets/errorsdialog.h",
        "widgets/fullpluginview.cpp",
        "widgets/fullpluginview.h",
        "widgets/fullpluginview.ui",
        "widgets/plugineditor.cpp",
        "widgets/plugineditor.h",
        "widgets/plugineditor.ui",
        "widgets/pluginview.cpp",
        "widgets/pluginview.h",
        "widgets/pluginview.ui",
        "extensionsystem_global.h"
    ]
}
