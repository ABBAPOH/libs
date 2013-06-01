import qbs.base 1.0
import "../../test.qbs" as Test

Test {
    name : "tst_kdesettings"
    Depends { name: "IO" }
    files : "tst_kdesettings.cpp"
}
