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

    SubProject {
        filePath: "src/src.qbs"
    }
    SubProject {
        filePath: "tests/tests.qbs"
    }
}
