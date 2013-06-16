#ifndef ABSTRACTCOMMAND_P_H
#define ABSTRACTCOMMAND_P_H

#include "abstractcommand.h"

namespace Parts {

class AbstractCommandPrivate
{
    Q_DECLARE_PUBLIC(AbstractCommand)
public:
    AbstractCommandPrivate(AbstractCommand *qq) : q_ptr(qq) {}
    virtual ~AbstractCommandPrivate() {}

    virtual void onTextChanged(const QString &text) { Q_UNUSED(text); }
    virtual void onIconChanged(const QIcon &icon) { Q_UNUSED(icon); }

    QByteArray id;
    QString text;
    QIcon icon;

protected:
    AbstractCommand *q_ptr;
};

} // namespace Parts

#endif // ABSTRACTCOMMAND_P_H
