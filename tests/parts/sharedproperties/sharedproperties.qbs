import qbs.base 1.0
import "../../test.qbs" as Test

Test {
    name : "tst_sharedproperties"
    Depends { name: "Parts" }
    files : "tst_sharedproperties.cpp"
}
