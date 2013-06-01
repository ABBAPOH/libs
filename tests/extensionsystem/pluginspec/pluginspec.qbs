import qbs.base 1.0
import "../../test.qbs" as Test

Test {
    name : "tst_pluginspec"
    Depends { name: "ExtensionSystem" }
    files : "tst_pluginspec.cpp"
}
