#include <QtCore/QString>
#include <QtTest/QtTest>

#include <FileManager/FileManagerModel>

using namespace FileManager;

class FileManagerModelTest : public QObject
{
    Q_OBJECT

public:
    FileManagerModelTest();

private Q_SLOTS:
    void testDefaultValues();
    void testState();
};

FileManagerModelTest::FileManagerModelTest()
{
}

void FileManagerModelTest::testDefaultValues()
{
    FileManagerModel model;
    QCOMPARE(model.url(), QUrl::fromLocalFile(QDir::homePath()));
    QCOMPARE(model.showHiddenFiles(), false);
    QCOMPARE(model.sortColumn(), FileManagerModel::NameColumn);
    QCOMPARE(model.sortOrder(), Qt::AscendingOrder);
}

void FileManagerModelTest::testState()
{
    FileManagerModel model;
    model.setUrl(QUrl::fromLocalFile(QDir::rootPath()));
    model.setShowHiddenFiles(true);
    model.setSortColumn(FileManagerModel::DateColumn);
    model.setSortOrder(Qt::DescendingOrder);

    QByteArray state = model.saveState();
    FileManagerModel model2;
    QVERIFY(model2.restoreState(state));

    QCOMPARE(model2.url(), model.url());
    QCOMPARE(model2.showHiddenFiles(), model.showHiddenFiles());
    QCOMPARE(model2.sortColumn(), model.sortColumn());
    QCOMPARE(model2.sortOrder(), model.sortOrder());

    // test empty state
    state.clear();
    QVERIFY(!model2.restoreState(state));
    // test no data
    state = "F15l\1";
    QVERIFY(!model2.restoreState(state));
    // test invalid version
    state = "F15l\2";
    QVERIFY(!model2.restoreState(state));
    // test invalid magic
    state = "F15r";
    QVERIFY(!model2.restoreState(state));
}

QTEST_MAIN(FileManagerModelTest)

#include "tst_filemanagermodel.moc"
