import qbs.base 1.0

Product {
    type: "application"
    destinationDirectory: project.install_binary_path
    consoleApplication: true

    Depends { name: "cpp" }
    Depends { name: "Qt"; submodules: ["core", "test"] }

    Properties {
         condition: qbs.targetOS.contains("linux") || qbs.targetOS.contains("unix")
         cpp.rpaths: [ "$ORIGIN/../lib" + project.lib_suffix + "/" + project.app_target ]
    }

    cpp.includePaths : "../../../include"
}
