import qbs.base 1.0

Project {
    references: [
        "3rdparty/qtsingleapplication/qtsingleapplication.qbs",
        "bookmarks/bookmarks.qbs",
        "extensionsystem/extensionsystem.qbs",
        "filemanager/filemanager.qbs",
        "imageviewer/imageviewer.qbs",
        "io/io.qbs",
        "parts/parts.qbs",
        "widgets/widgets.qbs"
    ]

    Product {
        name: "libs-translations"
        type: "qm"
        Depends { name: "Qt.core" }
        files: [
            "extensionsystem/translations/*.ts",
            "filemanager/translations/*.ts",
            "imageviewer/translations/*.ts",
            "io/translations/*.ts",
            "parts/translations/*.ts",
            "widgets/translations/*.ts"
        ]

        Group {
            fileTagsFilter: product.type
            qbs.install: true
            qbs.installDir: install_data_path + "/translations"
        }
    }
}
