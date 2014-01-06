#include "applicationcommand.h"
#include "applicationcommand_p.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif

#include "actionmanager.h"

using namespace Parts;

void ApplicationCommandPrivate::init()
{
    Q_Q(ApplicationCommand);
    action = new QAction(q);
    q->connect(action, SIGNAL(toggled(bool)), q, SIGNAL(toggled(bool)));
    q->connect(action, SIGNAL(triggered(bool)), q, SIGNAL(triggered(bool)));
}

void ApplicationCommandPrivate::onDefaultShortcutChanged(const QKeySequence &key)
{
    action->setShortcut(key);
}

void ApplicationCommandPrivate::onIconChanged(const QIcon &icon)
{
    action->setIcon(icon);
}

void ApplicationCommandPrivate::onShortcutChanged(const QKeySequence &key)
{
    action->setShortcut(key);
}

void ApplicationCommandPrivate::onTextChanged(const QString &text)
{
    action->setText(text);
}

void ApplicationCommandPrivate::onToolTipChanged(const QString &toolTip)
{
    action->setToolTip(toolTip);
}

/*!
    \class Parts::ApplicationCommand

    \brief Class for implementing single actions.

    This class provides a QAction-like API that can be used to implement
    application-wide actions.
*/

/*!
    Creates an ApplicationCommand with the given \a id and \a parent.
*/
ApplicationCommand::ApplicationCommand(const QByteArray &id, QObject *parent) :
    Command(*new ApplicationCommandPrivate(this), id, parent)
{
    Q_D(ApplicationCommand);
    d->init();
}

ApplicationCommand::ApplicationCommand(QObject *parent) :
    Command(*new ApplicationCommandPrivate(this), QByteArray::number(quintptr(this)), parent)
{
    Q_D(ApplicationCommand);
    d->init();
}

/*!
    \brief Destroys ApplicationCommand.
*/
ApplicationCommand::~ApplicationCommand()
{
}

/*!
    Returns pointer to an internal action.

    You are not supposed to modify action (use ApplicationCommand API instead),
    but you can connect to action signals if necessary.
*/
const QAction * ApplicationCommand::action() const
{
    Q_D(const ApplicationCommand);
    return d->action;
}

/*!
    \property ApplicationCommand::checkable

    \brief Holds wheter command is checkable or not.
    Default value is false.
*/

bool ApplicationCommand::isCheckable() const
{
    Q_D(const ApplicationCommand);
    return d->action->isCheckable();
}

void ApplicationCommand::setCheckable(bool checkable)
{
    Q_D(const ApplicationCommand);
    if (d->action->isCheckable() == checkable)
        return;

    d->action->setCheckable(checkable);
    emit changed();
}

/*!
    \property ApplicationCommand::checked

    \brief Holds wheter command is checked or not.
    Default value is false.
*/

bool ApplicationCommand::isChecked() const
{
    Q_D(const ApplicationCommand);
    return d->action->isChecked();
}

void ApplicationCommand::setChecked(bool checked)
{
    Q_D(const ApplicationCommand);
    if (d->action->isChecked() == checked)
        return;

    d->action->setChecked(checked);
    emit changed();
}

bool ApplicationCommand::isEnabled() const
{
    Q_D(const ApplicationCommand);
    return d->action->isEnabled();
}

void ApplicationCommand::setEnabled(bool enabled)
{
    Q_D(const ApplicationCommand);
    if (d->action->isEnabled() == enabled)
        return;

    d->action->setEnabled(enabled);
    emit changed();
}

/*!
    \reimp
*/
QAction * ApplicationCommand::createAction(QObject * /*parent*/) const
{
    Q_D(const ApplicationCommand);
    return d->action;
}
