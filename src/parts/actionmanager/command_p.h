#ifndef COMMAND_P_H
#define COMMAND_P_H

#include "command.h"
#include "abstractcommand_p.h"

namespace Parts {

class CommandPrivate : public AbstractCommandPrivate
{
    Q_DECLARE_PUBLIC(Command)
public:
    CommandPrivate(Command *qq) : AbstractCommandPrivate(qq), configurable(true) {}

    QString category;
    bool configurable;
    QVariant data;
    QKeySequence defaultShortcut;
    QKeySequence shortcut;
    QString toolTip;

    virtual void onDataChanged(const QVariant &data) { Q_UNUSED(data); }
    virtual void onDefaultShortcutChanged(const QKeySequence &key) { Q_UNUSED(key); }
    virtual void onShortcutChanged(const QKeySequence &key) { Q_UNUSED(key); }
    virtual void onToolTipChanged(const QString &toolTip) { Q_UNUSED(toolTip); }
};

} // namespace Parts

#endif // COMMAND_P_H
