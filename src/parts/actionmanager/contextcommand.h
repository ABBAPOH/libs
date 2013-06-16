#ifndef CONTEXTCOMMAND_H
#define CONTEXTCOMMAND_H

#include "../parts_global.h"

#include "command.h"

#include <QtCore/QVariant>

#include <QtGui/QKeySequence>

class QAction;
class QIcon;

namespace Parts {

class ContextCommandPrivate;
class PARTS_EXPORT ContextCommand : public Command
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ContextCommand)
    Q_DISABLE_COPY(ContextCommand)

    Q_PROPERTY(Attributes attributes READ attributes WRITE setAttributes)

public:
    enum Attribute {
        AttributeHide = 0x2,
        AttributeUpdateText = 0x4,
        AttributeUpdateIcon = 0x8,
        AttributeUpdateShortcut = 0x10,
        AttributeUpdateEnabled = 0x20
    };
    Q_DECLARE_FLAGS(Attributes, Attribute)

    explicit ContextCommand(const QByteArray &id, QObject *parent = 0);
    ~ContextCommand();

    QAction *commandAction() const;

    Attributes attributes() const;
    bool hasAttribute(Attribute attr) const;
    void setAttributes(Attributes attr);

protected:
    QAction *realAction() const;
    void setRealAction(QAction *commandAction);

    QAction *createAction(QObject *parent) const;

protected:
    friend class ActionManager;
    friend class CommandContainer;
};

} // namespace Parts

#endif // CONTEXTCOMMAND_H
