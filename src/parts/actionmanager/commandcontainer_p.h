#ifndef COMMANDCONTAINER_P_H
#define COMMANDCONTAINER_P_H

#include "abstractcommand_p.h"
#include "commandcontainer.h"
#include "menu.h"

namespace Parts {

class CommandContainerPrivate : public AbstractCommandPrivate
{
    Q_DECLARE_PUBLIC(CommandContainer)
public:
    explicit CommandContainerPrivate(CommandContainer *qq) :
        AbstractCommandPrivate(qq),
        menu(0)
    {}

    void onTextChanged(const QString &text);
    void onIconChanged(const QIcon &icon);

public:
    QList<AbstractCommand *> commands;
    mutable Menu *menu;
};

} // namespace Parts

#endif // COMMANDCONTAINER_P_H
