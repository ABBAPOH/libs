#ifndef WINDOWSMENU_H
#define WINDOWSMENU_H

#include "widgets_global.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QMenu>
#else
#include <QtGui/QMenu>
#endif

class WindowsMenuPrivate;
class WIDGETS_EXPORT WindowsMenu : public QMenu
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(WindowsMenu)
    Q_DISABLE_COPY(WindowsMenu)
public:
    explicit WindowsMenu(QWidget *parent = 0);
    ~WindowsMenu();

    QAction *minimizeAction() const;
    QAction *toggleFullscreenAction() const;
    QAction *nextAction() const;
    QAction *prevAction() const;

public slots:
    void minimizeWindow();
    void toggleFullscreen();
    void nextWindow();
    void prevWindow();

private slots:
    void onTriggered(bool checked);

protected:
    void changeEvent(QEvent *event);
    bool eventFilter(QObject *o, QEvent *e);

private:
    WindowsMenuPrivate *d_ptr;
};

#endif // WINDOWSMENU_H
