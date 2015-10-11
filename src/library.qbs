import qbs.base 1.0

DynamicLibrary {
    destinationDirectory: install_library_path

    Depends { name: "cpp" }
    cpp.includePaths : "../../include"
    cpp.dynamicLibraryPrefix: "lib"

    bundle.isBundle: false

    cpp.installNamePrefix: "@rpath/Frameworks"
    cpp.rpaths: qbs.targetOS.contains("osx")
                ? [ "@loader_path/..", "@executable_path/.." ]
                : [ "$ORIGIN" ]

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: project.install_library_path
    }
}
