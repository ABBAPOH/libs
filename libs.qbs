import qbs.base 1.0

Project {

    property string app_target: "andromeda-libs"
    property string install_binary_path: "bin"
    property string lib_suffix: ""
    property string install_library_path: "lib" + lib_suffix + "/" + app_target
    property string install_data_path: "share/" + app_target
    property string installNamePrefix: "@executable_path/../" + install_library_path + "/"

    SubProject {
        filePath: "src/src.qbs"
    }
    SubProject {
        filePath: "tests/tests.qbs"
    }
}
