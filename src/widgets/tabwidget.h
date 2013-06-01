#ifndef TABCONTAINER_P_H
#define TABCONTAINER_P_H

#include "widgets_global.h"

#if QT_VERSION >= 0x050000
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QAbstractButton>
#else
#include <QtGui/QTabWidget>
#include <QtGui/QAbstractButton>
#endif

class WIDGETS_EXPORT TabWidget : public QTabWidget
{
    Q_OBJECT
    Q_DISABLE_COPY(TabWidget)

    Q_PROPERTY(bool tabBarVisible READ isTabBarVisible WRITE setTabBarVisible NOTIFY tabBarVisibleChanged)
public:
    explicit TabWidget(QWidget *parent = 0);

    bool isTabBarVisible() const;
    void setTabBarVisible(bool visible);

signals:
    void tabBarDoubleClicked();
    void tabBarVisibleChanged(bool visible);

protected:
    bool eventFilter(QObject *object, QEvent *event);
};

#endif // TABCONTAINER_P_H
