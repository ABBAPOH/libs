#include "command.h"
#include "command_p.h"

#include "actionmanager.h"

using namespace Parts;

/*!
    \class Parts::Command
    \brief Base class for non-container commands.
*/

/*!
    Creates a Command with the given \a id and \a parent.
*/
Command::Command(const QByteArray &id, QObject *parent) :
    AbstractCommand(*new CommandPrivate(this), id, parent)
{
}

/*!
    \internal
*/
Command::Command(Parts::AbstractCommandPrivate &dd, const QByteArray &id, QObject *parent) :
    AbstractCommand(dd, id, parent)
{
    ActionManager::instance()->registerCommand(this);
}

/*!
    \brief Destroys the Command.
*/
Command::~Command()
{
    ActionManager::instance()->unregisterCommand(this);
}

/*!
    \property Command::configurable

    \brief Holds whether property can be configured by user or not.
*/

bool Command::isConfigurable() const
{
    Q_D(const Command);

    return d->configurable;
}

void Command::setConfigurable(bool configurable)
{
    Q_D(Command);

    if (d->configurable == configurable)
        return;

    if (!configurable)
        setShortcut(d->defaultShortcut);
    d->configurable = configurable;

    emit changed();
}

/*!
    \property Command::data

    \brief Property that stores user-specific data.
*/

QVariant Command::data() const
{
    Q_D(const Command);

    return d->data;
}

void Command::setData(const QVariant &data)
{
    Q_D(Command);

    if (d->data == data)
        return;

    d->data = data;
    d->onDataChanged(data);

    emit changed();
}

/*!
    \property Command::defaultShortcut

    \brief Default Command's shortcut
*/

QKeySequence Command::defaultShortcut() const
{
    Q_D(const Command);

    return d->defaultShortcut;
}

void Command::setDefaultShortcut(const QKeySequence &key)
{
        Q_D(Command);

    if (d->defaultShortcut == key)
        return;

    QKeySequence oldShortcut = d->defaultShortcut;
    d->defaultShortcut = key;
    d->onDefaultShortcutChanged(key);

    if (d->shortcut == oldShortcut) {
        d->shortcut = key;
        d->onShortcutChanged(key);
    }

    emit changed();
}

/*!
    \property Command::shortcut

    \brief Overrided Command's shortcut

    This property overrides current shortcut if Command::isConfigurable is
    false.
*/

QKeySequence Command::shortcut() const
{
    return d_func()->shortcut;
}

void Command::setShortcut(const QKeySequence &key)
{
    Q_D(Command);

    if (!d->configurable)
        return;

    if (d->shortcut == key)
        return;

    d->shortcut = key;
    d->onShortcutChanged(key);

    emit changed();
}

/*!
    \property Command::toolTip

    \brief ToolTip shown to user
*/

QString Command::toolTip() const
{
    Q_D(const Command);

    return d->toolTip;
}

void Command::setToolTip(const QString &toolTip)
{
    Q_D(Command);

    if (d->toolTip == toolTip)
        return;

    d->toolTip = toolTip;
    d->onToolTipChanged(toolTip);

    emit changed();
}
