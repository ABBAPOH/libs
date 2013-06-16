import qbs.base 1.0
import "../../test.qbs" as Test

Test {
    name : "tst_actionmanager"
    Depends { name: "Qt.widgets" }
    Depends { name: "Parts" }
    files : "tst_actionmanager.cpp"
}
