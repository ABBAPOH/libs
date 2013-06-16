#ifndef COMMAND_H
#define COMMAND_H

#include "abstractcommand.h"

#include <QtCore/QVariant>
#include <QtGui/QKeySequence>

namespace Parts {

class CommandPrivate;
class PARTS_EXPORT Command : public AbstractCommand
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Command)
    Q_DISABLE_COPY(Command)

    Q_PROPERTY(bool configurable READ isConfigurable WRITE setConfigurable)
    Q_PROPERTY(QVariant data READ data WRITE setData)
    Q_PROPERTY(QKeySequence defaultShortcut READ defaultShortcut WRITE setDefaultShortcut)
    Q_PROPERTY(QKeySequence shortcut READ shortcut WRITE setShortcut)
    Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)

public:
    explicit Command(const QByteArray &id, QObject *parent = 0);
    ~Command();

    bool isConfigurable() const;
    void setConfigurable(bool configurable);

    QVariant data() const;
    void setData(const QVariant &data);

    QKeySequence defaultShortcut() const;
    void setDefaultShortcut(const QKeySequence &key);

    QKeySequence shortcut() const;
    void setShortcut(const QKeySequence &key);

    QString toolTip() const;
    void setToolTip(const QString &toolTip);

protected:
    Command(AbstractCommandPrivate &dd, const QByteArray &id, QObject *parent = 0);
};

} // namespace Parts

#endif // COMMAND_H
