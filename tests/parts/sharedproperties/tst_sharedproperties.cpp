#include <QtCore/QString>
#include <QtTest/QtTest>

#include <Parts/SharedProperties>

using namespace Parts;

class Object : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString property READ property WRITE setProperty NOTIFY propertyChanged)
    Q_PROPERTY(QString propertyNoNotify READ property WRITE setProperty)
public:
    explicit Object(QObject *parent = 0) : QObject(parent) {}

    QString property() const { return m_property; }
    void setProperty(const QString &property)
    {
        if (m_property != property) {
            m_property = property;
            emit propertyChanged(property);
        }
    }

signals:
    void propertyChanged(const QString &);

private:
    QString m_property;
};

class SharedPropertiesTest : public QObject
{
    Q_OBJECT

public:
    SharedPropertiesTest();

private Q_SLOTS:
    void testGroup();
    void testAddProperty();
    void testDefaultValue();
    void testSetValue();
    void testUpdateValue();
    void testNotification();
    void testRemoveProperty();
    void testReadWrite_data();
    void testReadWrite();
};

SharedPropertiesTest::SharedPropertiesTest()
{
}

void SharedPropertiesTest::testAddProperty()
{
    SharedProperties props;
    Object o1;

    // test invalid property first
    QVERIFY(!props.addProperty("size", &o1));

    // test valid property without notifier signal
    QVERIFY(!props.addProperty("propertyNoNotify", &o1));

    // test valid property
    QVERIFY(props.addProperty("property", &o1));
}

void SharedPropertiesTest::testDefaultValue()
{
    QString key = "property";
    QString value = "value";
    SharedProperties props;
    Object o1;

    props.setValue(key, value);

    QCOMPARE(o1.property(), QString());
    QVERIFY(props.addProperty(key, &o1));
    QCOMPARE(o1.property(), value);
}

void SharedPropertiesTest::testSetValue()
{
    QString key = "property";
    QString value = "value";
    SharedProperties props;
    Object o1, o2;

    QCOMPARE(o1.property(), QString());
    QCOMPARE(o2.property(), QString());

    QVERIFY(props.addProperty(key, &o1));
    QCOMPARE(o1.property(), QString());
    props.setValue(key, value);
    QCOMPARE(props.value(key).toString(), value);
    QCOMPARE(o1.property(), value);

    QVERIFY(props.addProperty(key, &o2));
    QCOMPARE(o2.property(), value);
}

void SharedPropertiesTest::testUpdateValue()
{
    QString key = "property";
    QString value = "value";
    SharedProperties props;
    Object o1, o2;

    QCOMPARE(o1.property(), QString());
    QCOMPARE(o2.property(), QString());

    QVERIFY(props.addProperty(key, &o1));
    QCOMPARE(o1.property(), QString());
    props.updateValue(key, value);
    QCOMPARE(props.value(key).toString(), value);
    QCOMPARE(o1.property(), QString()); // check if value didn't change

    QVERIFY(props.addProperty(key, &o2));
    QCOMPARE(o2.property(), value); // check if value assigned to new objects
}

void SharedPropertiesTest::testNotification()
{
    QString key = "property";
    QString value = "value";
    SharedProperties props;
    Object o1, o2;

    QCOMPARE(o1.property(), QString());
    QCOMPARE(o2.property(), QString());

    props.addProperty(key, &o1);
    QCOMPARE(o1.property(), QString());

    o1.setProperty(value);
    QCOMPARE(props.value(key).toString(), value);
    QCOMPARE(o1.property(), value);

    props.addProperty(key, &o2);
    QCOMPARE(o2.property(), value);
}

void SharedPropertiesTest::testRemoveProperty()
{
    QString key = "property";
    QString value = "value";
    QString value2 = "value2";
    SharedProperties props;
    Object o1, o2;

    QCOMPARE(o1.property(), QString());

    props.addProperty(key, &o1);
    props.addProperty(key, &o2);
    QCOMPARE(o1.property(), QString());
    QCOMPARE(o2.property(), QString());
    props.setValue(key, value);
    QCOMPARE(o1.property(), value);
    QCOMPARE(o2.property(), value);
    props.removeProperty(key, &o1);
    props.setValue(key, value2);
    QCOMPARE(o1.property(), value);
    QCOMPARE(o2.property(), value2);
}

void SharedPropertiesTest::testReadWrite_data()
{
    QTest::addColumn<QString>("key");
    QTest::addColumn<QVariant>("value");

    QTest::newRow("1") << "key" << QVariant("value");
    QTest::newRow("2") << "group/key" << QVariant("value");
}

void SharedPropertiesTest::testReadWrite()
{
    QFETCH(QString, key);
    QFETCH(QVariant, value);

    QSettings settings;
    settings.setValue(key, value);

    SharedProperties props;
    props.read(&settings);
    QVERIFY(props.value(key).isValid());
    QCOMPARE(props.value(key), settings.value(key));

    settings.clear();
    props.write(&settings);
    QVERIFY(settings.value(key).isValid());
    QCOMPARE(settings.value(key), props.value(key));
}

void SharedPropertiesTest::testGroup()
{
    QString key = "key";
    QString longKey = "group/key";
    QString group = "group";
    QVariant value = "value";

    SharedProperties props;

    QCOMPARE(props.value(key), QVariant());
    props.setValue(key, value);
    QCOMPARE(props.value(key), value);
    props.setValue(key, QVariant());

    QCOMPARE(props.value(longKey), QVariant());
    props.setValue(longKey, value);
    QCOMPARE(props.value(longKey), value);
    props.setValue(longKey, QVariant());

    props.beginGroup(group);
    QCOMPARE(props.group(), group);

    QCOMPARE(props.value(key), QVariant());
    props.setValue(key, value);
    QCOMPARE(props.value(key), value);
    props.endGroup();
    QCOMPARE(props.value(longKey), value);
}

QTEST_APPLESS_MAIN(SharedPropertiesTest)

#include "tst_sharedproperties.moc"
