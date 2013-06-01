import qbs.base 1.0

Product {
    type: "qm"
    name: "translations-libs"
    Depends { name: "Qt.core" }
    files: [
        "src/extensionsystem/translations/*.ts",
        "src/io/translations/*.ts",
        "src/parts/translations/*.ts",
        "src/widgets/translations/*.ts"
    ]

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: install_data_path + "/translations"
    }
}
