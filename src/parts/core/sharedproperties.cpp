#include "sharedproperties.h"
#include "sharedproperties_p.h"

#include <QtCore/QDebug>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaProperty>
#include <QtCore/QSettings>
#include <QtCore/QThread>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QApplication>
#else
#include <QtGui/QApplication>
#endif

using namespace Parts;

typedef SharedPropertiesPrivate::Property Property;
typedef SharedPropertiesPrivate::Notifier Notifier;

static const char * const OnValueChangedSlot = "onValueChanged()";

static inline QString shortKey(const QString &longKey)
{
    int index = longKey.lastIndexOf('/');

    return longKey.mid(index + 1);
}

static inline QString longKey(const QString &group, const QString &shortKey)
{
    if (group.isEmpty())
        return shortKey;

    return group + '/' + shortKey;
}

static QMetaProperty objectProperty(const QObject *object, const QByteArray &propertyName)
{
    const QMetaObject *metaObject = object->metaObject();
    int propertyId = metaObject->indexOfProperty(propertyName.constData());

    if (propertyId == -1)
        return QMetaProperty();

    return metaObject->property(propertyId);
}

SharedPropertiesPrivate::SharedPropertiesPrivate(SharedProperties *qq) :
    q_ptr(qq)
{
}

/*!
    \internal

    Removes object from map. Object can already be deleted.
*/
void SharedPropertiesPrivate::removeObject(QObject *object)
{
    QMutableMapIterator<QString, Property> it1(mapKeyToProperty);
    while (it1.hasNext()) {
        it1.next();
        if (it1.value().object == object)
            it1.remove();
    }

    QMutableMapIterator<Notifier, QString> it2(mapNotifierToKey);
    while (it2.hasNext()) {
        it2.next();
        if (it2.key().object == object)
            it2.remove();
    }
}

void SharedPropertiesPrivate::disconnectNotifier(const Notifier &notifierKey)
{
    Q_Q(SharedProperties);

    QObject *object = notifierKey.object;
    int notifierId = notifierKey.id;

    const QMetaObject *metaObject = object->metaObject();
    QMetaMethod notifier = metaObject->method(notifierId);
    QMetaMethod handler = handlerMethod();
    QObject::disconnect(object, notifier, q, handler);
}

void SharedPropertiesPrivate::setDefaultValue(const QString &key, const QVariant &value)
{
    Q_Q(SharedProperties);

    if (values.value(key) != value) {
        values.insert(key, value);
        emit q->valueChanged(key, value);
    }
}

void SharedPropertiesPrivate::notifyValueChanged(const QString &key, const QVariant &value)
{
    foreach (const Property &prop, mapKeyToProperty.values(key)) {
        const QMetaObject *metaObject = prop.object->metaObject();
        QMetaProperty property = metaObject->property(prop.id);

        // this hack is required for Linux because QSettings
        // restores integers as strings.
        QVariant trueValue = value;
        trueValue.convert(property.type());

        property.write(prop.object, trueValue);
    }
}

QMetaMethod SharedPropertiesPrivate::handlerMethod()
{
    Q_Q(SharedProperties);

    const QMetaObject *metaObject = q->metaObject();
    int handlerId = metaObject->indexOfSlot(OnValueChangedSlot);
    return metaObject->method(handlerId);
}

SharedProperties::SharedProperties(QObject *parent) :
    QObject(parent),
    d_ptr(new SharedPropertiesPrivate(this))
{
}

SharedProperties::~SharedProperties()
{
    delete d_ptr;
}

bool SharedProperties::addProperty(const QString &key, QObject *object)
{
    return addProperty(key, object, shortKey(key).toLatin1());
}

bool SharedProperties::addProperty(const QString &key, QObject *object, const QByteArray &propertyName)
{
    Q_D(SharedProperties);

    QString longKey = ::longKey(d->group, key);

    QMetaProperty metaProperty = ::objectProperty(object, propertyName);
    if (!metaProperty.isValid()) {
        qWarning() << "SharedProperties::addObject :" << "Object" << object << "doesn't have property" << propertyName;
        return false;
    }

    QMetaMethod notifierMethod = metaProperty.notifySignal();
    if (notifierMethod.methodIndex() == -1) {
        qWarning() << "SharedProperties::addObject :" << "Property" << propertyName << "doesn't have notifier signal";
        return false;
    }

    Property propertyKey(object, metaProperty.propertyIndex());
    d->mapKeyToProperty.insert(longKey, propertyKey);

    if (d->values.contains(longKey))
        metaProperty.write(object, d->values.value(longKey));

    Notifier notifierKey(object, notifierMethod.methodIndex());
    d->mapNotifierToKey.insert(notifierKey, longKey);

    connect(object, notifierMethod, this, d->handlerMethod());

    return true;
}

void SharedProperties::removeProperty(const QString &key, QObject *object)
{
    removeProperty(key, object, ::shortKey(key).toLatin1());
}

