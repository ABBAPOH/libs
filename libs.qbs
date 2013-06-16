import qbs.base 1.0

Project {

    property string app_target: "andromeda-libs"
    property string install_binary_path: "bin"
    property string lib_suffix: qbs.targetOS == "linux" ? (qbs.architecture == "x86_64" ? "64" : "") : ""
    property string install_library_path: {
        if (qbs.targetOS == "linux")
            return "lib" + lib_suffix + "/" + app_target
        else
            return "lib" + "/" + app_target
    }
    property string install_data_path: "share/" + app_target
    property string installNamePrefix: "@executable_path/../" + install_library_path + "/"

    references: [
        "src/3rdparty/qtsingleapplication/qtsingleapplication.qbs",
        "src/extensionsystem/extensionsystem.qbs",
        "src/filemanager/filemanager.qbs",
        "src/imageviewer/imageviewer.qbs",
        "src/io/io.qbs",
        "src/parts/parts.qbs",
        "src/widgets/widgets.qbs",
        "tests/extensionsystem/pluginspec/pluginspec.qbs",
        "tests/filemanager/filemanagerwidget/filemanagerwidget.qbs",
        "tests/io/kdesettings/kdesettings.qbs",
        "tests/parts/actionmanager/actionmanager.qbs",
        "tests/parts/sharedproperties/sharedproperties.qbs"
    ]
}
