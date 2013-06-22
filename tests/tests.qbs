import qbs.base 1.0

Project {
    references: [
        "extensionsystem/pluginspec/pluginspec.qbs",
        "filemanager/filemanagerwidget/filemanagerwidget.qbs",
        "filemanager/filemanagermodel/filemanagermodel.qbs",
        "io/kdesettings/kdesettings.qbs",
        "parts/actionmanager/actionmanager.qbs",
        "parts/sharedproperties/sharedproperties.qbs"
    ]
}
