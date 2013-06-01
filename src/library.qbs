import qbs.base 1.0

DynamicLibrary {
    destinationDirectory: install_library_path

    Depends { name: "cpp" }
    cpp.includePaths : "../../include"
    cpp.dynamicLibraryPrefix: "lib"

    Properties {
        condition: qbs.targetOS == "mac"
        cpp.installNamePrefix: project.installNamePrefix
    }

    Properties {
         condition: qbs.targetOS == "linux" || qbs.targetOS == "unix"
         cpp.rpaths: "$ORIGIN"
    }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: project.install_library_path
    }
}
