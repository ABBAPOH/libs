#include "separator.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif

#include "abstractcommand_p.h"

namespace Parts {

class SeparatorPrivate : public AbstractCommandPrivate
{
    Q_DECLARE_PUBLIC(Separator)
public:
    explicit SeparatorPrivate(Separator *qq) : AbstractCommandPrivate(qq) {}

public:
    QAction *action;
};

} // namespace Parts

using namespace Parts;

/*!
    \class Parts::Separator

    \brief Helper class to add separators to CommandContainers.

    Due to plugin architecture, it is not possible to add separators that have containter as a parent,
    so we need this class to correctly remove separators when plugins are unloaded.
*/

/*!
    \brief Constructs Separator with the givena \a parent and registers it in ActionManager.

    Separator has auto-generated id that is calculated as a string representation of an object pointer.
*/
Separator::Separator(QObject *parent) :
    AbstractCommand(*new SeparatorPrivate(this), QString::number(quintptr(this), 16).toLatin1(), parent)
{
    Q_D(Separator);
    d->action = new QAction(this);
    d->action->setSeparator(true);
}

/*!
    \property Separator::isVisible

    \brief Holds whether separator is visible or not.
*/

bool Parts::Separator::isVisible() const
{
    Q_D(const Separator);
    return d->action->isVisible();
}

void Separator::setVisible(bool visible)
{
    Q_D(Separator);

    if (d->action->isVisible() == visible)
        return;

    d->action->setVisible(visible);
    emit changed();
}

/*!
    \reimp
*/
QAction * Separator::createAction(QObject * /*parent*/) const
{
    Q_D(const Separator);
    return d->action;
}
