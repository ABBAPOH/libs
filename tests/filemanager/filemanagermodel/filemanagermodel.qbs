import qbs.base 1.0
import "../../test.qbs" as Test

Test {
    name : "tst_filemanagermodel"
    Depends { name: "Qt"; submodules: ["widgets"] }
    Depends { name: "FileManager" }
    files : "tst_filemanagermodel.cpp"
}
