#include <QtCore/QString>
#include <QtTest/QtTest>

#include <QtGui/QStandardItemModel>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif

#include <Parts/AbstractCommand>
#include <Parts/ApplicationCommand>
#include <Parts/Command>
#include <Parts/ModelContainer>

using namespace Parts;

class AbstractCommandImpl : public AbstractCommand
{
public:
    AbstractCommandImpl() : AbstractCommand("id") {}

    QAction *createAction(QObject * /*parent*/) const { return 0; }
};

class CommandImpl : public Command
{
public:
    CommandImpl() : Command("id") {}

    QAction *createAction(QObject * /*parent*/) const { return 0; }
};

class ActionManagerTest : public QObject
{
    Q_OBJECT

public:
    ActionManagerTest();

private Q_SLOTS:
    void testAbstractCommandAPI();
    void testCommandAPI();
    void testModelCommand();
    void testApplicationCommandAPI();
};

ActionManagerTest::ActionManagerTest()
{
}

void ActionManagerTest::testAbstractCommandAPI()
{
    AbstractCommandImpl cmd;

    QCOMPARE(cmd.text(), QString());
    QVERIFY(cmd.icon().isNull());

    cmd.setText(QLatin1String("text"));
    QCOMPARE(cmd.text(), QString(QLatin1String("text")));
}

void ActionManagerTest::testCommandAPI()
{
    CommandImpl cmd;

    QCOMPARE(cmd.isConfigurable(), true);
    QVERIFY(cmd.data().isNull());
    QCOMPARE(cmd.defaultShortcut(), QKeySequence());
    QCOMPARE(cmd.shortcut(), QKeySequence());
    QCOMPARE(cmd.toolTip(), QString());

    cmd.setData(QString("data"));
    QCOMPARE(cmd.data().toString(), QString("data"));

    cmd.setDefaultShortcut(QKeySequence("Ctrl+N"));
    QCOMPARE(cmd.defaultShortcut(), QKeySequence("Ctrl+N"));
    QCOMPARE(cmd.shortcut(), QKeySequence("Ctrl+N"));

    cmd.setShortcut(QKeySequence("Ctrl+T"));
    QCOMPARE(cmd.shortcut(), QKeySequence("Ctrl+T"));
    QCOMPARE(cmd.defaultShortcut(), QKeySequence("Ctrl+N"));

    cmd.setDefaultShortcut(QKeySequence("Ctrl+O"));
    QCOMPARE(cmd.defaultShortcut(), QKeySequence("Ctrl+O"));
    QCOMPARE(cmd.shortcut(), QKeySequence("Ctrl+T"));

    cmd.setToolTip(QString("ToolTip"));
    QCOMPARE(cmd.toolTip(), QString("ToolTip"));

    cmd.setConfigurable(false);
    QCOMPARE(cmd.isConfigurable(), false);
    QCOMPARE(cmd.shortcut(), QKeySequence("Ctrl+O"));

    cmd.setShortcut(QKeySequence("Ctrl+P"));
    QCOMPARE(cmd.shortcut(), QKeySequence("Ctrl+O"));
}

void ActionManagerTest::testModelCommand()
{
    ModelContainer container("id");

    QStandardItemModel model;
    for (int i = 0; i < 10; ++i) {
        QStandardItem *item = new QStandardItem(QString("row %1").arg(i));
        model.appendRow(item);
    }

    QStandardItem *rootItem = new QStandardItem(QString("row 11"));
    for (int i = 0; i < 10; ++i) {
        QStandardItem *item = new QStandardItem(QString("Child row %1").arg(i));
        rootItem->appendRow(item);
    }
    model.appendRow(rootItem);

    container.setModel(&model);

    QVERIFY(container.commands().count() == 12);
    for (int i = 0; i < 10; ++i) {
        QCOMPARE(container.commands().at(i+1)->text(), QString("row %1").arg(i));
    }

    QCOMPARE(container.commands().at(11)->text(), QString("row 11"));

    CommandContainer *rootContainer = qobject_cast<CommandContainer *>(container.commands().at(11));
    QVERIFY(rootContainer);
    for (int i = 0; i < 10; ++i) {
        QCOMPARE(rootContainer->commands().at(i+1)->text(), QString("Child row %1").arg(i));
    }

    for (int i = 0; i < 4; ++i) {
        QStandardItem *item = new QStandardItem(QString("new row %1").arg(i));
        model.insertRow(5+i, item);
    }

    for (int i = 0; i < 4; ++i) {
        QCOMPARE(container.commands().at(5 + i + 1)->text(), QString("new row %1").arg(i));
    }
}

void ActionManagerTest::testApplicationCommandAPI()
{
    ApplicationCommand cmd("id");
    QAction *action = const_cast<QAction *>(cmd.action());

    QCOMPARE(action->shortcut(), QKeySequence());
    QCOMPARE(action->text(), QString());
    QCOMPARE(action->toolTip(), QString());

    cmd.setText(QLatin1String("text"));
    QCOMPARE(action->text(), QString(QLatin1String("text")));
}

QTEST_MAIN(ActionManagerTest)

#include "tst_actionmanager.moc"
