#include "contextcommand.h"

#include "command_p.h"

#include <QtGui/QKeySequence>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QAction>
#else
#include <QtGui/QAction>
#endif

#include "proxyaction.h"

namespace Parts {

class ContextCommandPrivate : public CommandPrivate
{
    Q_DECLARE_PUBLIC(ContextCommand)
public:
    ContextCommandPrivate(ContextCommand *qq) : CommandPrivate(qq) {}

    ProxyAction *action;
    QAction *realAction;

    ContextCommand::Attributes attributes;

    void init();
    void update();
    void onDefaultShortcutChanged(const QKeySequence &key);
    void onIconChanged(const QIcon &icon);
    void onShortcutChanged(const QKeySequence &key);
    void onTextChanged(const QString &text);
    void onToolTipChanged(const QString &toolTip);
};

} // namespace Parts

using namespace Parts;

void ContextCommandPrivate::init()
{
    Q_Q(ContextCommand);
    action = new ProxyAction(q);
    action->setEnabled(false);
    realAction = 0;
}

void ContextCommandPrivate::update()
{
    if (attributes & ContextCommand::AttributeHide)
        action->setVisible(bool(realAction));
    if (attributes & ContextCommand::AttributeUpdateText && !realAction)
        action->setText(text);
    if (attributes & ContextCommand::AttributeUpdateIcon && !realAction)
        action->setIcon(icon);
    if (attributes & ContextCommand::AttributeUpdateShortcut && !realAction) {
        action->setShortcut(defaultShortcut);
    }

    if (!realAction)
        action->setEnabled((attributes & ContextCommand::AttributeUpdateEnabled));
}

void ContextCommandPrivate::onDefaultShortcutChanged(const QKeySequence &key)
{
    if (!realAction || !(attributes & ContextCommand::AttributeUpdateShortcut))
        action->setShortcut(key);
}

void ContextCommandPrivate::onIconChanged(const QIcon &icon)
{
    if (!realAction || !(attributes & ContextCommand::AttributeUpdateIcon))
        action->setIcon(icon);
}

void ContextCommandPrivate::onShortcutChanged(const QKeySequence &key)
{
    if (shortcut != defaultShortcut) {
        action->setShortcut(key);
        action->setAttributes(ProxyAction::Attributes((int)(attributes & ~ContextCommand::AttributeUpdateShortcut)));
    } else {
        if (attributes & ContextCommand::AttributeUpdateShortcut && realAction)
            action->setShortcut(realAction->shortcut());
        else
            action->setShortcut(defaultShortcut);
        action->setAttributes(ProxyAction::Attributes((int)(attributes)));
    }
}

void ContextCommandPrivate::onTextChanged(const QString &text)
{
    if (!realAction || !(attributes & ContextCommand::AttributeUpdateText))
        action->setText(text);
}

void ContextCommandPrivate::onToolTipChanged(const QString &toolTip)
{
    if (!realAction || !(attributes & ContextCommand::AttributeUpdateText))
        action->setToolTip(toolTip);
}

/*!
    \class Parts::ContextCommand

    The ContextCommand serves as a wrapper around a proxy action which can be
    linked to actions added to QWidgets.

    Proxy action is linked by name - action with the same object name as this
    object's id is automatically connected to the proxy action.
*/

/*!
    \brief Constructs Context with the given \a id and \a parent.
*/
ContextCommand::ContextCommand(const QByteArray &id, QObject *parent) :
    Command(*new ContextCommandPrivate(this), id, parent)
{
    Q_D(ContextCommand);
    d->init();
}

/*!
    \brief Destroys ContextCommand.
*/
ContextCommand::~ContextCommand()
{
}

/*!
    \enum Parts::Context::Attribute

    This enum type describes Commands parameters.

    \value AttributeHide Command is hidden instead of disabling when no action is linked with it
    \value AttributeUpdateText Command uses linked action's text instead of defaultText
    \value AttributeUpdateIcon Command uses linked action's icon instead of defaultIcon
*/

/*!
    \enum Parts::Command::CommandContext

    This enum type set's Commands context, which can be one of following:

    \value WidgetCommand Command is linked to action added to focus widget.
    \value WidgetWithChildrenCommand Command is linked to action added to focus widget or one of it's parents.
    \value WindowCommand Command is linked to action added to widget that belongs to currently active window.
    \value ApplicationCommand Command is linked to it's own action and always enabled.
*/

/*!
    \brief Returns pointer to the ContextCommand's proxy action.
*/
QAction * ContextCommand::commandAction() const
{
    return d_func()->action;
}

/*!
    \property ContextCommand::attributes

    \brief ContextCommand's attributes.

    Default value is 0.
*/
ContextCommand::Attributes ContextCommand::attributes() const
{
    return d_func()->attributes;
}

/*!
    Returns true if attribute is set for this command.
*/
bool ContextCommand::hasAttribute(ContextCommand::Attribute attr) const
{
    return d_func()->attributes & attr;
}

void ContextCommand::setAttributes(Attributes attrs)
{
    Q_D(ContextCommand);

    if (d->attributes != attrs) {
        d->attributes = attrs;

        if (attrs & AttributeHide && !d->realAction)
            d->action->setVisible(false);
        d->action->setAttributes(ProxyAction::Attributes((int)attrs >> 1));
        if (!d->realAction)
            d->action->setEnabled(attrs & AttributeUpdateEnabled);

        emit changed();
    }
}

/*!
    \internal
*/
QAction * ContextCommand::realAction() const
{
    Q_D(const ContextCommand);

    return d->realAction;
}

/*!
    \internal

    Links \a action to this Command.
*/
void ContextCommand::setRealAction(QAction *action)
{
    Q_D(ContextCommand);

    if (d->realAction != action) {
        d->realAction = action;
        d->action->setAction(action);
        if (shortcut() != defaultShortcut())
            d->action->setShortcut(shortcut());
        d->update();
    }
}

/*!
    \reimp
*/
QAction * ContextCommand::createAction(QObject * /*parent*/) const
{
    Q_D(const ContextCommand);
    return d->action;
}
