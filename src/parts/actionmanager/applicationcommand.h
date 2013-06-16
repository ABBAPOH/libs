#ifndef APPLICATIONCOMMAND_H
#define APPLICATIONCOMMAND_H

#include "command.h"

namespace Parts {

class ApplicationCommandPrivate;
class PARTS_EXPORT ApplicationCommand : public Command
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ApplicationCommand)
    Q_DISABLE_COPY(ApplicationCommand)

    Q_PROPERTY(bool checkable READ isCheckable WRITE setCheckable)
    Q_PROPERTY(bool checked READ isChecked WRITE setChecked)

public:
    explicit ApplicationCommand(const QByteArray &id, QObject *parent = 0);
    ~ApplicationCommand();

    const QAction *action() const;

    bool isCheckable() const;
    void setCheckable(bool checkable);

    bool isChecked() const;

    QAction *createAction(QObject *parent = 0) const;

public slots:
    void setChecked(bool checked);

signals:
    void toggled(bool checked);
    void triggered(bool checked = false);
};

} // namespace Parts

#endif // APPLICATIONCOMMAND_H
