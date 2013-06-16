#ifndef MENU_H
#define MENU_H

#include "../parts_global.h"

#include <QtCore/QPointer>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenu>
#else
#include <QtGui/QMenu>
#endif

namespace Parts {

class AbstractCommand;
class CommandContainer;

class PARTS_EXPORT Menu : public QMenu
{
    Q_OBJECT
    Q_DISABLE_COPY(Menu)

public:
    explicit Menu(QWidget *parent = 0);

    CommandContainer *container() const;
    void setContainer(CommandContainer *container);
    void setContainer(const QByteArray &containerId);

private slots:
    void onCommandInserted(AbstractCommand *command, AbstractCommand *commandBefore);
    void onCommandRemoved(AbstractCommand *command);
    void onContainerChanged();

private:
    void populate();

private:
    CommandContainer *m_container;
    QMap<AbstractCommand *, QPointer<QAction> > m_mapToAction;
};

} // namespace Parts

#endif // MENU_H
