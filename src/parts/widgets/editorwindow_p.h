#ifndef MAINWINDOW_P_H
#define MAINWINDOW_P_H

#include "editorwindow.h"

#include <QtCore/QPointer>

#if QT_VERSION >= 0x050000
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QTabBar>
#else
#include <QtGui/QDockWidget>
#include <QtGui/QTabBar>
#endif

class QToolButton;

namespace Parts {

class AbstractDocument;
class History;
class HistoryButton;
class ToolWidgetFactory;

class EditorWindowPrivate
{
    Q_DECLARE_PUBLIC(EditorWindow)

public:
    explicit EditorWindowPrivate(EditorWindow *qq) : q_ptr(qq) {}

    void createActions();
    void retranslateUi();
    void initGeometry();
    void createTools();
    QDockWidget *createTool(ToolWidgetFactory *factory);

public:
    QPointer<AbstractEditor> editor;
    QPointer<AbstractDocument> document;

    QAction *actions[EditorWindow::ActionCount];

    bool menuVisible;
#ifndef Q_OS_MAC
    QToolButton *menuBarButton;
#endif

private:
    EditorWindow *q_ptr;

    class DockWidget;
};

class EditorWindowPrivate::DockWidget : public QDockWidget
{
    Q_OBJECT
public:
    explicit DockWidget(QWidget *parent);

protected:
    void changeEvent(QEvent *event);

private:
    class TabBar;
};

class EditorWindowPrivate::DockWidget::TabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit TabBar(QWidget *parent);

    QSize minimumSizeHint() const;
protected:
    QSize tabSizeHint(int index) const;
};

} // namespace Parts

#endif // MAINWINDOW_P_H
