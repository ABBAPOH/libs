import qbs.base 1.0

Product {
    type: "application"
    destinationDirectory: install_binary_path

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "test"] }

    Properties {
         condition: qbs.targetOS == "linux" || qbs.targetOS == "unix"
         cpp.rpaths: [ "$ORIGIN/../lib" + project.lib_suffix + "/" + project.app_target ]
    }

    cpp.includePaths : "../../../include"
}
