#ifndef TOOLBAR_H
#define TOOLBAR_H

#include "../parts_global.h"

#include <QtCore/QMap>
#include <QtCore/QPointer>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QToolBar>
#else
#include <QtGui/QToolBar>
#endif

namespace Parts {

class AbstractCommand;
class CommandContainer;

class PARTS_EXPORT ToolBar : public QToolBar
{
    Q_OBJECT
    Q_DISABLE_COPY(ToolBar)

public:
    explicit ToolBar(QWidget *parent = 0);

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

#endif // TOOLBAR_H
