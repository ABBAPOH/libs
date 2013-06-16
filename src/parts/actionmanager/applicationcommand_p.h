#ifndef APPLICATIONCOMMAND_P_H
#define APPLICATIONCOMMAND_P_H

#include "applicationcommand.h"
#include "command_p.h"

namespace Parts {

class ApplicationCommandPrivate : public CommandPrivate
{
    Q_DECLARE_PUBLIC(ApplicationCommand)
    Q_DISABLE_COPY(ApplicationCommandPrivate)

public:
    explicit ApplicationCommandPrivate(ApplicationCommand *qq) : CommandPrivate(qq) {}

    void onDefaultShortcutChanged(const QKeySequence &key);
    void onIconChanged(const QIcon &icon);
    void onShortcutChanged(const QKeySequence &key);
    void onTextChanged(const QString &text);
    void onToolTipChanged(const QString &toolTip);

public:
    QAction *action;
};

} // namespace Parts

#endif // APPLICATIONCOMMAND_P_H
