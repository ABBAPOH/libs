#ifndef MENUBAR_H
#define MENUBAR_H

#include "../parts_global.h"

#include <QtCore/QPointer>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenuBar>
#else
#include <QtGui/QMenuBar>
#endif

namespace Parts {

class AbstractCommand;
class CommandContainer;

class PARTS_EXPORT MenuBar : public QMenuBar
{
    Q_OBJECT
    Q_DISABLE_COPY(MenuBar)

public:
    explicit MenuBar(QWidget *parent = 0);

    CommandContainer *container() const;
    void setContainer(CommandContainer *container);
    void setContainer(const QByteArray &containerId);

private slots:
    void onCommandInserted(AbstractCommand *command, AbstractCommand *commandBefore);
    void onCommandRemoved(AbstractCommand *command);

private:
    void populate();

private:
    CommandContainer *m_container;
    QMap<AbstractCommand *, QPointer<QAction> > m_mapToAction;
};

} // namespace Parts

#endif // MENUBAR_H