void SharedProperties::removeProperty(const QString &key, QObject *object, const QByteArray &propertyName)
{
    Q_D(SharedProperties);

    QString longKey = ::longKey(d->group, key);

    QMetaProperty metaProperty = ::objectProperty(object, propertyName);
    if (!metaProperty.isValid()) {
        qWarning() << "SharedProperties::addObject :" << "Object" << object << "doesn't have property" << propertyName;
        return;
    }

    QMetaMethod notifierMethod = metaProperty.notifySignal();
    if (notifierMethod.methodIndex() == -1) {
        qWarning() << "SharedProperties::addObject :" << "Property" << propertyName << "doesn't have notifier signal";
        return;
    }

    Property propertyKey(object, metaProperty.propertyIndex());
    d->mapKeyToProperty.remove(longKey, propertyKey);

    Notifier notifierKey(object, notifierMethod.methodIndex());
    d->mapNotifierToKey.remove(notifierKey/*, longKey*/);

    disconnect(object, notifierMethod, this, d->handlerMethod());
}

void SharedProperties::removeProperties(QObject *object)
{
    Q_D(SharedProperties);

    QMapIterator<Notifier, QString> it(d->mapNotifierToKey);
    while (it.hasNext()) {
        it.next();
        if (it.key().object == object)
            d->disconnectNotifier(it.key());
    }

    d->removeObject(object);
}

void SharedProperties::removeAll()
{
    Q_D(SharedProperties);

    QMapIterator<Notifier, QString> it(d->mapNotifierToKey);
    while (it.hasNext()) {
        it.next();
        d->disconnectNotifier(it.key());
    }

    d->mapKeyToProperty.clear();
    d->mapNotifierToKey.clear();
}

QVariant SharedProperties::value(const QString &key, const QVariant &defaulValue) const
{
    Q_D(const SharedProperties);

    QString longKey = ::longKey(d->group, key);
    return d->values.value(longKey, defaulValue);
}

QVariantMap SharedProperties::values() const
{
    Q_D(const SharedProperties);
    return d->values;
}

void SharedProperties::setValue(const QString &key, const QVariant &value)
{
    Q_D(SharedProperties);

    QString longKey = ::longKey(d->group, key);
    if (d->values.value(longKey) == value)
        return;

    d->values.insert(longKey, value);
    d->notifyValueChanged(longKey, value);
    emit valueChanged(longKey, value);
}

void SharedProperties::updateValue(const QString &key, const QVariant &value)
{
    Q_D(SharedProperties);

    QString longKey = ::longKey(d->group, key);
    if (d->values.value(longKey) == value)
        return;

    d->values.insert(longKey, value);
    emit valueChanged(longKey, value);
}

void SharedProperties::clear()
{
    Q_D(SharedProperties);

    removeAll();

    QStringList keys = d->values.keys();
    d->values.clear();

    for (int i = 0; i < keys.count(); ++i) {
        const QString &key = keys.at(i);
        emit valueChanged(key, QVariant());
    }
}

QString SharedProperties::group() const
{
    Q_D(const SharedProperties);

    return d->group;
}

void SharedProperties::beginGroup(const QString &group)
{
    Q_D(SharedProperties);

    d->groupStack.append(group);
    QStringList tmp = d->groupStack;
    tmp.removeAll(QString());
    d->group = tmp.join("/");
}

void SharedProperties::endGroup()
{
    Q_D(SharedProperties);

    if (d->groupStack.isEmpty()) {
        qWarning() << "Not symmetrical calls to SharedProperties::beginGroup()/endGroup()";
        return;
    }

    d->groupStack.takeLast();
    d->group = d->groupStack.join("/");
}

void SharedProperties::read(const QSettings *settings)
{
    Q_D(SharedProperties);

    if (!settings)
        return;

    d->values.clear();
    foreach (const QString &key, settings->allKeys()) {
        setValue(key, settings->value(key));
    }
}

void SharedProperties::write(QSettings *settings)
{
    Q_D(SharedProperties);

    if (!settings)
        return;

    QMapIterator<QString, QVariant> it(d->values);
    while (it.hasNext()) {
        it.next();
        settings->setValue(it.key(), it.value());
    }

}

void SharedProperties::onDestroyed(QObject *object)
{
    Q_D(SharedProperties);
    d->removeObject(object);
}

void SharedProperties::onValueChanged()
{
    Q_D(SharedProperties);

    QObject *object = sender();
    const QMetaObject *metaObject = object->metaObject();
    int notifierId = senderSignalIndex();
    Notifier notifierKey(object, notifierId);
    QString longKey = d->mapNotifierToKey.value(notifierKey);

    Property propertyKey;
    foreach (propertyKey, d->mapKeyToProperty.values(longKey)) {
        if (propertyKey.object == object)
            break;
    }

    QMetaProperty metaProperty = metaObject->property(propertyKey.id);
    QVariant value = metaProperty.read(object);
    d->setDefaultValue(longKey, value);
}
