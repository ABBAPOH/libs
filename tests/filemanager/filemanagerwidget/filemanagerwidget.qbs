import qbs.base 1.0
import "../../test.qbs" as Test

Test {
    name : "tst_filemanagerwidget"
    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "FileManager" }
  files : "tst_filemanagerwidget.cpp"
}
