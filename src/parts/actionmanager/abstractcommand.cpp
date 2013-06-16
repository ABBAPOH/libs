#include "abstractcommand.h"
#include "abstractcommand_p.h"

using namespace Parts;

/*!
    \class Parts::AbstractCommand

    \brief Base class for Command and CommandContainer.

    Normally, you should not use this class directly, use its subclasses instead.
*/

/*!
    Creates an AbstractCommand with the given \a id and \a parent.
*/
AbstractCommand::AbstractCommand(const QByteArray &id, QObject *parent) :
    QObject(parent),
    d_ptr(new AbstractCommandPrivate(this))
{
    Q_D(AbstractCommand);
    d->id = id;
}

/*!
    \internal
*/
AbstractCommand::AbstractCommand(AbstractCommandPrivate &dd, const QByteArray &id, QObject *parent) :
    QObject(parent),
    d_ptr(&dd)
{
    Q_D(AbstractCommand);
    d->id = id;
}

/*!
    \brief Destroys AbstractCommand.
*/
AbstractCommand::~AbstractCommand()
{
    delete d_ptr;
}

/*!
    \property AbstractCommand::id

    \brief  Id is used to identify commands.
*/
QByteArray AbstractCommand::id() const
{
    Q_D(const AbstractCommand);
    return d->id;
}

/*!
    \property AbstractCommand::text

    \brief Default command's text

    Text is displayed in command settings and in menus.
*/
QString AbstractCommand::text() const
{
    Q_D(const AbstractCommand);
    return d->text;
}

void AbstractCommand::setText(const QString &text)
{
    Q_D(AbstractCommand);

    if (d->text == text)
        return;

    d->text = text;
    d->onTextChanged(text);

    emit changed();
}

/*!
    \property AbstractCommand::icon

    \brief Default command's icon

    Icon is displayed in command settings and in menus.
*/
QIcon AbstractCommand::icon() const
{
    Q_D(const AbstractCommand);
    return d->icon;
}

void AbstractCommand::setIcon(const QIcon &icon)
{
    Q_D(AbstractCommand);

    d->icon = icon;
    d->onIconChanged(icon);
    emit changed();
}
